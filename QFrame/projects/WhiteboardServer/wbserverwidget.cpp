#include "wbserverwidget.h"
#include "ui_wbserverwidget.h"

WbServerWidget::WbServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WbServerWidget)
{
    ui->setupUi(this);
}

WbServerWidget::~WbServerWidget()
{
    delete ui;
}

void WbServerWidget::on_startListenBtn_clicked()
{
    qint16 port = ui->portSpinBox->value();
    server.listen(QHostAddress::Any, port);
}
