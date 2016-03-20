#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "exposedclasstest1.h"

#include "pythonwrapper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    PythonWrapper m_pythonWrapper;
    ExposedClassTest1 m_testObject;
};

#endif // MAINWINDOW_H
