#include "TCPClient.h"

TCPClient::TCPClient(QWidget* parent) : QDialog(parent)
{
    socket = new QTcpSocket(this);
}

TCPClient::~TCPClient()
{
}

void TCPClient::EstablishConnection()
{
    connect(socket, &QTcpSocket::connected, this, &TCPClient::connected);
    connect(socket, &QTcpSocket::disconnected, this, &TCPClient::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &TCPClient::readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &TCPClient::bytesWritten);

    socket->connectToHost("localhost", 8888);

    if(socket->waitForConnected(1000))
    {

    }
    else
    {
        QTextStream cout(stdout);
        cout << "Client could not connect:\n" << socket->errorString() << "\n";
    }
}

void TCPClient::connected()
{
    emit ConnectedToServer();
    QTextStream cout(stdout);
    cout << "Client connected\n";
}

void TCPClient::disconnected()
{
    QTextStream cout(stdout);
    cout << "Client disconnected\n";
}

void TCPClient::readyRead()
{
    QTextStream cout(stdout);
    cout << socket->readAll() << "\n" ;
}

void TCPClient::bytesWritten(qint64 bytes)
{

}

void TCPClient::SendMessage(QString msg)
{
    Q_UNUSED(msg);
}
