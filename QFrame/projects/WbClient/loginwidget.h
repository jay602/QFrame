#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

class QFormLayout;
class QPushButton;
class QLineEdit;

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);

    QString getUserName() { return m_userName; }
    const QString& getIp() { return m_ip; }
    quint16 getPort() { return m_port; }

public slots:
    void startLogin();

signals:
    void login();

private:
    QFormLayout* m_layOut;
    QPushButton* m_loginBtn;
    QLineEdit* m_nameText;
    QLineEdit* m_ipText;
    QLineEdit* m_portText;
    QString m_userName;
    QString m_ip;
    quint16 m_port;
};

#endif // LOGINWIDGET_H
