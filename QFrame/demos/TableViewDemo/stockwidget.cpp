#include "stockwidget.h"
#include <QStandardItem>
#include <QHBoxLayout>
#include <QHeaderView>

StockWidget::StockWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(600, 300);
    QHBoxLayout* alayout = new QHBoxLayout(this);

    table_view = new QTableView;
    alayout->addWidget(table_view);
    item_model = new QStandardItemModel(4, 4);
    table_view->setModel(item_model);

    //    table_view->horizontalHeader()->hide();                       // 隐藏水平表头
    //    table_view->verticalHeader()->hide();                         // 隐藏垂直表头
    table_view->horizontalHeader()->setStretchLastSection(true);  // 伸缩最后一列


    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
           item_model->setItem(i, j, new QStandardItem(QString("%1").arg(i + j)));

    item_model->appendRow(new QStandardItem("HELLO"));

     QList<QStandardItem *> lsi;
     lsi.append(new QStandardItem("HELLO"));
     lsi.append(new QStandardItem("WORLD"));
     lsi.append(new QStandardItem("你好"));
     item_model->appendRow(lsi);   // 追加行，相应列均有数据,即前三列有数据

     table_view->setShowGrid(false);                               // 隐藏网格线
     table_view->setFocusPolicy(Qt::NoFocus);                      // 去除当前Cell周边虚线框
     table_view->setAlternatingRowColors(true);                    // 开启隔行异色

}

StockWidget::~StockWidget()
{
}

