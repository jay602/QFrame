#include <QApplication>
#include "network/tcpclient.h"

NETWORK_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    TcpClient client;
    client.connect("127.0.0.1", 6000);

    return a.exec();
}
