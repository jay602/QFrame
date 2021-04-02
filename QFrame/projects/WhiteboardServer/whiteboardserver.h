#ifndef WHITEBOARDSERVER_H
#define WHITEBOARDSERVER_H

#include <QTcpServer>
#include <list>
#include "clientconnection.h"

class WhiteboardServer : public QTcpServer
{
    Q_OBJECT
public:
    WhiteboardServer(QObject *parent = 0);
    ~WhiteboardServer();

protected:
    void incomingConnection(qintptr handle) override;

protected:
    std::list<ClientConnection*> m_clients;
};


#endif // WHITEBOARDSERVER_H
