#include "widget.h"
#include "ui_widget.h"
#include "sqlitemanger.h"
#include <QFile>
#include <QDebug>
#define T_log   "id INTEGER PRIMARY KEY, name varchar(32), password varchar(12)"
#define grp     "id INTEGER PRIMARY KEY, name TEXT, gender TEXT, age INTEGER, feature0 blob, feature1 blob, feature2 blob, image0 TEXT, image1 TEXT, image2 TEXT ,audio TEXT, private TEXT"

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

void Widget::on_createBtn_clicked()
{
    SqliteManger db("./database.db");
    //QString sql;
   // db.createTable("T_log",T_log);
    db.createTable("grp",grp);
}
/**
 * @funcname  funcname
 * @brief     插入一行
 * @param     param1
 * @param     param2
 * @return    ret
 */

void Widget::on_pushButton_clicked()
{
    SqliteManger db("./database.db");
#if 0
    db.insertOneRecordForId("T_log","1, 'admin', 'yangwx123_'");
#endif
    QString sql;
    sql = QString("1, 'yangwx', '男', 25, ?, ?, ?, 'NULL', 'NULL', 'NULL', 'NULL', 'NOPRI'");
    QList<QByteArray> baData;
    QByteArray ba;
    QFile file("grp1_pid23_fr_test1_man_35.feat");
    if(file.open(QIODevice::ReadOnly)){
        ba = file.readAll();
        file.close();
    }
    QFile file1("grp1_pid24_fr_test2_women_30.feat");
    QByteArray ba1;
    if(file1.open(QIODevice::ReadOnly)){
        ba1 = file1.readAll();
        file1.close();
    }
    baData.append(ba);
    baData.append(ba1);
    baData.append(ba);
    //db.insertOneRecordForId("grp","5, 'yangwx', 'man', 25, ?, ?, ?, 'NULL', 'NULL', 'NULL', 'NULL', 'NOPRI'",baData,3);
    db.insertOneRecordForId("grp","1, 'yangwx1', 'man', 25, '?', '?', '?', 'NULL', 'NULL', 'NULL', 'NULL', 'NOPRI'",baData,0);
    db.insertOneRecordForId("grp","2, 'yangwx2', 'man', 25, '?', '?', '?', 'NULL', 'NULL', 'NULL', 'NULL', 'NOPRI'",baData,0);
    db.insertOneRecordForId("grp","3, 'yangwx3', 'man', 25, '?', '?', '?', 'NULL', 'NULL', 'NULL', 'NULL', 'NOPRI'",baData,0);
    db.insertOneRecordForId("grp","4, 'yangwx4', 'man', 25, '?', '?', '?', 'NULL', 'NULL', 'NULL', 'NULL', 'NOPRI'",baData,0);
    db.insertOneRecordForId("grp","5, 'yangwx5', 'man', 25, '?', '?', '?', 'NULL', 'NULL', 'NULL', 'NULL', 'NOPRI'",baData,0);
}

void Widget::on_pushButton_2_clicked()
{
    SqliteManger db("./database.db");
    //db.updateOneFieldFromId("T_log","1","name","'yangwx2'"); /*update one field*/
    //db.insertOneRecordForId("T_log","2, 'admin', 'admin'");
    //db.updateOneRecordFromID("T_log","2",)
    //QList<QString> strList,strList2;
    //db.getAllTables(strList);
    //db.getTableInfo(strList.at(0),strList,strList2);
    //db.isTableExists("T_Log");
    //QByteArray ba;
    //QFile file("grp1_pid23_fr_test1_man_35.feat");
    //if(file.open(QIODevice::ReadOnly)){
    //    ba = file.readAll();
      //  file.close();
//    }
    //db.updateOneFieldFromId("grp", "1", "feature0",ba);
    QList<QByteArray> baData;
    QByteArray ba;
    QFile file("grp1_pid23_fr_test1_man_35.feat");
    if(file.open(QIODevice::ReadOnly)){
        ba = file.readAll();
        file.close();
    }
    QFile file1("grp1_pid24_fr_test2_women_30.feat");
    QByteArray ba1;
    if(file1.open(QIODevice::ReadOnly)){
        ba1 = file1.readAll();
        file1.close();
    }
    baData.append(ba);
    baData.append(ba1);
    baData.append(ba1);
    db.updateAllblobsFromId("grp", "3", baData);
    //db.updateOneFieldFromId("grp", "1", "feature0",ba1);
}
/**
 * @funcname  funcname
 * @brief     add col
 * @param     param1
 * @param     param2
 * @return    ret
 */

void Widget::on_pushButton_3_clicked()
{
    SqliteManger db("./database.db");

    QList<QString> strList,strList2;
    db.getAllTables(strList);
    db.addColForTable(strList.at(0),"gender","varchar(10)");
    db.getTableInfo(strList.at(0),strList,strList2);
    db.isTableExists("T_Log");

}
/*delete col*/
void Widget::on_pushButton_4_clicked()
{
    SqliteManger db("./database.db");

    QList<QString> strList,strList2;
    db.getAllTables(strList);
    db.deleteColFromTable(strList.at(0),"gender");
    db.getTableInfo(strList.at(0),strList,strList2);
    db.isTableExists("T_Log");

}
/*query*/
void Widget::on_pushButton_5_clicked()
{
    SqliteManger db("./database.db");
    QList<QString> strList,strList2;
   // db.getAllTables(strList);
    //db.queryOneFieldFromId("grp","1","feature0");
    QList<QByteArray> ba = db.queryAllBlobFieldFromId("grp","3");
    //qDebug() << ba.at(0);
    //qDebug() << ba.at(1);
    db.getMaxIdFromTable("grp");

}
/*delete one record*/
void Widget::on_pushButton_6_clicked()
{
    SqliteManger db("./database.db");
    //db.deleteOneRecord("grp","1");
    //db.clearTableRecord("grp");
    db.deleteRecords("grp","id >1 and id < 5");
}
/* delete database */
void Widget::on_pushButton_7_clicked()
{
    SqliteManger db("./database.db");
    db.deleteDatabase();
}
