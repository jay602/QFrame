#include "mainwindow.h"
#include<QtWidgets>
#include<QToolBar>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QtWidgets>
#include"painterscene.h"
#include"painterview.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,m_scene(nullptr)
    ,m_conn(nullptr)
    ,m_toolBar(nullptr)
    ,m_nameEdit(nullptr)
{
    prepareJoinUI();
}



MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    switch (QMessageBox::information(this,tr("topic"),tr("If not, click the return button to clear"),
                                     ("has cleared"),tr("return"),0,1))
    {
        case 0:
            event->accept();
            break;
        case 1:
            event->ignore();
            break;
        default:
            break;
    }
}

void MainWindow::prepareJoinUI()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(widget);
    QLabel *nameLabel = new QLabel("Input Your Name");
    layout->addWidget(nameLabel);

    m_nameEdit = new QLineEdit("Mary");
    layout->addWidget(m_nameEdit);

    auto button = new QPushButton("Join");
    connect(button,SIGNAL(clicked(bool)),
            this,SLOT(onJoinButtonClicked()));
    layout->addWidget(button);

    layout->addStretch(1);
    setCentralWidget(widget);
}

void MainWindow::preparePainterUI()
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

    m_scene = new PainterScene();
    PainterView *view = new PainterView(m_scene);

    connect(m_scene,SIGNAL(addFigureReq(QJsonObject)),
            this,SLOT(onAddFigureReq(QJsonObject)));
    connect(m_scene,SIGNAL(deletFigureReq(int)),
            this,SLOT(onDeleteFigureReq(int)));
    connect(m_scene,SIGNAL(clearFigureReq(int)),
            this,SLOT(onClearFigureReq(int)));


    setCentralWidget(view);
}


void MainWindow::onJoinButtonClicked()
{
    if(!m_conn)
    {
        m_conn = new WhiteBoardConnection(this);
        QString  strName = m_nameEdit->text();
        connect(m_conn,SIGNAL(joined(QString,int)),
                this,SLOT(onJoined(QString,int)));
        connect(m_conn,SIGNAL(userLeft(QString,int)),
                this,SLOT(onUserLeft(QString,int)));
        connect(m_conn,SIGNAL(figureAdded(QJsonObject)),
                this,SLOT(onFigureAdded(QJsonObject)));
        connect(m_conn,SIGNAL(figureDeleted(int)),
                this,SLOT(onFigureDeleted(int)));
        connect(m_conn,SIGNAL(figureCleared(int)),
                this,SLOT(onFigureCleared(int)));
        //m_conn->join(strName,"111.231.61.143",9001);

        m_conn->join(strName,"127.0.0.1",9001);

    }
}

void MainWindow::onJoined(QString name, int id)
{
    if(id == m_conn->id())
    {
        m_nameEdit = nullptr;
        preparePainterUI();
        m_scene->setUserId(id);

    }
    else
    {

    }
}

void MainWindow::onUserLeft(QString name, int id)
{
    if(id == m_conn->id())
    {
        m_scene = nullptr;
        removeToolBar(m_toolBar);
        prepareJoinUI();
    }
    else
    {

    }
}

void MainWindow::onFigureAdded(const QJsonObject &figure)
{
    m_scene->onFigureAdded(figure);
}

void MainWindow::onFigureDeleted(int id)
{
    m_scene->onFigureDeleted(id);
}

void MainWindow::onFigureCleared(int ownerId)
{
    m_scene->onFigureCleared(ownerId);
}

void MainWindow::onErrorOccured(const QString &desc)
{
    prepareJoinUI();
    if(m_conn)
    {
        m_conn->deleteLater();
        m_conn = nullptr;
    }
}

void MainWindow::onAddFigureReq(const QJsonObject &figure)
{
    if(m_conn) m_conn->addFigure(figure);

}

void MainWindow::onDeleteFigureReq(int id)
{
    if(m_conn) m_conn->deleteFigure(id);
}

void MainWindow::onClearFigureReq(int ownerId)
{
    if(m_conn) m_conn->clearFigure(ownerId);
}






void MainWindow::onDrawGraffitiAction()
{

    m_scene->setToolType(tt_Graffiti);

}

void MainWindow::onDrawLineAction()
{
    m_scene->setToolType(tt_Line);
}

void MainWindow::onDrawOvalAction()
{
    m_scene->setToolType(tt_Oval);

}

void MainWindow::onDrawRectangleAction()
{
    m_scene->setToolType(tt_Rectangle);

}

void MainWindow::onDrawTriangleAction()
{
    m_scene->setToolType(tt_Triangle);

}

void MainWindow::onUnio()
{
    m_scene->undo();
}

void MainWindow::onClearAll()
{
    if(m_conn) m_conn->clearFigure(-1);
}







