#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "network_global.h"

NETWORK_BEGIN_NAMESPACE

#include <QObject>
class MsgHandler;

class NETWORK_SHARED_EXPORT ClientSocket : public QObject
{
public:
    ClientSocket(QObject *parent = nullptr) {}
    virtual ~ClientSocket() {}

public:
    virtual void connect(QString strHost, int port) {}
    void setHandler(MsgHandler *handler) { m_handler = handler; }

 protected:
    MsgHandler* m_handler;
};

NETWORK_END_NAMESPACE

#endif // CLIENTSOCKET_H
