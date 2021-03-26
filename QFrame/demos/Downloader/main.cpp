#include <QTextCodec>
#include <QCoreApplication>

#include <QUrl>
#include "Http/httpdownloadtask.h"
#include "Http/networkerror.h"

void  initCodec()//for Qt 4
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
  // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
#endif
}

QString qUrl = QStringLiteral("http://m.weiguanli.cn/setup/微管理-setup-v1.0.exe");
QString url = "http://download.qt.io/linguist_releases/qtlinguistinstaller-5.12.2.exe";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    initCodec();
    qRegisterMetaType<NetworkError>("NetworkError");

    HttpDownloadTask task(QUrl(url), QString("D:\\"), NULL);
    task.start();
    task.getFileInfoSize(url, 3);

    return a.exec();
}
