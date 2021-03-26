#include "filepiece.h"
#include <QThread>

FilePiece::FilePiece( QObject* parent /*= 0*/ )
	: QObject(parent)
	, ID(-1), StartByte(0), EndByte(0), DoneBytes(0) 
{

}

FilePiece::FilePiece( qint64 id, qint64 startByte, qint64 endByte, qint64 doneBytes /*= 0*/, QObject* parent /*= 0*/ )
	: QObject(parent)
	, ID(id), StartByte(startByte) , EndByte(endByte), DoneBytes(doneBytes)
{
}

FilePiece::FilePiece( const FilePiece& piece )
{
	this->ID = piece.ID;
	this->StartByte = piece.StartByte;
	this->EndByte = piece.EndByte;
	this->DoneBytes = piece.DoneBytes;
}

FilePiece::~FilePiece()
{

}


FilePiece& FilePiece::operator= ( const FilePiece& piece )
{
	this->ID = piece.ID;
	this->StartByte = piece.StartByte;
	this->EndByte = piece.EndByte;
	this->DoneBytes = piece.DoneBytes;
	return *this;
}

bool FilePiece::operator==( const FilePiece& piece )
{
	return ID == piece.ID;
}

void FilePiece::clear()
{
	ID = -1;
	StartByte = EndByte = DoneBytes = 0;
}

bool FilePiece::isInitialized() const
{
	return ID != -1;
}

