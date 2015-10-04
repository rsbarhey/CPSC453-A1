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

signals:

public slots:

private:
    void connect();
    QTcpSocket* socket;
};

#endif // TCPCLIENT_H
