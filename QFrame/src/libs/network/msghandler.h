#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "network_global.h"
#include <QByteArray>

NETWORK_BEGIN_NAMESPACE

class NETWORK_SHARED_EXPORT MsgHandler
{
public:
    MsgHandler();
    virtual void handleMsg(QByteArray msg, int len);
    virtual void handleOnConnected();
    virtual void handleOnDisConnected();
    virtual void handleOnRepeatonnected();
    virtual void handleError();
};

NETWORK_END_NAMESPACE

#endif // MSGHANDLER_H
