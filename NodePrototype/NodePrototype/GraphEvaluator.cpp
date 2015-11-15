#include "GraphEvaluator.h"

#include <queue>
#include <thread>
#include <mutex>
#include <future>
#include <set>

bool GraphEvaluator::addNode(PtrNode node, std::shared_ptr<SyncLock> lock /*= nullptr*/)
{
	bool ok = hasNode(node) == false;

	if (ok)
	{
		m_nodes.push_back(node);
		m_synchLocks[node] = lock;

		if (node->get_input_arguments().size() == 0)
			m_startNodes.push_back(node);
	}

	m_inputBindings[node] = VecBinding();
	m_outputBindings[node] = VecBinding();

	return ok;
}

bool GraphEvaluator::removeNode(PtrNode node)
{
	bool ok = hasNode(node);

	if (ok)
	{
		auto foundIterator = std::find(m_nodes.begin(), m_nodes.end(), node);
		assert(foundIterator != m_nodes.end());

		m_nodes.erase(foundIterator);

		if (node->get_input_arguments().size() == 0)
		{
			auto foundStartNodeIterator = std::find(m_startNodes.begin(), m_startNodes.end(), node);
			assert(foundStartNodeIterator != m_startNodes.end());

			m_startNodes.erase(foundStartNodeIterator);
		}

		m_synchLocks.erase(node);
		m_inputBindings.erase(node);
		m_outputBindings.erase(node);
	}

	return ok;
}

bool GraphEvaluator::hasNode(PtrNode node)
{
	auto foundIterator = std::find(m_nodes.begin(), m_nodes.end(), node);
	return (foundIterator != m_nodes.end());
}

bool GraphEvaluator::addBinding(PtrBinding binding)
{
	assert(binding);

	if (!binding)
		return false;

	auto sourceNode = binding->getSourceNode();
	auto targetNode = binding->getTargetNode();
	assert(sourceNode && targetNode);

	bool ok = sourceNode && targetNode;

	if (!ok)
		return false;

	m_outputBindings[sourceNode].push_back(binding);
	m_inputBindings[targetNode].push_back(binding);

	return true;
}

bool GraphEvaluator::removeBinding(PtrBinding binding)
{
	auto sourceNode = binding->getSourceNode();
	auto targetNode = binding->getTargetNode();
	assert(sourceNode && targetNode);

	bool ok = sourceNode && targetNode;

	if (sourceNode)
	{
		auto iterOutputBindings = std::find(m_outputBindings[sourceNode].begin(), m_outputBindings[sourceNode].end(), binding);
		m_outputBindings[sourceNode].erase(iterOutputBindings);
	}
	
	if (targetNode)
	{
		auto iterInputBindings = std::find(m_inputBindings[targetNode].begin(), m_inputBindings[targetNode].end(), binding);
		m_inputBindings[targetNode].erase(iterInputBindings);
	}

	return ok;
}


bool GraphEvaluator::evaluate()
{
	std::queue<PtrNode>				nextNodesToEvaluate;
	std::unordered_set<PtrNode>		nodesToEvaluate;
	std::unordered_set<PtrBinding>	evaluatedBindings;
	std::unordered_set<PtrNode>		evaluatedNodes;

	for (auto iter : m_startNodes)
		nextNodesToEvaluate.push(iter);

	while (nextNodesToEvaluate.empty() == false)
	{
		auto node = nextNodesToEvaluate.front();
		nextNodesToEvaluate.pop();

		if (evaluatedNodes.find(node) != evaluatedNodes.end())
			continue;

		ItlEvaluateNode(node);

		evaluatedNodes.insert(node);
		nodesToEvaluate.erase(node);
		
		auto outputBindings = m_outputBindings[node];

		// mark all related output bindings for current evaluated node as "reevaluate"
		for (auto iterBinding : outputBindings)
		{
			nodesToEvaluate.insert(iterBinding->getTargetNode());
			evaluatedBindings.insert(iterBinding);
		}	

		for (auto iterNode : nodesToEvaluate)
		{
			bool bAllBindingsEvaluated = true;
			
			for (auto iterInputBinding : m_inputBindings[iterNode])
				bAllBindingsEvaluated &= evaluatedBindings.find(iterInputBinding) != evaluatedBindings.end();

			bool alreadyDone = evaluatedNodes.find(iterNode) != evaluatedNodes.end();

			if (bAllBindingsEvaluated && !alreadyDone)
			{
				assert(evaluatedNodes.find(iterNode) == evaluatedNodes.end());
				nextNodesToEvaluate.push(iterNode);
			}
		}
	}

	return true;
}

bool GraphEvaluator::evaluate_threaded()
{
	for (auto iter : m_startNodes)
		m_nodesToEvaluate.insert(iter);

	while (true)
	{
		std::lock_guard<std::mutex> lock(m_threadedInfoMutex);
		if (m_nodesToEvaluate.empty() && m_evaluatingNodes.empty())
			break;

		for (auto &nextNodeToEvaluate : m_nodesToEvaluate)
		{
			auto nextNodeToEvaluate = *(m_nodesToEvaluate.begin());

			auto lockObject = m_synchLocks[nextNodeToEvaluate];

			if (lockObject && m_activeLocks.find(lockObject) != m_activeLocks.end())
				continue;

			if (lockObject && m_activeLocks.find(lockObject) == m_activeLocks.end())
				m_activeLocks.insert(lockObject);


			std::thread *pNewThread = new std::thread([&]() {ItlThreadedEvaluation(nextNodeToEvaluate);});

			m_evaluatingNodes[nextNodeToEvaluate] = pNewThread;

			m_nodesToEvaluate.erase(nextNodeToEvaluate);

			break;
		}		
	}

	// clean up threads
	for (auto pThread : m_finishedThreads)
	{
		pThread->join();
		delete pThread;
	}
	m_finishedThreads.clear();

	return true;
}

std::shared_ptr<GraphEvaluator::SyncLock> GraphEvaluator::createSyncLock()
{
	static unsigned int nextFreeId = 0;

	return std::shared_ptr<GraphEvaluator::SyncLock>(new unsigned int (nextFreeId++));
}

bool GraphEvaluator::ItlEvaluateNode(PtrNode node)
{
	ConstIValueVector inputValues;
	IValueVector outputValues;

	auto inputBindings = m_inputBindings[node];
	auto outputBindings = m_outputBindings[node];

	auto inputArguments = node->get_input_arguments();
	auto outputArguments = node->get_output_arguments();

	for (auto &inputArgument : inputArguments)
	{
		auto name = inputArgument->get_name();
		PtrBinding bindingForArgument(nullptr);
		for (auto &binding : inputBindings)
		{
			if (strcmp(name, binding->getTargetArgumentName()) == 0)
			{
				bindingForArgument = binding;
				break;
			}
		}
		assert(bindingForArgument);
		
		auto valuePtr = m_valuePool.getValue(bindingForArgument->getSourceNode(), bindingForArgument->getSourceArgumentName(), inputArgument->get_type());

		inputValues.push_back(valuePtr.get());
	}


	for (auto &outputArgument : outputArguments)
	{
		auto name = outputArgument->get_name();
		PtrBinding bindingForArgument(nullptr);
		for (auto &binding : outputBindings)
		{
			if (strcmp(name, binding->getSourceArgumentName()) == 0)
			{
				bindingForArgument = binding;
				break;
			}
		}
		assert(bindingForArgument);

		auto valuePtr = m_valuePool.getValue(node, outputArgument->get_name(), outputArgument->get_type());
		outputValues.push_back(valuePtr.get());
	}

	return node->execute(inputValues, outputValues);
}

void GraphEvaluator::ItlThreadedEvaluation(PtrNode node)
{
	bool done = ItlEvaluateNode(node);
	assert(done);

	{
		std::lock_guard<std::mutex> lock(m_threadedInfoMutex);

		ItlOnNodeEvaluated(node);
		ItlOnThreadFinished(node);
	}
}

void GraphEvaluator::ItlOnThreadFinished(PtrNode node)
{
	assert(m_evaluatingNodes.find(node) != m_evaluatingNodes.end());

	std::thread * pThread = m_evaluatingNodes[node];

	m_finishedThreads.insert(pThread);
	m_evaluatingNodes.erase(node);

	auto syncLock = m_synchLocks[node];
	if (syncLock)
		m_activeLocks.erase(syncLock);
}

void GraphEvaluator::ItlOnNodeEvaluated(PtrNode node)
{
	auto outputBindings = m_outputBindings[node];

	std::set<PtrNode> affectedOutputNodes;

	// mark all related output bindings for current evaluated node as "reevaluate"
	for (auto iterBinding : outputBindings)
	{
		affectedOutputNodes.insert(iterBinding->getTargetNode());
		m_evaluatedBindings.insert(iterBinding);
	}
	
	for (auto iterNode : affectedOutputNodes)
	{
		bool bAllBindingsEvaluated = true;

		for (auto iterInputBinding : m_inputBindings[iterNode])
			bAllBindingsEvaluated &= m_evaluatedBindings.find(iterInputBinding) != m_evaluatedBindings.end();

		bool alreadyDone = m_evaluatedNodes.find(iterNode) != m_evaluatedNodes.end();

		if (bAllBindingsEvaluated && !alreadyDone)
			m_nodesToEvaluate.insert(iterNode);
	}
}
