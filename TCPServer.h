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

public slots:
    void newConnection();

private:
    QTcpServer* server;

    void setupServer();
};

#endif // TCPSERVER_H
