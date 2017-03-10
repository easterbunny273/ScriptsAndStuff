#include "NodeItem.h"

NodeItem::NodeItem(GraphicalNodeNetwork &parent)
    : m_parent(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    const int leftPosX = - ParameterItem::WIDTH + 2;
    const int rightPosX = WIDTH - ParameterItem::WIDTH - leftPosX;

    const int leftPosY = 40;
    const int marginY = 10;

    std::vector<int> params = {1, 2, 3};
    int indexIn = 0;
    for (auto param : params)
    {
        QSharedPointer<ParameterItem> newItem(new ParameterItem(this));
        newItem->setPos(leftPosX, leftPosY + indexIn*(ParameterItem::HEIGHT + marginY));

        m_ParamItems_In.push_back(newItem);
        indexIn++;
    }

    std::vector<int> paramsOut = {1, 2, 3};
    int indexOut = 0;
    for (auto param : paramsOut)
    {
        QSharedPointer<ParameterItem> newItem(new ParameterItem(this));
        newItem->setPos(rightPosX, leftPosY + indexOut*(ParameterItem::HEIGHT + marginY));

        m_ParamItems_Out.push_back(newItem);
        indexOut++;
    }
}
