#pragma once

#include <vector>
#include <memory>

#include "Node.h"

class NodeNetwork
{
public:
	class Exception : public std::runtime_error
	{
	public:
		enum class Type
		{
			NODE_ALREADY_EXISTS,
			NODE_NOT_FOUND,

			CONNECTION_ALREADY_EXISTS,
			CONNECTION_NOT_FOUND,
			CONNECTION_PARAMS_DO_NOT_MATCH,
			CONNECTION_INPUT_PARAM_ALREADY_CONNECTED,
		};

		Exception(Type type) : std::runtime_error(StaticTranslateMessage(type)) {};

	private:
		static const char * StaticTranslateMessage(Type type);
	};

	struct TConnection
	{
		std::shared_ptr<Node> outNode, inNode;
		Node::TParamName outParam, inParam;

		bool operator==(const TConnection &other) { return outNode == other.outNode && inNode == other.inNode && outParam == other.outParam && inParam == other.inParam; };

		TConnection(std::shared_ptr<Node> _outNode, std::string _outParam, std::shared_ptr<Node> _inNode, std::string _inParam) : outNode(_outNode), inNode(_inNode), outParam(_outParam), inParam(_inParam) {};
	};

	class IListener
	{
	public:
		virtual void onNodeAdded(std::shared_ptr<Node> node) = 0;
		virtual void onNodeRemoved(std::shared_ptr<Node> node) = 0;

		virtual void onConnectionAdded(TConnection connection) = 0;
		virtual void onConnectionRemoved(TConnection connection) = 0;
	};

	void addNode(std::shared_ptr<Node> node);
	void removeNode(std::shared_ptr<Node> node);
	bool hasNode(std::shared_ptr<Node> node);

	std::vector<std::shared_ptr<Node>> getNodes();

	void addConnection(TConnection connection);
	void removeConnection(TConnection connection);
	void removeConnection(std::shared_ptr<Node> inNode, std::string inParam);

	std::vector<TConnection> getConnections();
	std::vector<TConnection> getConnections(std::shared_ptr<Node> node);

	void registerListener(IListener * listener);
	void unregisterListener(IListener * listener);

private:
	class ListenerNotifier : public IListener
	{
	public:
		void registerListener(IListener * listener);
		void unregisterListener(IListener * listener);

		virtual void onNodeAdded(std::shared_ptr<Node> node) override;
		virtual void onNodeRemoved(std::shared_ptr<Node> node) override;

		virtual void onConnectionAdded(TConnection connection) override;
		virtual void onConnectionRemoved(TConnection connection) override;

	private:
		std::vector<IListener*>		m_listeners;
	};

	bool ItlNodeParamsMatch(TConnection connection);
	bool ItlInputParamConnected(std::shared_ptr<Node> node, Node::TParamName inputParam);

	std::vector<std::shared_ptr<Node>>	m_nodes;
	std::vector<TConnection>			m_connections;
	
	ListenerNotifier					m_listenerNotifier;
	
};