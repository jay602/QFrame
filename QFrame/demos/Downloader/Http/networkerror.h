#ifndef NETWORKERROR_H
#define NETWORKERROR_H

#include <QObject>
#include <QNetworkReply>
#define UserDefinedErrorCode 999

class NetworkError : public QObject
{
	Q_OBJECT
public:
	NetworkError();
    NetworkError(QNetworkReply::NetworkError error, QObject *parent = 0);
	NetworkError(const NetworkError& error);
	~NetworkError();

	void setError(const QNetworkReply::NetworkError& error);
	void setUserDefinedError(const QString& error);

	QString toString() const;
	QString userDefinedErrorString() const;

	bool hasError() const;

	QNetworkReply::NetworkError error() const;
	NetworkError& NetworkError::operator =(const NetworkError & error);

private: 
	static void initErrorCode();

private:
	QNetworkReply::NetworkError m_error;
	QString m_userDefinedError;

	static QHash<QNetworkReply::NetworkError, QString> Error_Codes;
	static const QNetworkReply::NetworkError UserDefinedError = (QNetworkReply::NetworkError) UserDefinedErrorCode;
};

#endif //NETWORKERROR_H