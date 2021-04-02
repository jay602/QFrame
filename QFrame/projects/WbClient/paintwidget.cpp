#include "paintwidget.h"
#include <QLabel>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(320, 640);
    QLabel *label = new QLabel("2222", this);
}
