#ifndef PAINTERSCENE_H
#define PAINTERSCENE_H
#include<QGraphicsScene>
#include<QGraphicsSceneMouseEvent>
#include"shapes.h"
#include<QPen>
#include<QColor>
#include<list>
class PainterScene:public QGraphicsScene
{
    Q_OBJECT
public:
    PainterScene(QObject *parent = 0);
    ~PainterScene();
    void setToolType(int type);

    void setUserId(int id) { m_id = id; }
    void undo();

    void onFigureAdded(const QJsonObject &figure);
    void onFigureDeleted(int id);
    void onFigureCleared(int ownerId);
signals:
    void addFigureReq(const QJsonObject &figure);
    void deletFigureReq(int id);
    void clearFigureReq(int ownerId);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
    int m_toolTyep;
    int m_id;
    Shapes *m_currentShape;

    std::list<Shapes *> m_shapes;

};

#endif // PAINTERSCENE_H
