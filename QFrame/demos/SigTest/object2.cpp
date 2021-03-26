#include "object2.h"

#include <QThread>
#include <QTimer>
#include <QDebug>

object2::object2(QObject *parent) : QObject(parent)
{
    index_ = 0;
    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    connect(t,&QTimer::timeout,this,[this](){
        iPoint p = new int [1024*1024];
        p[0] = ++index_;
        sigSendPointer(p);
    });
    t->start();
    worker = new QThread();
    worker->setObjectName("object2");
    this->moveToThread(worker);
    worker->start();
}

void object2::onRevMsg(const Message & msg)
{
    qDebug()<<"rev:"<<msg.index;
    delete msg.data;//不删除会有内存泄露
    qDebug()<<"rev thread:"<<QThread::currentThreadId();
}
