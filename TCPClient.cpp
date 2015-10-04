#include "TCPClient.h"

TCPClient::TCPClient(QWidget* parent) : QDialog(parent)
{
    socket = new QTcpSocket(this);
    connect();
}

void TCPClient::connect()
{
    socket->connectToHost("localhost", 8888);

    if(socket->waitForConnected(3000))
    {
        QTextStream cout(stdout);
        cout << "Client connected\n";

        socket->write("Hello Server \n\r");

        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);

        cout << socket->readAll() << "\n" ;
    }
    else
    {
        QTextStream cout(stdout);
        cout << "Client could not connect\n";
    }
}
