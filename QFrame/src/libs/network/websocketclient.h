#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include "clientsocket.h"

NETWORK_BEGIN_NAMESPACE

class NETWORK_SHARED_EXPORT WebSocketClient : public ClientSocket
{
    Q_OBJECT
public:
    WebSocketClient();
    ~WebSocketClient();
};

NETWORK_END_NAMESPACE
#endif // WEBSOCKETCLIENT_H
