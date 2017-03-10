#ifndef PARAMETERITEM_H
#define PARAMETERITEM_H

#include "QGraphicsItem"
#include "QPainter"

#include <iostream>


class ParameterItem : public QGraphicsItem
{
public:
    ParameterItem(QGraphicsItem * parent);

    static const int WIDTH = 10;
    static const int HEIGHT = 10;

    QRectF boundingRect() const
    {
        qreal penWidth = 1;
        return QRectF(- penWidth / 2, - penWidth / 2,
                      WIDTH + penWidth, HEIGHT + penWidth);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
    {
        if (isSelected())
        {
            painter->setBrush(QBrush(Qt::gray));
        }
        else
            painter->setBrush(QBrush(Qt::white));

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawRect(0, 0, WIDTH, HEIGHT);
        //painter->drawEllipse(0, 0, WIDTH, HEIGHT);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsItem::mousePressEvent(event);

        std::cout << "hello!" << std::endl;
    }
};

#endif // PARAMETERITEM_H
