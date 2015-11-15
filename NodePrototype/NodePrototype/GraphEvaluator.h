#pragma once

#ifndef __GRAPHEVALUATOR_H_
#define __GRAPHEVALUATOR_H_

#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <unordered_set>

#include "INode.h"
#include "Bindings.h"
#include "ValuePool.h"

class Binding;
class INode;

class GraphEvaluator
{
public:
	using SyncLock = unsigned int;

	bool addNode(PtrNode node, std::shared_ptr<SyncLock> lock = nullptr);
	bool removeNode(PtrNode node);
	bool hasNode(PtrNode node);

	bool addBinding(PtrBinding binding);
	bool removeBinding(PtrBinding binding);

	bool evaluate();
	bool evaluate_threaded();

	std::shared_ptr<SyncLock> createSyncLock();


private:
	bool ItlEvaluateNode(PtrNode node);

	using VecBinding = std::vector<PtrBinding>;

	std::map<PtrNode, VecBinding> m_inputBindings;
	std::map<PtrNode, VecBinding> m_outputBindings;

	std::vector<PtrNode> m_nodes;
	std::vector<PtrNode> m_startNodes;

	void ItlThreadedEvaluation(PtrNode node);
	void ItlOnThreadFinished(PtrNode node);
	void ItlOnNodeEvaluated(PtrNode node);

	std::unordered_set<std::thread *> m_finishedThreads;
	std::unordered_map<PtrNode, std::thread *> m_evaluatingNodes;
	std::unordered_map<PtrNode, std::shared_ptr<SyncLock>> m_synchLocks;
	std::unordered_set<std::shared_ptr<SyncLock>> m_activeLocks;

	std::unordered_set<PtrBinding>		m_evaluatedBindings;
	std::unordered_set<PtrNode>			m_evaluatedNodes;
	std::unordered_set<PtrNode>			m_nodesToEvaluate;
	std::mutex					m_threadedInfoMutex;

	ValuePool					m_valuePool;
};

#endif
