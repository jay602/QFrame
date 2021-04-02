#include <QCoreApplication>
#include "whiteboardserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WhiteboardServer server;
    server.listen(QHostAddress::Any, 9001);

    return a.exec();
}
