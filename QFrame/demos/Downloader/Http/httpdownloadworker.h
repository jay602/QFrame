#ifndef HTTPDOWNLOADWORKER_H
#define HTTPDOWNLOADWORKER_H

#include <QFile>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "filepiece.h"
#include "networkerror.h"
#include <QWeakPointer>
#include <QSharedPointer>

class HttpDownloadTask;

class HttpDownloadWorker : public QObject
{
	Q_OBJECT
public:
	HttpDownloadWorker(HttpDownloadTask* task, FilePiece& filePiece) ;
	~HttpDownloadWorker();

	Q_INVOKABLE void stop();
	NetworkError error() const;

public slots: 	
	void doWork();

signals:
	void finished();
	void pieceFinished(int id);
	void pieceProgressChanged(int id, qint64 doneByte);
	void error(const NetworkError& errorInfo);

private slots:
	void slotReplyFinished();
	void slotError(QNetworkReply::NetworkError code);

private:
	bool init();
	bool mapFile2Memory();

	void clear();
	void downloadSmallPiece();
	void emitError(const NetworkError& error);

	int write2File(const QByteArray& data);
	qint64 leftBytes() const;

private:
	FilePiece m_filePiece;
	NetworkError m_error;
	int m_tryTimes;
	QString m_url;

	uchar* m_pData; //�����ֶ�ɾ����ָ�룡�Ը�ָ���ε�unMap���ᷢ�����������Ѳ⣩
	QSharedPointer<QFile> m_file;
	QSharedPointer<QNetworkAccessManager> m_manager;

	static const qint64 m_requestBytes;
};

#endif //HTTPDOWNLOADWORKER_H