#include <QApplication>
#include "network/clientsocket.h"

NETWORK_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    ClientSocket client;


    return a.exec();
}
