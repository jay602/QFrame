#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    m_tcpClient = new QTcpSocket(this);
}

void TcpClient::connect(QString ip, int port)
{
    m_tcpClient->connectToHost(ip, port);
}
