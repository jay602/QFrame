#include "HttpDownloadWorker.h"
#include "httpdownloadtask.h"
#include "Common/fileutils.h"
#include <QWriteLocker>
#include <QEventLoop>
#define TRY_TIMES 3

const qint64 HttpDownloadWorker::m_requestBytes = 64 * 1024; //64K

static void doDeleteLater(QFile *obj)
{
	obj->close();
	obj->deleteLater();
}

HttpDownloadWorker::HttpDownloadWorker(HttpDownloadTask* task, FilePiece& filePiece)
	:m_filePiece(filePiece)
	,m_manager(NULL), m_file(NULL), m_tryTimes(TRY_TIMES)
{
	QString file = task->tempFullFileName();
	m_url = task->taskUrl();
	m_manager = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager(this), &QObject::deleteLater);
	m_file = QSharedPointer<QFile>(new QFile(file), doDeleteLater);
}

HttpDownloadWorker::~HttpDownloadWorker()
{
	clear();
}

void HttpDownloadWorker::slotReplyFinished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	Q_ASSERT(reply);

	if(!m_error.hasError())
	{
		m_tryTimes = TRY_TIMES;
		int bytes = write2File(reply->readAll());
		m_filePiece.DoneBytes += bytes;
		emit pieceProgressChanged(m_filePiece.ID, m_filePiece.DoneBytes);
// 		QString info = QString("piece id: %0 ,total: %1, left: %2").arg(QString::number(m_filePiece.ID)).arg(QString::number(m_filePiece.DoneBytes)).arg(QString::number(leftBytes()));
// 	    qDebug()<<info;
		if(leftBytes() != 0) 
		{
			downloadSmallPiece();
		}
		else
		{
			emit pieceFinished(m_filePiece.ID);
			emit finished();
		}
	}
	else 
	{
		//Retry
		if((--m_tryTimes) > 0)
		{
			reply->deleteLater();
			downloadSmallPiece();
			return;
		}
		//下载错误，提交错误信号;
		emitError(error());
	}
}

void HttpDownloadWorker::downloadSmallPiece()
{
	qint64 startByte = m_filePiece.StartByte + m_filePiece.DoneBytes;
	qint64 endByte = startByte + qMin(m_requestBytes, leftBytes()) - 1;
	QString strData = QString("bytes=%0-%1").arg(startByte).arg(endByte);

	QNetworkRequest request;
	request.setUrl(m_url);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	request.setRawHeader("Range", strData.toAscii());
#else
	request.setRawHeader("Range", strData.toLatin1());
#endif
	request.setRawHeader("User-Agent", "Rokh.V5");
	//request.setRawHeader("Host", url.port());
	//request.setRawHeader("Accept-Type", "");

	QNetworkReply* reply = m_manager->get(request);
	if(!reply) return;

	connect(reply, SIGNAL(finished()), this, SLOT(slotReplyFinished()), Qt::QueuedConnection);
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)), Qt::QueuedConnection);
}

qint64 HttpDownloadWorker::leftBytes() const
{
	return m_filePiece.EndByte - m_filePiece.StartByte + 1 - m_filePiece.DoneBytes; 
}

bool HttpDownloadWorker::init()
{
	return true;
}

int HttpDownloadWorker::write2File(const QByteArray& data)
{
	memcpy(m_pData + m_filePiece.DoneBytes, data.data(), data.size());
	return data.size();
}

bool HttpDownloadWorker::mapFile2Memory()
{
	QString name = m_file->fileName();
	if(!FileUtils::exists(name) || 
		(!m_file->isOpen() && !m_file->open(QIODevice::ReadWrite)))
	{
		m_error.setUserDefinedError(QString("Open the tmep file: %0 failed in mapFile2Memory ").arg(name));
		return false;
	}

	if( !(m_pData = m_file->map(m_filePiece.StartByte, m_filePiece.EndByte - m_filePiece.StartByte + 1)))
	{
		m_error.setUserDefinedError("Map file to memeory failed: " + QString::number(m_file->error()));
		return false;
	}
	return true;
}

void HttpDownloadWorker::doWork()
{
	if(!mapFile2Memory()) //映射文件到内存
	{
		emitError(error());
		return;
	}
	downloadSmallPiece();
}


void HttpDownloadWorker::clear()
{	
	if(m_pData && m_file)
	{
		m_file->unmap(m_pData);
		m_pData = NULL;
		if(m_file->isOpen()) 
		{
			m_file->close();
		}
	}

	m_filePiece.clear();
	m_tryTimes = TRY_TIMES;
}

void HttpDownloadWorker::emitError( const NetworkError& errorInfo )
{
	if(m_file->isOpen()) m_file->close();
	emit error(m_error);
	emit pieceFinished(m_filePiece.ID);
	finished();
}

void HttpDownloadWorker::slotError(QNetworkReply::NetworkError code)
{
	m_error.setError(code);
	emitError(error());
}

NetworkError HttpDownloadWorker::error() const
{
	return m_error;
}

Q_INVOKABLE void HttpDownloadWorker::stop()
{
	//m_error.setError(QNetworkReply::OperationCanceledError);
	emit finished();
}


