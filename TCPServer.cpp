#include "TCPServer.h"

TCPServer::TCPServer(QWidget *parent) : QDialog(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &TCPServer::newConnection);

    if(!server->listen(QHostAddress::LocalHost, 8888))
    {
        QTextStream cout(stdout);
        cout << "Server could not start!\n";
    }
    else
    {
        QTextStream cout(stdout);
        cout << "Server started\n";
    }
}

TCPServer::~TCPServer()
{

}

void TCPServer::newConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &TCPServer::readyReadHandler);

    emit ClientConnected();
}

void TCPServer::readyReadHandler()
{
    QTextStream cout(stdout);
    cout << socket->readAll() <<"\n";
}

