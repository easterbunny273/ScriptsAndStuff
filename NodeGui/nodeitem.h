#ifndef NODEITEM_H
#define NODEITEM_H

#include "QGraphicsItem"
#include "QPainter"

#include "ParameterItem.h"
#include "GraphicalNodeNetwork.h"

class NodeItem : public QGraphicsItem
{
public:
    NodeItem(GraphicalNodeNetwork &parent);

    const int X = 0;
    const int Y = 0;
    const int WIDTH = 80;
    const int HEIGHT = 180;
    const int HEIGHT_TITLE = 17;

    const int PADDING_CONTENT_TEXT = 2;

    const int WIDTH_PARAM_HEADER = 30;
    const int HEIGHT_PARAM_HEADER = 10;

    QRectF boundingRect() const
    {
        qreal penWidth = 1;
        return QRectF(X - penWidth / 2, Y - penWidth / 2,
                      WIDTH + penWidth, HEIGHT + penWidth);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
    {
        painter->setBrush(QBrush(Qt::white));
        painter->drawRect(X, Y, WIDTH, HEIGHT);

        if (isSelected())
            painter->setBrush(QBrush(Qt::gray));

        painter->drawRect(X, Y, WIDTH, HEIGHT_TITLE);
        painter->drawText(QRectF(qreal(X), qreal(Y), qreal(WIDTH), qreal(HEIGHT_TITLE)), "RenderPass", QTextOption(Qt::AlignCenter | Qt::AlignVCenter));

        painter->drawText(QRectF(qreal(X), qreal(Y + HEIGHT_TITLE + PADDING_CONTENT_TEXT), qreal(WIDTH_PARAM_HEADER), qreal(HEIGHT_PARAM_HEADER)), "IN", QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
        painter->drawText(QRectF(qreal(X + WIDTH - WIDTH_PARAM_HEADER), qreal(Y + HEIGHT_TITLE + PADDING_CONTENT_TEXT), qreal(WIDTH_PARAM_HEADER), qreal(HEIGHT_PARAM_HEADER)), "OUT", QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    }

private:
    GraphicalNodeNetwork& m_parent;

    QVector<QSharedPointer<ParameterItem>> m_ParamItems_In, m_ParamItems_Out;
};

#endif // NODEITEM_H
