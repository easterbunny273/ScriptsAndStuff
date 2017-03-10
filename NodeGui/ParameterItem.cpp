#include "ParameterItem.h"

#include "QLineEdit"
#include "QPushButton"
#include "QGraphicsProxyWidget"

ParameterItem::ParameterItem(QGraphicsItem * parent) : QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);


    //connect(this, SIGNAL())

    /*QPushButton * testEdit = new QPushButton("a");
    testEdit->setMinimumWidth(20);
    testEdit->setMinimumHeight(20);
    testEdit->setMaximumHeight(20);
    testEdit->setMaximumWidth(20);

    QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
    pMyProxy->setWidget(testEdit);

    pMyProxy->setPos(10, 0);*/
}
