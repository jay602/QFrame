#ifndef HTTPDOWNLOADTASK_H
#define HTTPDOWNLOADTASK_H

#include "abstractdownloadtask.h"
#include "networkerror.h"
#include <QUrl>
#include <QMutex>
#include <QReadWriteLock>

class FilePiece;
class AbstractDownloadTask;
class HttpDownloadWorker;

struct TaskData
{
	int	 ThreadCount;
	qint64 FileSize;
	qint64 UnstartedOffsetIndex; //从未下载块的偏移量 = 文件长度 - 下载完成的 - 正在下载的;

	QString TaskUrl;
	QList<FilePiece> UnFinishedPieces;
};

class HttpDownloadTask : public AbstractDownloadTask
{
	Q_OBJECT
public:
	HttpDownloadTask(const QUrl& url, const QString& fullFileName, QObject* parent = 0);
	~HttpDownloadTask();
	
	void start();
	void stop();
	void setThreadCount(int count);

    qint64 getFileInfoSize(QString url, int tryTimes);

	QString taskUrl() const;
    QString tempFullFileName() const;
	int downLoadThreadCount() const;
	NetworkError error() const;

signals:
	void finished();
	void error(const NetworkError& errorInfo);

private slots:
	void slotPieceFinished(int pieceID);
	void slotError(const NetworkError& errorInfo);
	void slotPieceProgressChanged(int pieceId, qint64 doneBytes);

private:
	bool createTempFile(const QString& name, int size);
	bool cvtTempFile2OriginFile();
	bool requirePiece(FilePiece& fp);
	bool hasContainsPiece(QList<FilePiece>& fpList, qint64 ID) const;
	bool initFileSize();
	bool serializeTaskData();
	bool unSerializeTaskData();
	bool startOneWorker();
	
	void resume();
	void doNewTask();
	void calcPieceSize();
	void splitFileForDownload();
	void doDownload();
	void doFinished();
	void emitError(const NetworkError& errorInfo);

	int getRemoteFileSize();
	int findPiece(QList<FilePiece>& fpList, qint64 ID) const;

private:
	qint64 m_pieceSize;

	QMutex m_mutex;
	QReadWriteLock m_readWriteLock;

	TaskData m_taskData;
	NetworkError m_error;

	QList<FilePiece> m_waittingPieces;
	QList<HttpDownloadWorker*> m_workers;
};

#endif // HTTPDOWNLOADTASK_H
