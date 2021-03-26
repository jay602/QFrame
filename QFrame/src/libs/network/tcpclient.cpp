#include "tcpclient.h"
#include "msghandler.h"
#include <QDebug>


NETWORK_BEGIN_NAMESPACE


TcpClient::TcpClient()
{
    m_tcpSocket = new QTcpSocket(this);
    QObject::connect(m_tcpSocket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    QObject::connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpClient::onReadyReaded);
    QObject::connect(m_tcpSocket, &QTcpSocket::disconnected, this, &TcpClient::onDisConnected);
    QObject::connect(m_tcpSocket, SIGNAL(QAbstractSocket::errorOccurred(QAbstractSocket::SocketError)),\
                     this, SLOT(onErrorOccurred(QAbstractSocket::SocketError)));
}

TcpClient::~TcpClient()
{

}

void TcpClient::connect(QString strHost, int port)
{
    m_tcpSocket->connectToHost(strHost, port);
    m_tcpSocket->waitForConnected();
}

void TcpClient::onConnected()
{
    qDebug() << "conect ok";
    m_handler->handleOnConnected();
}

void TcpClient::onDisConnected()
{
    qDebug() << "onDisConnected";
    m_handler->handleOnConnected();
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "onErrorOccurred " << socketError;
    m_handler->handleError();
}

void TcpClient::onReadyReaded()
{
    QByteArray buffer;
        //读取缓冲区数据
    buffer = m_tcpSocket->readAll();
    if(!buffer.isEmpty())
    {
        m_handler->handleMsg(buffer, buffer.length());
    }

}

NETWORK_END_NAMESPACE
