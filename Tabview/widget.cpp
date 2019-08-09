#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    table = NULL;
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_create_clicked()
{
    QList<QString> strListText;
    strListText << tr("序号") << tr("设备ID") << tr("设备名称") << tr("设备序列号") << tr("私有数据") << tr("IP地址") << tr("状态");
    table = new  MyTableView(this);
    //table->setParent(this);
    table->resize(600,400);
    table->move(100,100);
    table->show();
}

void Widget::on_add_clicked()
{
    if(table == NULL)
        return;
    QList<QString> strListText;
    strListText << tr("001") << tr("dev1") << tr("sn1") << tr("private") << tr("0.0.0.0") << tr("在线");
    table->doTable("add",strListText);
}

void Widget::on_remove_clicked()
{
    if(table == NULL)
        return;
    QList<QString> strListText;
    table->doTable("remove",strListText);
}

void Widget::on_removeAll_clicked()
{
    if(table == NULL)
        return;
    QList<QString> strListText;
    table->doTable("removeAll",strListText);
}

void Widget::on_getValue_clicked()
{
    if(table == NULL)
        return;
    table->getValueFromRowAndCol(1,2);
}
