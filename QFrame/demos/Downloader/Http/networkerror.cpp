#include "networkerror.h"

QHash<QNetworkReply::NetworkError, QString> NetworkError::Error_Codes;

NetworkError::NetworkError()
	: QObject()
	, m_error(QNetworkReply::NoError)
{
	initErrorCode();
}

NetworkError::NetworkError(QNetworkReply::NetworkError error, QObject *parent)
	: QObject(parent)
	, m_error(error)/*, m_userDefinedError("")*/
{
	initErrorCode();
}

NetworkError::NetworkError( const NetworkError& error )
{
	m_error = error.error();
	m_userDefinedError = error.userDefinedErrorString();
}

NetworkError::~NetworkError()
{

}

void NetworkError::initErrorCode()
{
	if(!Error_Codes.empty()) return;

#define ERROR_CODE(state, description) Error_Codes.insert(state, description);
	// {{{
	ERROR_CODE(QNetworkReply::NoError,                         "No error");
	ERROR_CODE(QNetworkReply::TimeoutError,                    "The connection to the remote server timed out.");
	ERROR_CODE(QNetworkReply::HostNotFoundError,               "The remote host name was not found (invalid hostname).");
	ERROR_CODE(QNetworkReply::ConnectionRefusedError,          "The remote server refused the connection (the server is not accepting requests).");
	ERROR_CODE(QNetworkReply::SslHandshakeFailedError,         "The SSL/TLS handshake failed and the encrypted channel could not be established.");
	ERROR_CODE(QNetworkReply::OperationCanceledError,          "The operation was canceled via calls to abort() or close() before it was finished.");
	ERROR_CODE(QNetworkReply::ProxyConnectionRefusedError,     "The connection to the proxy server was refused (the proxy server is not accepting requests).");
	ERROR_CODE(QNetworkReply::RemoteHostClosedError,           "The remote server closed the connection prematurely, before the entire reply was received and processed.");
	ERROR_CODE(QNetworkReply::TemporaryNetworkFailureError,    "The connection was broken due to disconnection from the network, however the system has initiated \
															   roaming to another access point. The request should be resubmitted and will be processed as soon as \
															   the connection is re-established.");
	ERROR_CODE(QNetworkReply::ProxyAuthenticationRequiredError, "The proxy requires authentication in order to honour the request but did not accept any credentials offered (if any).");
	ERROR_CODE(QNetworkReply::ProxyTimeoutError,                "The connection to the proxy timed out or the proxy did not reply in time to the request sent.");
	ERROR_CODE(QNetworkReply::ProxyNotFoundError,               "The proxy host name was not found (invalid proxy hostname).");
	ERROR_CODE(QNetworkReply::ProxyConnectionClosedError,       "The proxy server closed the connection prematurely, before the entire reply was received and processed.");
	ERROR_CODE(QNetworkReply::ContentAccessDenied,              "The access to the remote content was denied (similar to HTTP error 401).");
	ERROR_CODE(QNetworkReply::ContentOperationNotPermittedError,"The operation requested on the remote content is not permitted.");
	ERROR_CODE(QNetworkReply::ContentNotFoundError,             "The remote content was not found at the server (similar to HTTP error 404).");
	ERROR_CODE(QNetworkReply::UnknownNetworkError,				"An unknown network-related error was detected.");
	ERROR_CODE(QNetworkReply::UnknownProxyError,				"An unknown proxy-related error was detected.");
	ERROR_CODE(QNetworkReply::ProtocolInvalidOperationError,	"The requested operation is invalid for this protocol.");
	ERROR_CODE(QNetworkReply::ProtocolUnknownError,				"The Network Access API cannot honor the request because the protocol is not known.");
	ERROR_CODE(QNetworkReply::ContentReSendError,				"The request needed to be sent again, but this failed for example because the upload data could not be read a second time.");
	ERROR_CODE(QNetworkReply::AuthenticationRequiredError,      "The remote server requires authentication to serve the content but the credentials provided were not accepted (if any).");
	ERROR_CODE(QNetworkReply::UnknownContentError,				"An unknown error related to the remote content was detected.");
	ERROR_CODE(QNetworkReply::ProtocolFailure,					 "A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.).");
	// }}}
}

void NetworkError::setError(const QNetworkReply::NetworkError& error)
{
	m_error = error;
}

void NetworkError::setUserDefinedError(const QString& txt)
{
	m_error = UserDefinedError;
	m_userDefinedError = txt;
}

QNetworkReply::NetworkError NetworkError::error() const
{
	return m_error;
}

QString NetworkError::userDefinedErrorString() const
{
	return m_userDefinedError;
}

QString NetworkError::toString() const
{
	if(m_error == UserDefinedError)
	{
		return std::move(QString("Code:%0, Description: %1").arg(error()).arg(m_userDefinedError));
	}

	QString strErrorString  = Error_Codes.value(m_error, "");

	if(strErrorString.isEmpty())
	{
		strErrorString = QString("Code:%0, Description: Unknown error.").arg(m_error);
	}

	return std::move(QString("Code:%0, Description: %1").arg(error()).arg(strErrorString));
}

bool NetworkError::hasError() const
{
	return !(m_error == QNetworkReply::NoError);
}

NetworkError& NetworkError::operator=( const NetworkError & error )
{
	m_error = error.error();
	m_userDefinedError = error.userDefinedErrorString();
	return *this;
}


