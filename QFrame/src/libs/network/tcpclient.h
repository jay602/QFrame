#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "clientsocket.h"
#include <QTcpSocket>

NETWORK_BEGIN_NAMESPACE


class NETWORK_SHARED_EXPORT TcpClient : public ClientSocket
{
    Q_OBJECT
public:
    TcpClient();
    ~TcpClient();

    void connect(QString strHost, int port);

private slots:
    void onConnected();
    void onReadyReaded();
    void onDisConnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket * m_tcpSocket;
};

NETWORK_END_NAMESPACE

#endif // TCPCLIENT_H
