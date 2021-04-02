#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <QMutexLocker>
#include <QScopedPointer>

template <class TypeImp>
class Singleton
{
public:
    static TypeImp& getRef()
    {
        return *(getInstance());
    }

protected:
    Singleton() {}

private:
    static TypeImp* getInstance()
    {
        static QMutex mutex;
        static QScopedPointer<TypeImp> pInstance(NULL);
        static bool instantiated(false);

        if (!instantiated)
        {
            if(NULL == pInstance.get())
            {
                QMutexLocker locker(&mutex);
                if (NULL == pInstance.get())
                {
                    pInstance.reset(new TypeImp());
                    instantiated = true;
                }
            }
        }

        return pInstance.data();
    }
};


#define DECLARE_SINGLETON(Class) \
Q_DISABLE_COPY(Class) \
public: \
    static Class* Instance() \
    { \
        static QMutex mutex; \
        static QScopedPointer<Class> inst; \
        if (Q_UNLIKELY(!inst)) { \
            mutex.lock(); \
            if (!inst) inst.reset(new Class); \
            mutex.unlock(); \
        } \
        return inst.data(); \
    }

#endif // SINGLETON_H
