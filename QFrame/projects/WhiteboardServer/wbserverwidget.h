#ifndef WBSERVERWIDGET_H
#define WBSERVERWIDGET_H

#include <QWidget>
#include "whiteboardserver.h"

namespace Ui {
class WbServerWidget;
}

class WbServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WbServerWidget(QWidget *parent = nullptr);
    ~WbServerWidget();

private slots:
    void on_startListenBtn_clicked();

private:
    Ui::WbServerWidget *ui;

private:
    WhiteboardServer server;
};

#endif // WBSERVERWIDGET_H
