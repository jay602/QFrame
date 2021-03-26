#include "abstractdownloadtask.h"
#include "Common/fileutils.h"
#include <QFileInfo>
#include <QDebug>

AbstractDownloadTask::AbstractDownloadTask( QObject* parent /*= 0*/ )
	:QObject(parent)
	,m_savePath(""), m_fileName("")
	,m_state(Finished)
{

}

AbstractDownloadTask::AbstractDownloadTask( const QString& fileName, QObject* parent /*= 0*/ )
	:QObject(parent)
	,m_savePath("")
	,m_state(Finished)
{
	setFullFileName(fileName);
}

AbstractDownloadTask::~AbstractDownloadTask()
{

}

void AbstractDownloadTask::setFullFileName( const QString& fileName )
{
	QFileInfo info(FileUtils::getFullPath(fileName));
	m_fileName = info.fileName();
	m_savePath = info.path();
}

void AbstractDownloadTask::setFileName( const QString& fileName )
{
	m_fileName = fileName;
}

QString AbstractDownloadTask::savePath() const
{
	return std::move(m_savePath);
}

QString AbstractDownloadTask::fileName() const
{
	return std::move(m_fileName);
}

QString AbstractDownloadTask::fullPath() const
{
	return std::move(m_savePath + "/" + m_fileName);
}

AbstractDownloadTask::TaskState AbstractDownloadTask::state() const
{
	return m_state;
}

void AbstractDownloadTask::setSavePath( const QString& path )
{
	m_savePath = FileUtils::getFullPath(path);
}

bool AbstractDownloadTask::isRunning() const
{
	return m_state == Running;
}

bool AbstractDownloadTask::isPaused() const
{
	return m_state == Paused;
}

bool AbstractDownloadTask::isFinished() const
{
	return m_state == Finished;
}

void AbstractDownloadTask::setTaskState( TaskState taskState )
{
    if(taskState == Running) qDebug() << QStringLiteral("开始下载...");
    if(taskState == Finished) qDebug() << QStringLiteral("下载结束！");
	m_state = taskState;
}
