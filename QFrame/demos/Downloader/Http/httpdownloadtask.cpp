#include "filepiece.h"
#include "abstractdownloadtask.h"
#include "httpdownloadtask.h"
#include "httpdownloadworker.h"
#include "httpdownloadtask.h"
#include "Common/fileutils.h"
#include "Common/systemutils.h"

#include <math.h>
#include <QEventLoop>
#include <QNetworkProxyFactory>
#include <QFileInfo>
#include <QDataStream>
#include <QTimer>
#include <QPair>

#define TmpSuffix ".rkdl" //未下载完成的文件;
#define InfoBlock 64 * 1024 //文件信息块大小;
#define MinPieceSize 2 * 1024 * 1024 //要保证是64k的倍数

HttpDownloadTask::HttpDownloadTask( const QUrl& url, const QString &saveFileName, QObject* parent /*= 0*/ )
	:AbstractDownloadTask(saveFileName, parent)
	,m_pieceSize(MinPieceSize)/*, m_helper(NULL)*/
{
	QString strUrl = url.toString().toLower();
	m_taskData.TaskUrl = strUrl;
	m_taskData.ThreadCount = 5;
	m_taskData.FileSize = -1;
	m_taskData.UnstartedOffsetIndex = 0;
	QNetworkProxyFactory::setUseSystemConfiguration(true);
	if(fileName().isEmpty())
	{
		QFileInfo info(url.path());
		setFileName(info.fileName());
	}
}

HttpDownloadTask::~HttpDownloadTask()  
{
	/*Free_Ptr(m_helper);*/
}

QString HttpDownloadTask::taskUrl() const
{
	return m_taskData.TaskUrl;
}

NetworkError HttpDownloadTask::error() const
{
	return m_error;
}

int HttpDownloadTask::downLoadThreadCount() const
{
	return m_taskData.ThreadCount;
}

bool HttpDownloadTask::initFileSize()
{
	int fileSize = getRemoteFileSize();
	if(fileSize == -1) return false;
	m_taskData.FileSize = fileSize;
	return true;
}

void HttpDownloadTask::start()
{
	if(isRunning()) return;
	if(!initFileSize()) 
	{
		emitError(m_error);
		return;
	}

	if(fileName().isEmpty())
	{
		m_error.setUserDefinedError("无法获取下载文件名！");
		emitError(m_error);
		return;
	}

	QString tmpFile = tempFullFileName();
	if(FileUtils::exists(tmpFile))
	{ 
		return resume();
	}
	else
	{
		return doNewTask();
	}
}

void HttpDownloadTask::doNewTask()
{
	QString tmpFile = tempFullFileName();
	if(!createTempFile(tmpFile, m_taskData.FileSize))
	{
		m_error.setUserDefinedError("Create temp file failed in func doNewTask!");
		emitError(m_error);
		return;
	}
		
	if(!serializeTaskData())
	{
		m_error.setUserDefinedError("Serialize task info failed in func doNewTask!");
		emitError(m_error);
		return;
	}
	doDownload();
}


void HttpDownloadTask::resume()
{	
	//从配置文件读入信息;
	qint64 fileSize = m_taskData.FileSize;
	if(!unSerializeTaskData())
	{
		m_error.setUserDefinedError("Unserialize task info failed in func resume!");
		emitError(m_error);
		return;
	}

	//验证下载;
	if(fileSize != m_taskData.FileSize)
	{
		if(fileSize != -1)
			m_error.setUserDefinedError("Assert fileSize failed in func resume!");
		emitError(m_error);
		return;
	}

	//下载还未完成的pieces;
 	m_waittingPieces.append(m_taskData.UnFinishedPieces);

	doDownload();
}

void HttpDownloadTask::stop()
{
	foreach(HttpDownloadWorker* worker, m_workers)
	{
		QMetaObject::invokeMethod(worker, "stop", Qt::BlockingQueuedConnection);
		//worker->deleteLater();
	}
	m_workers.clear();
	setTaskState(Finished);

}



qint64 HttpDownloadTask::getFileInfoSize(QString url, int tryTimes)
{
    qint64 size = -1;
     \
    if (tryTimes <= 0)
    {
        tryTimes = 1;
    }

     do
     {
         QNetworkAccessManager manager;
         // 事件循环，等待请求文件头信息结束;
         QEventLoop loop;
         // 超时，结束事件循环;
         QTimer timer;

         //发出请求，获取文件地址的头部信息;
         QNetworkReply *reply = manager.head(QNetworkRequest(url));
         if (!reply)
             continue;

         connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
         connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

         timer.start(2000);
         loop.exec();

         if (reply->error() != QNetworkReply::NoError)
         {
             qDebug() << reply->errorString();
             continue;
         }
         if (reply->error() != QNetworkReply::NoError)
         {
             // 请求发生错误;
             qDebug() << reply->errorString();
             continue;
         }
         else if (!timer.isActive())
         {
             // 请求超时超时,未获取到文件信息;
             qDebug() << "Request Timeout";
             continue;
         }
         timer.stop();

         QVariant var = reply->header(QNetworkRequest::ContentLengthHeader);
         const QList<QNetworkReply::RawHeaderPair> lst = reply->rawHeaderPairs();
         size = var.toLongLong();
         for(auto item : lst)
         {
             qDebug() << "size = " << lst.size();
         }

         reply->deleteLater();
         break;
     } while (tryTimes--);

     return size;
}

int HttpDownloadTask::getRemoteFileSize()
{
	QNetworkAccessManager manager;
	QNetworkRequest headReq(m_taskData.TaskUrl);
    qDebug()<< QStringLiteral("获取下载文件的大小......");
	headReq.setRawHeader("User-Agent", "Rokh.V5");

	QNetworkReply* headReply = NULL;
	bool connectError = false;
	int tryTimes = 3;
	//如果失败,连接尝试3次;
    do
	{
        qDebug()<< QStringLiteral("正在进行") << 4 - tryTimes << QStringLiteral("次连接尝试...");
		connectError = false;
		headReply =  manager.head(headReq);
		if(!headReply) 
		{
            qDebug() << "connectError " << 4 - tryTimes;
			connectError = true;
			continue;
		}

		QEventLoop loop;
		connect(headReply, SIGNAL(finished()), &loop, SLOT(quit()));
		loop.exec();
		connectError = (headReply->error() != QNetworkReply::NoError); 
        if(connectError)
            qDebug() << QStringLiteral("11连接失败!");
		headReply->deleteLater();

	} while (connectError && --tryTimes);

	if(connectError)
	{
		headReply->error() == QNetworkReply::NoError ?
			m_error.setUserDefinedError("Unkown error in func getFileSize!") : m_error.setError(headReply->error());
		return -1;
	}
    qDebug() << QStringLiteral("连接成功!");

	
	int fSize = -1;
	int statusCode = headReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if(statusCode == 302) // 处理重定向
	{
		QUrl newUrl = headReply->header(QNetworkRequest::LocationHeader).toUrl();
		if(newUrl.isValid())
		{
            qDebug() << QStringLiteral("重定向：") << newUrl;
			m_taskData.TaskUrl = newUrl.toString();
			QFileInfo info(newUrl.path());
			setFileName(info.fileName());
			return getRemoteFileSize();
		}
		else
		{
			m_error.setUserDefinedError("重定向Url错误 in func getFileSize!");
		}
	}
	else
	{
		bool bSuc = false; 
		fSize = headReply->header(QNetworkRequest::ContentLengthHeader).toInt(&bSuc);
		if(!bSuc)
			m_error.setUserDefinedError("ContentLength covert failed in func getFileSize!");
	}
	headReply->deleteLater();
	return fSize;
}

void HttpDownloadTask::doDownload()
{
	splitFileForDownload();
	if(m_waittingPieces.size() == 0)
	{
		doFinished();
		return;
	}

	setTaskState(Running);
	int n = qMin(m_waittingPieces.size(), m_taskData.ThreadCount);
	for(int i = 0; i < n; i++)
	{
		if(!startOneWorker())
		{
			m_error.setUserDefinedError("startOneWorker failed in func doDownload");
			emitError(m_error);
			return;
		}
	}
}

bool HttpDownloadTask::createTempFile( const QString& name, int size )
{
	int realSize = size + InfoBlock;
	if(!FileUtils::createFile(name, realSize, true)) 
	{
		m_error.setUserDefinedError("Can't create temp downloadding file!");
		return false;
	}
	return true;
}

//划分任务创建线程
void HttpDownloadTask::calcPieceSize()
{
	m_pieceSize = (qint64) ceil(qMax(m_taskData.FileSize / (10.0 * m_taskData.ThreadCount), MinPieceSize * 1.0));
	qint64 remiander = m_pieceSize % (64 * 1024);//windows下系统的分配粒度是64K, 保证每个分段的起始字节数都是n倍64K;
	m_pieceSize -= remiander;
}

void HttpDownloadTask::splitFileForDownload()
{
	calcPieceSize();
	qint64 sumPieceIndex = (qint64) ceil( m_taskData.FileSize / (double)m_pieceSize );
	qint64 unstartedPieceIndex =  m_taskData.UnstartedOffsetIndex;
	qint64 pieceId = unstartedPieceIndex + 1;

	if(unstartedPieceIndex >= sumPieceIndex) return;

	qDebug()<<"FileSize:"<<m_taskData.FileSize<<" ThreadCount:"<<m_taskData.ThreadCount<<" FileUnstartedIndex:"<<m_taskData.UnstartedOffsetIndex<<"Piece Total:"<<sumPieceIndex;
	for(qint64 i = unstartedPieceIndex; i != sumPieceIndex; i++, pieceId++)
	{
		if(hasContainsPiece(m_waittingPieces, pieceId)) continue;
		if( pieceId == sumPieceIndex )
		{//最后一块
			m_waittingPieces.append(FilePiece(pieceId, i * m_pieceSize, m_taskData.FileSize - 1));
		}
		else
		{
			m_waittingPieces.append(FilePiece(pieceId, i * m_pieceSize, pieceId * m_pieceSize - 1));
		}
	}
}

bool HttpDownloadTask::requirePiece(FilePiece& piece)
{
	if(!m_waittingPieces.isEmpty())
	{
		piece = m_waittingPieces.takeAt(0);
		if(!hasContainsPiece(m_taskData.UnFinishedPieces, piece.ID)) 
		{
			m_taskData.UnFinishedPieces.append(piece);
			if(piece.ID > m_taskData.UnstartedOffsetIndex) m_taskData.UnstartedOffsetIndex = piece.ID;
		}
	
		if(!serializeTaskData())
		{
			return false;
		}
		qDebug()<<"Piece Start: "<<"ID:"<<piece.ID<<", start:"<<piece.StartByte<<", end:"<<piece.EndByte<<", Done:"<<piece.DoneBytes;
		return true;
	}
	return false;
}

bool HttpDownloadTask::startOneWorker()
{
	FilePiece fp;
	if(!requirePiece(fp)) 
	{
		return false;
	}

	HttpDownloadWorker* worker = new HttpDownloadWorker(this, fp);
	QThread* thread = new QThread(this);
	worker->moveToThread(thread);
	worker->connect(thread, SIGNAL(started()), SLOT(doWork()));
	thread->connect(worker, SIGNAL(finished()), SLOT(quit()));
	worker->connect(thread, SIGNAL(finished()), SLOT(deleteLater()));
	thread->connect(thread, SIGNAL(finished()), SLOT(deleteLater()));

	connect(worker, SIGNAL(pieceProgressChanged(int,qint64)), SLOT(slotPieceProgressChanged(int,qint64)), Qt::QueuedConnection);
	connect(worker, SIGNAL(pieceFinished(int)), SLOT(slotPieceFinished(int)), Qt::QueuedConnection);
	connect(worker, SIGNAL(error(const NetworkError&)), SLOT(slotError(const NetworkError&)), Qt::QueuedConnection);

	m_workers.append(worker);
	thread->start();
	//QtConcurrent::run(worker, &HttpDownloadWorker::doWork);
	return true;
}

void HttpDownloadTask::slotPieceProgressChanged( int pieceID, qint64 doneBytes )
{
	m_mutex.lock();
	//m_readWriteLock.lockForWrite();
	int index = findPiece(m_taskData.UnFinishedPieces, pieceID);
	if(index != -1)
	{
		FilePiece fp = m_taskData.UnFinishedPieces.at(index);
		fp.DoneBytes = doneBytes;
		m_taskData.UnFinishedPieces.replace(index, fp);
		if(!serializeTaskData())
		{
			qDebug()<<"serializeTaskData in slotPieceProgressChanged id: "<<pieceID;
		}
	}
	//m_readWriteLock.unlock();
	m_mutex.unlock();
}

void HttpDownloadTask::slotPieceFinished( int pieceID )
{
	m_mutex.lock();
	//m_readWriteLock.lockForWrite();

	HttpDownloadWorker* worker = qobject_cast<HttpDownloadWorker*>(sender()); 
	Q_ASSERT(worker);
	//worker->deleteLater();
	m_workers.removeAll(worker);
	
	if(m_error.hasError()) 
	{
		emitError(m_error);
		return;
	}

	qDebug()<<"Piece Finished: "<< pieceID;
    int index = findPiece(m_taskData.UnFinishedPieces, pieceID);
	if(index != -1)
	{
		m_taskData.UnFinishedPieces.removeAt(index);
		if(!serializeTaskData())
		{
			qDebug()<<"serializeTaskData in slotPieceFinished id: "<<pieceID;
		}
	}

	 if(!m_waittingPieces.isEmpty())
	 {
		 if(!startOneWorker())
		 {
			 m_error.setUserDefinedError("RestartOneWorker failed");
			 emitError(m_error);
			 return;
		 }
	 }
	 else if(m_workers.isEmpty())
	 {//结束了
		 SystemUtils::sleep(2000);
		 doFinished();
	 }
	
	//m_readWriteLock.unlock();
	m_mutex.unlock();
}

bool HttpDownloadTask::cvtTempFile2OriginFile()
{
	//去掉信息块
	FileUtils::rmFile(fullPath());

	if(!QFile::resize(tempFullFileName(), m_taskData.FileSize)) return false;
	if(!FileUtils::renameFile(tempFullFileName(), fullPath()))
	{
		return false;
	}
	return true;
}

void HttpDownloadTask::emitError( const NetworkError& errorInfo )
{
    qDebug()<<QStringLiteral("错误: ")<<errorInfo.toString();
	emit error(errorInfo);
	emit finished();
	setTaskState(Finished);
}

void HttpDownloadTask::slotError( const NetworkError& errorInfo)
{
	m_error = errorInfo;
	stop();
}

void HttpDownloadTask::setThreadCount( int count )
{
	if( isFinished() && count >=0)
		m_taskData.ThreadCount = count;
}

QString HttpDownloadTask::tempFullFileName() const
{
	return fullPath() + TmpSuffix;
}

bool HttpDownloadTask::hasContainsPiece(QList<FilePiece>& fpList, qint64 ID ) const
{
	return findPiece(fpList, ID) != -1;
}

int HttpDownloadTask::findPiece( QList<FilePiece>& fpList, qint64 ID ) const
{
	int len = fpList.size();
	for(int i = 0; i < len; i++)
	{
		FilePiece fp = fpList.at(i);
		if(fp.ID == ID)
		{
			return i;
		}
	}
	return -1;
}

bool HttpDownloadTask::serializeTaskData()
{
	QFile file(tempFullFileName());
	if(!file.open(QIODevice::ReadWrite)) return false;
	QDataStream stream(&file);
	int ts = m_taskData.UnFinishedPieces.size();
	qint64 realFileSize = file.size() - InfoBlock;
	quint32 magicNumber = (quint32)0xA0B0C0D0;

	stream.device()->seek(realFileSize);
	stream<<magicNumber<<m_taskData.ThreadCount<<m_taskData.UnstartedOffsetIndex<<m_taskData.TaskUrl<<ts;
	
	for(int i = 0; i < ts; i++)
	{
		FilePiece fp = m_taskData.UnFinishedPieces.at(i);
		stream<<fp.ID<<fp.StartByte<<fp.EndByte<<fp.DoneBytes;
	}
	file.close();
	return true;
}

bool HttpDownloadTask::unSerializeTaskData()
{
	QFile file(tempFullFileName());
	if(!file.open(QIODevice::ReadWrite)) return false;
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_8);
	TaskData data;
	qint64 realFileSize = file.size() - InfoBlock;
	quint32 magicNumber = 0;
	int ts = 0;

	stream.device()->seek(realFileSize);
	stream>>magicNumber;
	if(magicNumber != (quint32)0xA0B0C0D0)
	{
		return false;
	}
	stream>>data.ThreadCount>>data.UnstartedOffsetIndex>>data.TaskUrl>>ts;

	for(int i = 0; i < ts; i++)
	{
		FilePiece fp;
		stream>>fp.ID>>fp.StartByte>>fp.EndByte>>fp.DoneBytes;
		if(hasContainsPiece(data.UnFinishedPieces, fp.ID)) continue;
		data.UnFinishedPieces.append(fp);
	}

	data.FileSize = file.size() - InfoBlock;
	m_taskData = data;
	file.close();
	return true;
}

void HttpDownloadTask::doFinished()
{
	cvtTempFile2OriginFile();
	emit finished();
	setTaskState(Finished);
    qDebug()<<QStringLiteral("下载成功！");
}
