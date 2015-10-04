#include "TCPServer.h"

TCPServer::TCPServer(QWidget *parent) : QDialog(parent)
{
    server = new QTcpServer(this);
    socket = NULL;
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
    //QTest::qSleep(1000);
    recievedGameBoard.clear();
    QString tmp = socket->readAll();

    QStringList message = tmp.split('\n');
    for(int i = 0; i < message.size(); i++)
    {
        recievedGameBoard.append(message[i].toInt());
    }

    emit RecievedGameState(recievedGameBoard);
}

void TCPServer::SendGameState(QList<int> gameState)
{
    if(socket != NULL)
    {
        QString message;
        for(int i = 0; i < gameState.size(); i++)
        {
            message.append(QString::number(gameState[i]) + "\n");
        }

        socket->write(message.toStdString().c_str());
    }
}

