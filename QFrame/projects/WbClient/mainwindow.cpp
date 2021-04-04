#include "mainwindow.h"
#include "loginwidget.h"
#include "paintwidget.h"
#include <QStackedWidget>
#include <QActionGroup>
#include <QAction>
#include <QDebug>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_toolBar(nullptr)
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
    if(!m_toolBar)
    {
        QToolBar *toolbar = addToolBar("Figure Type");
        QActionGroup *actionGroup = new QActionGroup(toolbar);

        m_toolBar = toolbar;

        QAction *action = toolbar->addAction(QIcon(":/line.png"),
                                             "Draw a Line",
                                             this,SLOT(onDrawLineAction()));
        action->setCheckable(true);
        action->setChecked(true);
        action->setActionGroup(actionGroup);

        action = toolbar->addAction(QIcon(":/rect.png"),
                                    "Draw a Rectangle",
                                    this,SLOT(onDrawRectangleAction()));
        action->setCheckable(true);
        action->setActionGroup(actionGroup);

        action = toolbar->addAction(QIcon(":/oval.png"),
                                    "Draw a Oval",this,SLOT(onDrawOvalAction()));
        action->setCheckable(true);
        action->setActionGroup(actionGroup);

        action = toolbar->addAction(QIcon(":/triangle.png"),
                                    "Draw a Triangle",this,SLOT(onDrawTriangleAction()));
        action->setCheckable(true);
        action->setActionGroup(actionGroup);

        action = toolbar->addAction(QIcon(":/pen.png"),
                                    "Draw as you like",this,SLOT(onDrawGraffitiAction()));
        action->setCheckable(true);
        action->setActionGroup(actionGroup);

        toolbar->addSeparator();
        action = toolbar ->addAction(QIcon(":/del.png"),
                                     "Delete the Last",this,SLOT(onUnio()));
        //action->setCheckable(true);
        action->setActionGroup(actionGroup);


        action = toolbar->addAction(QIcon(":/clear.png"),
                                    "Clear All",this,SLOT(onClearAll()));
        //action->setCheckable(true);
        action->setActionGroup(actionGroup);
    }
    else
        addToolBar(m_toolBar);
    m_centralWidget->setCurrentWidget(m_paintWidget);
}

void  MainWindow::onDrawLineAction()
{

}

void  MainWindow::onDrawRectangleAction()
{

}

void  MainWindow::onDrawOvalAction()
{

}

void  MainWindow::onDrawTriangleAction()
{

}

void  MainWindow::onDrawGraffitiAction()
{

}

void  MainWindow::onUnio()
{

}

void  MainWindow::onClearAll()
{

}

