#include "mainwindow.h"
#include "NodeNetwork.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    NodeNetwork testNetwork;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
