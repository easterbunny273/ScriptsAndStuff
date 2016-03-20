#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pythonwrapper_qtpart.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto output = PythonWrapper_QtPart::Expose(&m_pythonWrapper, &m_testObject, "testObject");
    ui->plainTextEdit->appendPlainText(QString(output.c_str()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    auto currentLine = ui->lineEdit->text();
    ui->lineEdit->clear();

    ui->plainTextEdit->appendPlainText(">" + currentLine);

    auto output = m_pythonWrapper.execute(currentLine.toStdString());
    ui->plainTextEdit->appendPlainText(QString(output.c_str()));
}
