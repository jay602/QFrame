#include "loginwidget.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
   m_layOut= new QFormLayout(this);

   m_nameText = new QLineEdit(this);
   m_nameText->setText("jay");

   m_ipText = new QLineEdit(this);
   m_ipText->setText("127.0.0.1");

   m_portText = new QLineEdit("9000", this);

   m_loginBtn = new QPushButton("Join");

   m_layOut->addRow("Name:", m_nameText);//
   m_layOut->addRow("IP:", m_ipText);
   m_layOut->addRow("Port:", m_portText);
   m_layOut->addRow(m_loginBtn);
   m_layOut->setSpacing(10);

   m_layOut->setRowWrapPolicy(QFormLayout::WrapAllRows);
   m_layOut->setLabelAlignment(Qt::AlignLeft);//设置标签的对齐方式
   setLayout(m_layOut);

   connect(m_loginBtn, SIGNAL(clicked()), this, SLOT(startLogin()));
}

void LoginWidget::startLogin()
{
    m_userName = m_nameText->text();
    m_ip = m_ipText->text();
    m_port = m_portText->text().toUInt();

    m_loginBtn->setText("正在登陆....");

    m_nameText->setReadOnly(true);
    m_ipText->setReadOnly(true);
    m_ipText->setReadOnly(true);

    emit login();
}
