
//************************************************************************
// class:		FilePiece
// Access:		私有类,不能外部创建;
// description:	提供下载信息xml和log文件操作， 为下载提供断点续传支持;
//************************************************************************

#ifndef FILEPIECE_H
#define FILEPIECE_H

#include <QObject>

class FilePiece : public QObject
{
	Q_OBJECT
	friend class HttpDownloadHelper;
	friend class HttpDownloadTask;
	friend class HttpDownloadWorker;

public:
	FilePiece(const FilePiece& piece);
	~FilePiece();
	bool isInitialized() const;
	void clear();
	FilePiece& operator = (const FilePiece& piece);
	bool operator == (const FilePiece& piece);

private:
	explicit FilePiece(QObject* parent = 0);
	explicit FilePiece (qint64 id, qint64 startByte, qint64 endByte, qint64 doneBytes = 0, QObject* parent = 0);
	

private:
	qint64 ID;
	qint64 StartByte;
	qint64 EndByte;
	qint64 DoneBytes;

};
#endif // FILEPIECE_H
