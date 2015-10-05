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
    // Read the game state and convert it to QList<int> for the second drawing object
    recievedGameBoard.clear();
    QString tmp = socket->readAll();
    QStringList message = tmp.split('\n');
    for(int i = 0; i < message.size(); i++)
    {
        recievedGameBoard.append(message[i].toInt());
    }

    emit RecievedGameState(recievedGameBoard);
}

void TCPClient::bytesWritten(qint64 bytes)
{
    QTextStream cout(stdout);
    cout << bytes << "bytes sent\n";
}

void TCPClient::SendGameState(QList<int> gameState)
{
    // Send our own game state
    QString message;
    for(int i = 0; i < gameState.size(); i++)
    {
        message.append(QString::number(gameState[i]) + "\n");
    }
    socket->write(message.toStdString().c_str());
}
