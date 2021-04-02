#include "mainwindow.h"
#include "loginwidget.h"
#include "paintwidget.h"
#include <QStackedWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_centralWidget = new QStackedWidget();
    setCentralWidget(m_centralWidget);
    m_loginWidget = new LoginWidget(this);
    m_paintWidget = new PaintWidget(this);
    m_centralWidget->addWidget(m_loginWidget);
    m_centralWidget->addWidget(m_paintWidget);
    m_centralWidget->setCurrentWidget(m_loginWidget);

    m_tcpClient = new QTcpSocket(this);
    this->setWindowTitle("WhiteBoard");

    connect(m_loginWidget, SIGNAL(login()), this, SLOT(startLogin()));
    connect(m_tcpClient, SIGNAL(connected()), this, SLOT(onConnected()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::startLogin()
{
    qDebug() << "ip : " <<  m_loginWidget->getIp();
    qDebug() << "port : " <<  m_loginWidget->getPort();
    m_tcpClient->connectToHost(m_loginWidget->getIp(), m_loginWidget->getPort());
    bool connected = m_tcpClient->waitForConnected();
    qDebug() << "开始登陆----" << connected;
}

void MainWindow::onConnected()
{
    m_centralWidget->setCurrentWidget(m_paintWidget);
}

