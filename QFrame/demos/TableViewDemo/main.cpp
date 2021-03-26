#include "stockwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StockWidget w;
    w.show();
    return a.exec();
}
