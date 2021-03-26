#ifndef OBJECT1_H
#define OBJECT1_H

#include <QObject>
#include "define.h"

class object1 : public QObject
{
    Q_OBJECT
public:
    explicit object1(QObject *parent = nullptr);
signals:
    void sigSendMsg(const Message&);
public slots:
    void onRevPointer(iPoint);
private:
    class QThread *worker;
    int index_;
};

#endif // OBJECT1_H
