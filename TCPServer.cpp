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
    QTcpSocket* socket = server->nextPendingConnection();

    socket->waitForReadyRead(3000);
    QTextStream cout(stdout);
    cout << socket->readAll() <<"\n";

    socket->write("Hello client\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);
//    socket->close();
}

void TCPServer::setupServer()
{

}

