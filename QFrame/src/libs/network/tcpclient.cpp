#include "tcpclient.h"
#include "msghandler.h"
#include <QDebug>


NETWORK_BEGIN_NAMESPACE


TcpClient::TcpClient()
{
    m_tcpSocket = new QTcpSocket(this);
    //QObject::connect(m_tcpSocket, &QTcpSocket::connected, this, &TcpClient::connected);
   // QObject::connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpClient::readyread);
}

TcpClient::~TcpClient()
{

}

void TcpClient::connect(QString strHost, int port)
{
    if(m_tcpSocket)
    {
        m_tcpSocket->connectToHost(strHost, port);
        m_tcpSocket->waitForConnected();
    }
}

void TcpClient::connected()
{
    qDebug() << "conect ok";
    //m_handler->handleOnConnected();
}

void TcpClient::readyread()
{
    QByteArray buffer;
        //读取缓冲区数据
    buffer = m_tcpSocket->readAll();
    if(!buffer.isEmpty())
    {

    }

}

NETWORK_END_NAMESPACE
