#ifndef OBJECT2_H
#define OBJECT2_H

#include <QObject>
#include "define.h"
class object2 : public QObject
{
    Q_OBJECT
public:
    explicit object2(QObject *parent = nullptr);
public slots:
    void onRevMsg(const Message&);
signals:
    void sigSendPointer(iPoint);
private:
    class QThread *worker;
    int index_;
};

#endif // OBJECT2_H
