#include <QCoreApplication>

#include "object1.h"
#include "object2.h"

#include <QThread>
#include <QDebug>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug()<<"Main thread:"<<QThread::currentThreadId();

    object1 *o1 = new object1;
    object2 *o2 = new object2;
    qRegisterMetaType<Message>("Message");//注册
    qRegisterMetaType<iPoint>("iPoint");//注册
    QObject::connect(o1,&object1::sigSendMsg,o2,&object2::onRevMsg);
    QObject::connect(o2,&object2::sigSendPointer,o1,&object1::onRevPointer);

    return a.exec();
}
