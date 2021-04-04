#ifndef PAINTERVIEW_H
#define PAINTERVIEW_H

#include<QGraphicsView>
#include<QtWidgets>

class  PainterView : public QGraphicsView
{
public:
     PainterView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
     void resizeEvent(QResizeEvent *event) override;
};

#endif // PAINTERVIEW_H
