#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "NodeItem.h"

#include "QGraphicsItem"
#include "GraphicalNodeNetwork.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GraphicalNodeNetwork * test(new GraphicalNodeNetwork);

    NodeItem *node1(new NodeItem(*test));
    NodeItem *node2(new NodeItem(*test));
    NodeItem *node3(new NodeItem(*test));

    node1->setPos(0, 0);
    node2->setPos(130, 0);
    node3->setPos(260, 0);

    test->addItem(node1);
    test->addItem(node2);
    test->addItem(node3);

    ui->graphicsView->setScene(test);
}

MainWindow::~MainWindow()
{
    delete ui;
}
