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

    void EstablishConnection();
signals:
    void ConnectedToServer();
    void RecievedGameState(QList<int>);

public slots:
    void SendGameState(QList<int> gameState);

private slots:
    void connected();
    void disconnected();
    void readyRead();
    void bytesWritten(qint64 bytes);

private:
    QTcpSocket* socket;
    QList<int> recievedGameBoard;
};

#endif // TCPCLIENT_H
