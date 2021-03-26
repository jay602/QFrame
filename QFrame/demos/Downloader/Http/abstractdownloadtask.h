#ifndef ABSTRACTDOWNLOADTASK_H
#define ABSTRACTDOWNLOADTASK_H

#include <QObject>

class AbstractDownloadTask : public QObject
{
	Q_OBJECT
public:
	enum TaskState
	{
		Finished, 
		Running, 
		Canceled, 
		Paused
	};

public:
	AbstractDownloadTask( QObject* parent = 0 );
	AbstractDownloadTask(const QString& fileName, QObject* parent = 0);
	virtual ~AbstractDownloadTask();

	virtual void start() = 0;
	virtual void stop() = 0;

	void setFullFileName(const QString& fileName);
	void setFileName(const QString& fileName);
	void setSavePath(const QString& path);
	
	QString savePath() const;
	QString fileName() const;
	QString fullPath() const;

	bool isRunning() const;
	bool isPaused() const;
	bool isFinished() const;

	TaskState state() const;

protected:
	void setTaskState(TaskState taskState);

protected:
	QString m_savePath;
	QString m_fileName;
	TaskState m_state;
};

#endif //ABSTRACTDOWNLOADTASK_H