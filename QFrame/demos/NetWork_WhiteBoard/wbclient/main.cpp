#include "mainwindow.h"
#include <QApplication>
#include"painterscene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(600,480);

    QString Application = QApplication::applicationDirPath();
    Application = ":/bing.ico";
    w.setWindowIcon(QIcon(Application));
    w.setWindowTitle("网络电子白板");
    w.show();

    return a.exec();
}
