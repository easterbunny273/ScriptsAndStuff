#include <cassert>
#include <algorithm>
#include <exception>

#include "NodeNetwork.h"
#include "Node.h"

void NodeNetwork::addNode(std::shared_ptr<Node> node)
{
	bool alreadyRegistered = (std::find(m_nodes.begin(), m_nodes.end(), node) != m_nodes.end());
	assert(!alreadyRegistered);

	if (alreadyRegistered)
		throw Exception(Exception::Type::NODE_ALREADY_EXISTS);

	m_nodes.push_back(node);
	m_listenerNotifier.onNodeAdded(node);
}

void NodeNetwork::removeNode(std::shared_ptr<Node> node)
{
	auto iter = std::find(m_nodes.begin(), m_nodes.end(), node);

	if (iter == m_nodes.end())
		throw Exception(Exception::Type::NODE_NOT_FOUND);

	auto connections = getConnections(node);

	for (auto connection : connections)
		removeConnection(connection);

	m_nodes.erase(iter);
	m_listenerNotifier.onNodeRemoved(node);
}

bool NodeNetwork::hasNode(std::shared_ptr<Node> node)
{
	auto iter = std::find(m_nodes.begin(), m_nodes.end(), node);

	return (iter != m_nodes.end());
}

std::vector<std::shared_ptr<Node>> NodeNetwork::getNodes()
{
	return m_nodes;
}

void NodeNetwork::addConnection(TConnection connection)
{
	bool alreadyRegistered = std::find(m_connections.begin(), m_connections.end(), connection) != m_connections.end();

	if (alreadyRegistered)
		throw Exception(Exception::Type::CONNECTION_ALREADY_EXISTS);

	if (!hasNode(connection.inNode) || !hasNode(connection.outNode))
		throw Exception(Exception::Type::NODE_NOT_FOUND);

	if (ItlInputParamConnected(connection.inNode, connection.inParam))
		throw Exception(Exception::Type::CONNECTION_INPUT_PARAM_ALREADY_CONNECTED);

	if (!ItlNodeParamsMatch(connection))
		throw Exception(Exception::Type::CONNECTION_PARAMS_DO_NOT_MATCH);

	m_connections.push_back(connection);
	m_listenerNotifier.onConnectionAdded(connection);
}

void NodeNetwork::removeConnection(TConnection connection)
{
	auto iter = std::find(m_connections.begin(), m_connections.end(), connection);

	if (iter == m_connections.end())
		throw Exception(Exception::Type::CONNECTION_NOT_FOUND);

	m_connections.erase(iter);
	m_listenerNotifier.onConnectionRemoved(connection);
}

void NodeNetwork::removeConnection(std::shared_ptr<Node> inNode, std::string inParam)
{
	auto iter = std::find_if(m_connections.begin(), m_connections.end(), [&](auto &connection) { return (connection.inNode == inNode && connection.inParam == inParam);});

	if (iter == m_connections.end())
		throw Exception(Exception::Type::CONNECTION_NOT_FOUND);

	removeConnection(*iter);
}

std::vector<NodeNetwork::TConnection> NodeNetwork::getConnections()
{
	return m_connections;
}

std::vector<NodeNetwork::TConnection> NodeNetwork::getConnections(std::shared_ptr<Node> node)
{
	std::vector<TConnection> resultVector;

	for (auto connection : m_connections)
	{
		if (connection.inNode == node || connection.outNode == node)
			resultVector.push_back(connection);
	}

	return resultVector;
}

void NodeNetwork::registerListener(IListener * listener)
{
	m_listenerNotifier.registerListener(listener);
}

void NodeNetwork::unregisterListener(IListener * listener)
{
	m_listenerNotifier.unregisterListener(listener);
}

bool NodeNetwork::ItlNodeParamsMatch(TConnection connection)
{
	auto inParams = connection.inNode->getInputParamDescription();
	auto outParams = connection.outNode->getOuputParamDescription();

	auto inParam = inParams.find(connection.inParam);
	auto outParam = outParams.find(connection.outParam);

	if (inParam == inParams.end() || outParam == outParams.end())
		return false;

	return (inParam->second.type == outParam->second.type);
}

bool NodeNetwork::ItlInputParamConnected(std::shared_ptr<Node> node, Node::TParamName inputParam)
{
	auto iter = std::find_if(m_connections.begin(), m_connections.end(), [&](auto &connection) { return (connection.inNode == node && connection.inParam == inputParam);});

	return (iter != m_connections.end());
}

void NodeNetwork::ListenerNotifier::registerListener(IListener * listener)
{
	assert(std::find(m_listeners.begin(), m_listeners.end(), listener) == m_listeners.end());
	m_listeners.push_back(listener);
}

void NodeNetwork::ListenerNotifier::unregisterListener(IListener * listener)
{
	auto iter = std::find(m_listeners.begin(), m_listeners.end(), listener);
	assert(iter != m_listeners.end());

	m_listeners.erase(iter);
}

void NodeNetwork::ListenerNotifier::onNodeAdded(std::shared_ptr<Node> node)
{
	for (auto& listener : m_listeners)
		listener->onNodeAdded(node);
}

void NodeNetwork::ListenerNotifier::onNodeRemoved(std::shared_ptr<Node> node)
{
	for (auto& listener : m_listeners)
		listener->onNodeRemoved(node);
}

void NodeNetwork::ListenerNotifier::onConnectionAdded(TConnection connection)
{
	for (auto& listener : m_listeners)
		listener->onConnectionAdded(connection);
}

void NodeNetwork::ListenerNotifier::onConnectionRemoved(TConnection connection)
{
	for (auto& listener : m_listeners)
		listener->onConnectionRemoved(connection);
}

const char * NodeNetwork::Exception::StaticTranslateMessage(Type type)
{
	switch (type)
	{
	case Type::NODE_ALREADY_EXISTS: return "Node already exists";
	case Type::NODE_NOT_FOUND: return "Node not found";
	case Type::CONNECTION_ALREADY_EXISTS: return "Connection already exists";
	case Type::CONNECTION_NOT_FOUND: return "Connection not found";
	case Type::CONNECTION_PARAMS_DO_NOT_MATCH: return "Wrong parameter names/types for connection, they do not match with nodes";
	case Type::CONNECTION_INPUT_PARAM_ALREADY_CONNECTED: return "The input parameter is already connected";
	default: break;
	};

	assert(!"should not come here");
	return "unknown error";
}
