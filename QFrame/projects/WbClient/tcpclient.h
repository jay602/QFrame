#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include "Singleton.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

    void  connect(QString ip, int port);
signals:

private:
    QTcpSocket *m_tcpClient;
};

typedef Singleton< TcpClient > ClientApp;

#endif // TCPCLIENT_H
