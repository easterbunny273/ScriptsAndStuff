#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void on_pushButton_clicked();
    void on_tcpSocket_onError(QAbstractSocket::SocketError socketError);
    void on_tcpSocket_readyRead();
    void on_tcpSocket_stateChanged();

private:
    void addLogMessage(QString message);

    QScopedPointer<QTcpSocket> m_clientSocket;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
