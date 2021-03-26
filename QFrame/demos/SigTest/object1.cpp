#include "object1.h"

#include <QThread>
#include <QTimer>
#include <QDebug>

object1::object1(QObject *parent) : QObject(parent)
{
    index_ = 0;
    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    connect(t,&QTimer::timeout,this,[this](){
        Message msg;
        msg.index = ++index_;
        msg.data = new int[1024*1024];
        sigSendMsg(msg);
        qDebug()<<"send thread:"<<QThread::currentThreadId();
    });

    t->start();
    worker = new QThread();
    worker->setObjectName("object1");
    this->moveToThread(worker);
    worker->start();
}

void object1::onRevPointer(iPoint p)
{
    qDebug()<<p[0];
    delete p;//不删除会有内存泄露
    qDebug()<<Q_FUNC_INFO;
}

