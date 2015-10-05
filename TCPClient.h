#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QTextStream>

class TCPClient : public QDialog
{
    Q_OBJECT
public:
    explicit TCPClient(QWidget *parent = 0);
    ~TCPClient();

    // Establish connection to TCPServer in localhost
    void EstablishConnection();
signals:
    //emmited when the TCPClient is succesfully connected to TCPServer
    void ConnectedToServer();
    //emmited when the client recieves game state of the other player
    void RecievedGameState(QList<int>);

public slots:
    // If own game state changed send it to the other player in the server
    void SendGameState(QList<int> gameState);

private slots:
    // handles when the socket successfully connects
    void connected();
    // handles when the socket successfully disconnects
    void disconnected();
    // handler for when data is ready to be read
    void readyRead();
    // handler for when you write into the socket
    void bytesWritten(qint64 bytes);

private:
    QTcpSocket* socket;
    QList<int> recievedGameBoard;
};

#endif // TCPCLIENT_H
