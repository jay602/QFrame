#include "paintwidget.h"
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QPen>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
//    setFixedSize(320, 640);
//    QLabel *label = new QLabel("2222", this);
   QGraphicsScene *scene = new QGraphicsScene(this);  //场景

   QPen pen;   // 定义一个画笔，设置画笔颜色和宽度
   pen.setColor(QColor(0, 160, 230));
   pen.setWidth(10);

   QGraphicsRectItem *m_rectItem = new QGraphicsRectItem();   // 定义一个矩形图元
   m_rectItem->setRect(10, 10, 100, 80);
   m_rectItem->setPen(pen);
   m_rectItem->setBrush(QBrush(QColor(255, 0, 255)));
   m_rectItem->setFlag(QGraphicsItem::ItemIsMovable);
   scene->addItem(m_rectItem);

   QGraphicsView *view = new QGraphicsView(this); //视图
   view->setScene(scene);  //视图关联场景
}
