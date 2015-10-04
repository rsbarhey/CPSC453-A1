#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>



class TCPServer : public QDialog
{
    Q_OBJECT
public:
    explicit TCPServer(QWidget *parent = 0);
    ~TCPServer();

signals:
    void ClientConnected();

public slots:
    void newConnection();

private slots:
    void readyReadHandler();

private:
    QTcpServer* server;
    QTcpSocket* socket;
};

#endif // TCPSERVER_H
