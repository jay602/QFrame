#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"painterscene.h"
#include"WhiteBoardConnection.h"
#include<QCloseEvent>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

protected slots:

    void onDrawLineAction();
    void onDrawRectangleAction();
    void onDrawOvalAction();
    void onDrawTriangleAction();
    void onDrawGraffitiAction();
    void onUnio();
    void onClearAll();

    void onJoinButtonClicked();

    void onJoined(QString name,int id);
    void onUserLeft(QString name,int id);

    void onFigureAdded(const QJsonObject &figure);
    void onFigureDeleted(int id);
    void onFigureCleared(int ownerId);
    void onErrorOccured(const QString &desc);

    void onAddFigureReq(const QJsonObject &figure);
    void onDeleteFigureReq(int id);
    void onClearFigureReq(int ownerId);
protected:
    void prepareJoinUI();
    void preparePainterUI();

public:
    PainterScene *m_scene;
    WhiteBoardConnection *m_conn;
    QLineEdit *m_nameEdit;
    QToolBar *m_toolBar;

};


#endif // MAINWINDOW_H
