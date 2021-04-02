#include "clientconnection.h"
#include <QHostAddress>

ClientConnection::ClientConnection(QObject *parent)
    : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater));
}

ClientConnection::~ClientConnection()
{
    qDebug() << __FUNCTION__;
}

QString ClientConnection::info()
{
    QString strInfo = QString("%1 : %2").arg(peerAddress().toString())
            .arg(peerPort());

    return strInfo;
}

void ClientConnection::onReadyRead()
{
    QByteArray data = readAll();
    data.append('\n');
    write(data);
}
