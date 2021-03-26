#ifndef STOCKWIDGET_H
#define STOCKWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>

class StockWidget : public QWidget
{
    Q_OBJECT

public:
    StockWidget(QWidget *parent = nullptr);
    ~StockWidget();


private:
    QTableView * table_view;
    QStandardItemModel * item_model;

};
#endif // STOCKWIDGET_H
