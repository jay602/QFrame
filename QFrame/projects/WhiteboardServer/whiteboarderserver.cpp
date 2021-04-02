#include "whiteboardserver.h"

WhiteboardServer::WhiteboardServer(QObject *parent)
    : QTcpServer(parent)
{

}

WhiteboardServer::~WhiteboardServer()
{

}

void WhiteboardServer::incomingConnection(qintptr handle)
{
    ClientConnection *conn = new ClientConnection(this);
    conn->setSocketDescriptor(handle);
    m_clients.push_back(conn);

    qDebug() << __FUNCTION__ << " : " << conn->info();

}
