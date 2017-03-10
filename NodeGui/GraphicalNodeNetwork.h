#ifndef GRAPHICALNODENETWORK_H
#define GRAPHICALNODENETWORK_H

#include "QGraphicsScene"

class NodeItem;
class ConnectionItem;

class GraphicalNodeNetwork : public QGraphicsScene
{
public:
    GraphicalNodeNetwork();

    void startConnection(NodeItem &startNode);
    void endConnection(NodeItem &stopNode);
    void abortConnection();

    NodeItem * connectionGetStartNode();

    void addConnection(NodeItem &startNode, NodeItem &endNode);

private:
    //QVector<QScopedPointer<NodeItem>>       m_nodes;
    //QVector<QScopedPointer<ConnectionItem>> m_connections;

    NodeItem                                * m_tempConnection_Start;
};

#endif // GRAPHICALNODENETWORK_H
