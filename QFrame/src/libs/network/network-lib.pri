QT       += network

DEFINES += NETWORK_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

VERSION = 1.0.0

HEADERS += \
    $$PWD/clientsocket.h \
    $$PWD/messagereader.h \
    $$PWD/msghandler.h \
    $$PWD/network_global.h \
    $$PWD/tcpclient.h \
    $$PWD/websocketclient.h

SOURCES += \
    $$PWD/messagereader.cpp \
    $$PWD/msghandler.cpp \
    $$PWD/tcpclient.cpp \
    $$PWD/websocketclient.cpp
