#include <QApplication>
#include "wbserverwidget.h"
#include "whiteboardserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WbServerWidget w;
    w.show();

    return a.exec();
}
