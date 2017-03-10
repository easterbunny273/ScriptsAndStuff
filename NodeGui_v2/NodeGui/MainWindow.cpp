#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_clientSocket(new QTcpSocket(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(m_clientSocket.data(), static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &MainWindow::on_tcpSocket_onError);
    connect(m_clientSocket.data(), &QTcpSocket::readyRead, this, &MainWindow::on_tcpSocket_readyRead);
    connect(m_clientSocket.data(), &QTcpSocket::stateChanged, this, &MainWindow::on_tcpSocket_stateChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addLogMessage(QString message)
{
    auto oldText = ui->logWidget->toPlainText();
    oldText.append(message);
    oldText.append("\n");

    ui->logWidget->setPlainText(oldText);
}

void MainWindow::on_pushButton_clicked()
{
    if (m_clientSocket->isOpen())
    {
        m_clientSocket->disconnectFromHost();

        addLogMessage("disconnected from host");

        m_clientSocket->abort();
        m_clientSocket->close();

        on_tcpSocket_stateChanged();

        return;
    }


    m_clientSocket->abort();

    auto host = ui->host->text();
    auto port = ui->port->value();

    addLogMessage(tr("trying to connect to %1:%2").arg(host).arg(port));
    m_clientSocket->connectToHost(host, port);
}

void MainWindow::on_tcpSocket_onError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
       case QAbstractSocket::RemoteHostClosedError:
           break;
       case QAbstractSocket::HostNotFoundError:
           addLogMessage("The host was not found. Please check the host name and port settings.");
           break;
       case QAbstractSocket::ConnectionRefusedError:
           addLogMessage("The connection was refused by the peer. ");
           break;
       default:
            addLogMessage(tr("The following error occurred: %1.").arg(m_clientSocket->errorString()));
    }

    m_clientSocket->close();
}

void MainWindow::on_tcpSocket_readyRead()
{
    auto receivedData = m_clientSocket->readAll();
    addLogMessage("got#" + receivedData);

    if (receivedData.startsWith("WELCOME\n"))
        m_clientSocket->write("GET\n");
}

void MainWindow::on_tcpSocket_stateChanged()
{
    if (m_clientSocket->isOpen())
        ui->pushButton->setText("Disconnect");
    else
        ui->pushButton->setText("Connect");
}
