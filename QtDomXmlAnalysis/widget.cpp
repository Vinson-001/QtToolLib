#include "widget.h"
#include "ui_widget.h"
#include "domxmlanalysisbase.h"
#include "domxmlanalysisforudp.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_create_clicked()
{
    DomXmlAnalysisForRegister xml("./http.xml");
}

void Widget::on_add_clicked()
{
    DomXmlAnalysisForRegister xml("./http.xml");
    QList<QString> strlist;
    strlist << "dev1" << "T1031" << "Sn1" << "00-0c-00-01-14-3d" << "private" <<"0.0.0.0" << "在线" \
            << "0.0.0.0" << "/mnt/path" << "/mnt/path" << "5555" << "5556";
    xml.doXml("add","001",strlist);
}

void Widget::on_delete_2_clicked()
{
    DomXmlAnalysisForRegister xml("./http.xml");
    QList<QString> strlist;
    strlist << " ";
    xml.doXml("delete","001",strlist);
}

void Widget::on_update_clicked()
{
    DomXmlAnalysisForRegister xml("./http.xml");
    QList<QString> strlist;
    strlist << "dev1" << "T1031" << "Sn1" << "00-0c-00-01-14-3d" << "private" <<"1.0.0.0" << "在线" \
            << "0.0.0.0" << "/mnt/path" << "/mnt/path" << "5555" << "5556";
    xml.doXml("update","001",strlist);
}

void Widget::on_read_clicked()
{
#if 1
    DomXmlAnalysisForRegister xml("./http.xml");
    QList<QString> strlist;
    QList<QString> ElementList;
    xml.readDomXml("001",strlist);
    qDebug() << "strlist" << strlist;
    QMap<QString,QList<QString>> map;
    xml.readDomXmlAll(map);
    qDebug() << map;

    xml.getDevInfo("001",ElementList);
    qDebug() << "strlist" << ElementList;

    xml.getSysSetInfo("001",ElementList);
    qDebug() << "ElementList" << ElementList;
#endif
#if 0
    DomXmlAnalysisForUnRegister xml("./temp.xml");
    QList<QString> strlist;
    xml.readDomXmlTemp(strlist);
    qDebug() << strlist;
#endif
}
