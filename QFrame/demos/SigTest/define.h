#ifndef DEFINE_H
#define DEFINE_H

#include <QMetaType>

struct Message{
    int index;
    int *data;
};
typedef int* iPoint;
Q_DECLARE_METATYPE(Message)

#endif // DEFINE_H
