#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

class LoginWidget;
class QTcpSocket;
class PaintWidget;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void  startLogin();
    void  onConnected();

private:
    LoginWidget* m_loginWidget;
    QTcpSocket* m_tcpClient;
    PaintWidget* m_paintWidget;
    QStackedWidget* m_centralWidget;
};
#endif // MAINWINDOW_H
