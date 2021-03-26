/****************************************************************************
** 
**˵��: ��װ��ϵͳ��ص��̡߳����̵ȵĹ��ܺ���
**
****************************************************************************/

#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <QObject>
#include <QRect>
#include <windows.h>

class SystemUtils: public QObject
{
	Q_OBJECT

public:
	static bool isProcessIsRunning(const QString& processName);
	static QStringList getAllRunningProcess();
	static bool killProcess(const QString& processName);
	static void sleep(unsigned int msSecond);
	//static DWORD getProcessFromName(QString name)
};

#endif