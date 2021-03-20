#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "network_global.h"

NETWORK_BEGIN_NAMESPACE

class NETWORK_SHARED_EXPORT MsgHandler
{
public:
    MsgHandler();
    virtual void handleMsg();
    virtual void handleOnConnected();
    virtual void handleOnDisConnected();
    virtual void handleOnRepeatonnected();
};

NETWORK_END_NAMESPACE

#endif // MSGHANDLER_H
