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
    // Signals is emmited when a client is connected
    void ClientConnected();
    void RecievedGameState(QList<int>);

public slots:
    void SendGameState(QList<int> gameState);

private slots:
    void newConnection();
    void readyReadHandler();

private:
    QTcpServer* server;
    QTcpSocket* socket;

    QList<int> recievedGameBoard;
};

#endif // TCPSERVER_H
