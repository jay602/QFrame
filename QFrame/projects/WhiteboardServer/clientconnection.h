#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>

class ClientConnection : public QTcpSocket
{
    Q_OBJECT

public:
    ClientConnection(QObject *parent=0);
    ~ClientConnection();

    QString info();

protected slots:
    void onReadyRead();
};

#endif // CLIENTCONNECTION_H
