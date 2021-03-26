#include "systemutils.h"
#include <QProcess>
#include <QMutex>
#include <QTime>
#include <QWaitCondition>
#include <windows.h>
#include <TlHelp32.h>
#include <QCoreApplication>

QStringList SystemUtils::getAllRunningProcess()
{
	QProcess process;
	process.setReadChannel(QProcess::StandardOutput);
	process.setReadChannelMode(QProcess::MergedChannels);

#ifdef Q_OS_WIN
	process.start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption");
#endif

	process.waitForStarted(1000);
	process.waitForFinished(1000);

	QByteArray list = process.readAll();
	QList<QByteArray> _tt = list.split('\n');
	QStringList proList;
	for(int i = 0 ; i< _tt.length(); i++)
	{
		QString tmp = QString::fromLocal8Bit( _tt.at(i).simplified());
		if(tmp.isEmpty()) continue;
		proList<<tmp;
	}
	return proList;
}

bool SystemUtils::isProcessIsRunning(const QString& processName)
{
#ifdef Q_OS_WIN

	PROCESSENTRY32 pE;
	pE.dwSize = sizeof(pE);
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapshot == INVALID_HANDLE_VALUE) 
	{
		return false;
	}

	bool bNext = Process32First(hSnapshot, &pE);
	while(bNext)
	{
		QString name = QString::fromWCharArray(pE.szExeFile);
		if(!name.compare(processName, Qt::CaseInsensitive)) {
			CloseHandle(hSnapshot);
			return true;
		}
		bNext = Process32Next(hSnapshot, &pE);
	}
	CloseHandle(hSnapshot);
#endif

	return false;
}

bool SystemUtils::killProcess(const QString& processName)
{
	bool bRet = true;

#ifdef Q_OS_WIN
	PROCESSENTRY32 pE;
	pE.dwSize = sizeof(pE);
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapshot == INVALID_HANDLE_VALUE) 
	{
		return false;
	}

	bool bNext = Process32First(hSnapshot, &pE);
	while(bNext)
	{
		QString name = QString::fromWCharArray(pE.szExeFile);
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, pE.th32ProcessID);
		if(!name.compare(processName, Qt::CaseInsensitive) &&
			pE.th32ProcessID != ::GetCurrentProcessId() &&
			hProcess) {
			bool ret =  ::TerminateProcess(hProcess, 0);
			if(bRet){ bRet = ret; }
			if(ret) CloseHandle(hProcess);
		}
		bNext = Process32Next(hSnapshot, &pE);
	}
	CloseHandle(hSnapshot);
#endif

	return bRet;
}

void SystemUtils::sleep(unsigned int msSecond)
{
	QTime t; 
	t.start(); 
	while(t.elapsed() < msSecond) 
	{ 
		QCoreApplication::processEvents(); 
	} 
}

