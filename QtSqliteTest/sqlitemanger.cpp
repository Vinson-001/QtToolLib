#include "sqlitemanger.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QFile>
/**
 * @funcname  SqliteManger
 * @brief     构造函数
 * @param     path 数据库存放路径
 * @return    no
 */

SqliteManger::SqliteManger(const QString path):
    m_dbFilePath(path)
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))  {
        m_db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else  {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
    }
    m_db.setDatabaseName(m_dbFilePath);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }


}

SqliteManger::~SqliteManger()
{
    close();
}




/**
 * @funcname  isOpen
 * @brief     判断数据库是否打开
 * @return    true isOpen
 */
bool SqliteManger::isOpen() const
{
    return m_db.isOpen();
}

/**
 * @funcname  createTable
 * @brief     创建数据库表
 * @param     tableName table name
 * @param     sqlField 数据表的字段值id INTEGER PRIMARY KEY, name TEXT
 * @return    true is suucess
 */

bool SqliteManger::createTable(QString tableName, QString sqlField)
{
    /*("CREATE TABLE people(id INTEGER PRIMARY KEY, name TEXT);");*/

    bool success = false;
    QString createTable = "CREATE TABLE IF NOT EXISTS ";
    QString sql;
    sql = QString("%1 (%2);").arg(tableName).arg(sqlField);
    sql = createTable + sql;
    qDebug() << sql;

    QSqlQuery query;
    query.prepare(sql);

    if (!query.exec())
    {
        qDebug() << QString("(%1:%2)Couldn't create the table '%3' one might already exist.").arg(__func__).arg(__LINE__).arg(tableName);
        success = false;
    }
    else {
        success = true;
    }

    return success;
}
/**
 * @funcname  deleteTable
 * @brief     删除数据表
 * @param     tableName 要删除的表名
 * @param     param2
 * @return    ret
 */

bool SqliteManger::deleteTable(const QString tableName)
{
    QString sql;
    QSqlQuery sqlQuery;
    sql = QString("DROP table IF EXISTS %1").arg(tableName);
    sqlQuery.prepare(sql);
    if(!sqlQuery.exec()){
        qDebug() << QString("(%1:%2) ").arg(__func__).arg(__LINE__) << sqlQuery.lastError();
        return false;
    }
    else {
        qDebug() << QString("(%1:%2) delete table : %3").arg(__func__).arg(__LINE__).arg(tableName);
        return true;
    }
}
/**
 * @funcname  renameTable
 * @brief     重命名数据表的name
 * @param     oldtableName 原来table 名
 * @param     newtableName 新的table名
 * @return    ret
 */

bool SqliteManger::renameTable(const QString oldtableName, const QString newtableName)
{
    QString sql;
    QSqlQuery sqlQuery;
    sql = QString("ALTER table %1 RENAME TO %2").arg(oldtableName).arg(newtableName);
    sqlQuery.prepare(sql);
    if(!sqlQuery.exec()){
        qDebug() << QString("(%1:%2) ").arg(__func__).arg(__LINE__) << sqlQuery.lastError();
        return false;
    }
    else {
        qDebug() << QString("(%1:%2) rename '%3' to '%4' ").arg(__func__).arg(__LINE__).arg(oldtableName).arg(newtableName);
        return true;
    }
}

void SqliteManger::close()
{
    if(isOpen())
        m_db.close();
}
/**
 * @funcname  isTableExists
 * @brief     判断数据表是否存在
 * @param     tableName
 * @param     param2
 * @return    ret
 */

bool SqliteManger::isTableExists(QString tableName)
{
    QString sql;
    QSqlQuery sqlQuery;
    /*select count(*)  from sqlite_master where type='table' and name = 'yourtablename';*/
    sql = QString("SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = '%1';").arg(tableName);
    sqlQuery.prepare(sql);
    if(!sqlQuery.exec()){
        return false;
    }else{
        QSqlRecord sqlRecord = sqlQuery.record();
        int count = sqlRecord.count();
        qDebug() << count;
        return true;
    }

}
/**
 * @funcname  getTablInfo
 * @brief     获取表格各字段
 * @param     tableName
 * @param     listAllfields 表格各字段
 * @return    ret
 */

void SqliteManger::getTableInfo(QString tableName, QList<QString> &listAllfields, QList<QString> &listFieldsType)
{
    /*PRAGMA  table_info(t_cmpt_cp)*/
    QString sql;
    sql = QString("PRAGMA table_info(%1)").arg(tableName);
    QSqlQuery queryTable;
    queryTable.prepare(sql);
    if(!queryTable.exec()){
        qDebug() << QString("(%1:%2) ").arg(__func__).arg(__LINE__) << queryTable.lastError();
    }
    else {
        listAllfields.clear();
        listFieldsType.clear();
        while(queryTable.next()){

            listAllfields.append(queryTable.value(1).toString());   /*value(0) cid | value(1) field | value(2) data type*/
            listFieldsType.append(queryTable.value(2).toString());
        }

    }

}
/**
 * @funcname  getAllTables
 * @brief     获取所有的表格名
 * @param     listAllTables 输出表格名
 * @return    ret
 */

bool SqliteManger::getAllTables(QList<QString> &listAllTables)
{
    listAllTables.clear();
    if(isOpen())
    {
        listAllTables = m_db.tables();
        qDebug() << listAllTables;
        return true;
    }
    else {
        return false;
    }
}
/**
 * @funcname  getAllBlobfields
 * @brief     提取所有blob字段名
 * @param     tableName
 * @param     listBlob blob字段名
 * @return    返回blob 个数
 */

int SqliteManger::getAllBlobfields(const QString tableName, QList<QString> &listBlob)
{
    QList<QString> listAllfields,listallfieldType;
    //QString strBlob;
    getTableInfo(tableName,listAllfields,listallfieldType);
    for(int i = 0; i < listallfieldType.count(); i++){
        QString str = listallfieldType.at(i).toUpper();
        if(str == "BLOB"){
            QString strField = listAllfields.at(i);
            listBlob.append(strField);
        }
    }
    return listBlob.count();
}
/**
 * @funcname  getMaxIdFromTable
 * @brief     获取数据表的最大主键值
 * @param     tableName
 * @return    返回数据表的最大主键值
 */

int SqliteManger::getMaxIdFromTable(const QString tableName)
{
    QString sql;
    sql = QString("select max(id) from %1").arg(tableName);
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sql);
    if(sqlQuery.exec()){
        while(sqlQuery.next()){
            int maxid = sqlQuery.value(0).toInt();
            qDebug() << maxid;
            return maxid;
        }
    }
    return -1;
}

/**
 * @funcname  insertOneRecordForId
 * @brief     插入一条记录
 * @param     tableName 要插入的表
 * @param     values 各字段绑定值
 * @return    true is sucess
 */

bool SqliteManger::insertOneRecordForId(const QString tableName, const QString values)
{
    bool success = false;
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) insert failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return false;
    }
    /*
     * sql : INSERT INTO 'T_log' VALUES (maxid+1,value1,value2,...)"
     * sql : INSERT INFO 'tableName' VALUES (values)
     */
    QString sql;
    sql = QString("INSERT INTO %1 VALUES (%2)").arg(tableName).arg(values);
    qDebug() << sql;
    if (!sql.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare(sql);
        //queryAdd.prepare("INSERT INTO people (name) VALUES (:name)");
        //queryAdd.bindValue(":name", name);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << QString("(%1:%2) insert (%3) failed").arg(__func__).arg(__LINE__).arg(values) << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "add person failed: name cannot be empty";
    }

    return success;
}
/**
 * @funcname  insertOneRecordForId
 * @brief     插入一条记录，包含blob数据
 * @param     tableName
 * @param     values (value1,value2, ..., ?, ?,? )
 * @param     blobData blob数据list
 * @param     blobCount (? 个数) blobCount = 0 插入的是普通文本
 * @return    ret
 */

bool SqliteManger::insertOneRecordForId(const QString tableName, const QString values, QList<QByteArray> blobData, quint8 blobCount)
{
    bool success = false;
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) insert failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return false;
    }
    /*
     * sql : INSERT INTO 'T_log' VALUES (maxid+1,value1,value2,...)"
     * sql : INSERT INFO 'tableName' VALUES (values)
     */
    QString sql;
    sql = QString("INSERT INTO %1 VALUES (%2)").arg(tableName).arg(values);
    qDebug() << sql;
    if (!sql.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare(sql);
        //queryAdd.prepare("INSERT INTO people (name) VALUES (:name)");
        //queryAdd.bindValue(":name", name);
        for(int i = 0; i < blobCount; i++){
            queryAdd.addBindValue(blobData.at(i));
        }

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << QString("(%1:%2) insert (%3) failed").arg(__func__).arg(__LINE__).arg(values) << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "add person failed: name cannot be empty";
    }

    return success;
}

/**
 * @funcname  updateOneFieldFromId
 * @brief     根据id更新某一字段的值
 * @param     tableName table name
 * @param     strId id
 * @param     strField 要修改的字段
 * @param     value 修改的值(TEXT or INT)
 * @return    ret
 */

bool SqliteManger::updateOneFieldFromId(const QString tableName, const QString strId, const QString strField, const QString value)
{
    /*"update student set name = :name where id = :id";*/
    /*"update student set strField = 'value' where id = strId";*/
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) update failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return false;
    }
    QString sql ;
    sql = QString("UPDATE %1 set %2  = %3 where id = %4").arg(tableName).arg(strField).arg(value).arg(strId);

    QSqlQuery queryUpdate;
    queryUpdate.prepare(sql);
    //queryUpdate.bindValue(":name", "Qt"); /*:name 表示待定的变量*/
    //queryUpdate.bindValue(":id", 1);
    qDebug() << sql;
    if(!queryUpdate.exec())
    {
        qDebug() << QString("(%1:%2) update (%3) to (%4) failed").arg(__func__).arg(__LINE__).arg(strField).arg(value) << queryUpdate.lastError();
        return false;
    }
    else
    {
        qDebug() << QString("(%1:%2) update (%3) to (%4) ...ok").arg(__func__).arg(__LINE__).arg(strField).arg(value);
        return true;
    }
}
/**
 * @funcname  updateOneFieldFromId
 * @brief     更新blob的数值
 * @param     tableName
 * @param     strId key
 * @param     strField 要更新的字段
 * @param     blobData 要更新的二进制数据
 * @return    ret
 */

bool SqliteManger::updateOneFieldFromId(const QString tableName, const QString strId, const QString strField, const QByteArray blobData)
{
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) update field failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return false;
    }
    /*"update student set name = :name where id = :id";*/
    /*"update student set strField = :strFileld where id = strId";*/
    /*"update student set feature0 = :feature0 where id = 1*/
    QString sql ;
    sql = QString("UPDATE %1 set %2  = :%3 where id = %4").arg(tableName).arg(strField).arg(strField).arg(strId);

    QSqlQuery queryUpdate;
    queryUpdate.prepare(sql);
    //queryUpdate.bindValue(":name", "Qt"); /*:name 表示待定的变量*/
    QString field;
    field = QString(":%1").arg(strField);
    queryUpdate.bindValue(field, blobData);
    qDebug() << sql;
    if(!queryUpdate.exec())
    {
        qDebug() << QString("(%1:%2) update (%3) failed").arg(__func__).arg(__LINE__).arg(strField) << queryUpdate.lastError();
        return false;
    }
    else
    {
        qDebug() << QString("(%1:%2) update (%3) ...ok").arg(__func__).arg(__LINE__).arg(strField);
        return true;
    }
}
/**
 * @funcname  updateAllblobsFromId
 * @brief     更新一条记录的所有的blob数据
 * @param     tableName 表单名
 * @param     strId key
 * @param     blobData 要更新的二进制数据
 * @return    ret
 */

bool SqliteManger::updateAllblobsFromId(const QString tableName, const QString strId, QList<QByteArray> blobData)
{
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) update all blobs failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return false;
    }
    /*1. 获取table 所有字段，提取所有blob*/
    QList<QString> listAllfields,listallfieldType;
    QString sql;
    QString strBlob;
    QList<QString> listBlob;
    getAllBlobfields(tableName,listBlob);
    for(int i = 0; i < listBlob.count(); i++){
        QString strField = listBlob.at(i);
        strBlob += QString("%1 = :%2 ,").arg(strField).arg(strField);
    }
    if(blobData.count() != listBlob.count()){
        qDebug() << QString("(%1:%2) update all blob failed: blobData is not right").arg(__func__).arg(__LINE__);
        return false;
    }
    /*feature0 = :feature0, feature1 = :feature1 , feature2 = :feature2, */
    int index = strBlob.lastIndexOf(",");   /*去掉逗号*/
    strBlob =strBlob.mid(0,index);

    /*"update student set feature0 = :feature0, feature1 = :feature1 where id = strId";*/
    sql = QString("UPDATE %1 set %2 where id = %3").arg(tableName).arg(strBlob).arg(strId);
    //qDebug() << sql;
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sql);
    qDebug() << listBlob;
    for(int i = 0; i < listBlob.count(); i++){
        QString field;
        field = QString(":%1").arg(listBlob.at(i));
        sqlQuery.bindValue(field,blobData.at(i));
    }
    if(!sqlQuery.exec())
    {
        qDebug() << QString("(%1:%2) update all blob failed").arg(__func__).arg(__LINE__) << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << QString("(%1:%2) update all blob ...ok").arg(__func__).arg(__LINE__);
        return true;
    }

}
/**
 * @funcname  queryOneFieldFromId
 * @brief     根据id 查询特定的字段
 * @param     tableName
 * @param     strId key
 * @param     strField 要查询的字段
 * @return    返回要查询的字段值
 */

QString SqliteManger::queryOneFieldFromId(const QString tableName, const QString strId, const QString strField)
{
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) update query failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return "";
    }
    QString sql;
    QSqlQuery sqlQuery;
    sql = QString("SELECT %1 FROM %2 WHERE id = %3").arg(strField).arg(tableName).arg(strId);
    qDebug() << sql;
    sqlQuery.prepare(sql);
    if(sqlQuery.exec()){
        while(sqlQuery.next()) {
            QString value = sqlQuery.value(0).toString();
            qDebug() << value;
            return value;
        }

    }
    else {
        qDebug() << QString("(%1:%2) update all blob failed").arg(__func__).arg(__LINE__) << sqlQuery.lastError();
        return "";
    }
}
/**
 * @funcname  queryOneBlobFieldFromId
 * @brief     查询的blob字段值
 * @param     tableName
 * @param     strId key
 * @param     strField blob字段
 * @return    返回要查询的blob字段值(二进制数据)
 */

QByteArray SqliteManger::queryOneBlobFieldFromId(const QString tableName, const QString strId, const QString strField)
{
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) update query failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return "";
    }
    QString sql;
    QSqlQuery sqlQuery;
    sql = QString("SELECT %1 FROM %2 WHERE id = %3").arg(strField).arg(tableName).arg(strId);
    qDebug() << sql;
    sqlQuery.prepare(sql);
    if(sqlQuery.exec()){
        while(sqlQuery.next()) {
            QByteArray value = sqlQuery.value(0).toByteArray();
            //qDebug() << value;
            return value;
        }

    }
    else {
        qDebug() << QString("(%1:%2) update all blob failed").arg(__func__).arg(__LINE__) << sqlQuery.lastError();
        return "";
    }
}
/**
 * @funcname  queryAllBlobFieldFromId
 * @brief     查询所有的blob字段
 * @param     tableName
 * @param     strId key
 * @return    返回查询所有的blob字段二进制数据
 */

QList<QByteArray> SqliteManger::queryAllBlobFieldFromId(const QString tableName, const QString strId)
{
    QList<QByteArray> baValue;
    baValue.clear();
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) query failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return baValue;
    }
    /*1. 获取所有blob字段*/
    QList<QString> listblob;
    int count = getAllBlobfields(tableName, listblob);
    QString sql;
    QString strBlob;
    for(int i = 0; i < count; i++){
        strBlob += QString("%1, ").arg(listblob.at(i));
    }
    int index = strBlob.lastIndexOf(",");   /*去掉逗号*/
    strBlob =strBlob.mid(0,index);
    sql = QString("SELECT %1 FROM %2 WHERE id = %3").arg(strBlob).arg(tableName).arg(strId);
    qDebug() << sql;
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sql);
    if(sqlQuery.exec()){
        while(sqlQuery.next()){
            for(int i = 0; i < listblob.count(); i++){
                QByteArray value = sqlQuery.value(i).toByteArray();
                baValue.append(value);
            }
        }
        return baValue;
    }
    else {
        baValue.clear();
        return baValue;
    }
}
/**
 * @funcname  deleteOneRecord
 * @brief     删除一条记录
 * @param     tableName
 * @param     strId key
 * @return    no
 */

void SqliteManger::deleteOneRecord(const QString tableName, const QString strId)
{
    /*delete from <table_name> where <f1> = <value>*/
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) delete failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return;
    }
    QString sql;
    sql = QString("delete from %1 where id = %2").arg(tableName).arg(strId);
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sql);
    if(sqlQuery.exec()){
        qDebug() << QString("(%1:%2) delete ok").arg(__func__).arg(__LINE__);
    }
    else {
        qDebug() << QString("(%1:%2) delete failed").arg(__func__).arg(__LINE__) << sqlQuery.lastError();
    }
}
/**
 * @funcname  deleteRecords
 * @brief     根据限定条件删除记录
 * @param     tableName
 * @param     exp 限定条件
 * @return    ret
 */

void SqliteManger::deleteRecords(const QString tableName, const QString exp)
{
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) delete failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return;
    }
    QString sql;
    sql = QString("delete from %1 where %2").arg(tableName).arg(exp);
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sql);
    if(sqlQuery.exec()){
        qDebug() << QString("(%1:%2) delete ok").arg(__func__).arg(__LINE__);
    }
    else {
        qDebug() << QString("(%1:%2) delete failed").arg(__func__).arg(__LINE__) << sqlQuery.lastError();
    }
}
/**
 * @funcname  clearTableRecord
 * @brief     清空所有记录
 * @param     tableName
 * @param     param2
 * @return    ret
 */

void SqliteManger::clearTableRecord(const QString tableName)
{
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) clear failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return;
    }
    QString sql;
    sql = QString("delete from %1").arg(tableName);
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sql);
    if(sqlQuery.exec()){
        qDebug() << QString("(%1:%2) clear %3 table ..ok").arg(__func__).arg(__LINE__).arg(tableName);
    }
    else {
        qDebug() << QString("(%1:%2) clear failed").arg(__func__).arg(__LINE__) << sqlQuery.lastError();
    }
}
/**
 * @funcname  addColForTable
 * @brief     为数据表新增一列
 * @param     tableName
 * @param     colName 新增字段名称
 * @param     colType 新增字段名称类型
 * @return    ret
 */

void SqliteManger::addColForTable(const QString tableName, const QString colName, const QString colType)
{
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) add col failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return;
    }
    /*ALTER  TABLE   table-name ADD COLUMN  column-name column-type*/
    QString sql;
    sql = QString("ALTER TABLE %1 ADD COLUMN %2 %3").arg(tableName).arg(colName).arg(colType);
    qDebug() << sql;
    QSqlQuery addColQuery;
    addColQuery.prepare(sql);
    if(!addColQuery.exec()){
        qDebug() << QString("(%1:%2) add col (%3:%4)failed ").arg(__func__).arg(__LINE__).arg(colName).arg(colType) << addColQuery.lastError();
        return;
    }else {
        qDebug() << QString("(%1:%2) add col (%3:4) ...ok").arg(__func__).arg(__LINE__).arg(colName).arg(colType);
    }

}
/**
 * @funcname  deleteColFromTable
 * @brief     从数据表里删除一列
 * @param     tableName
 * @param     colName 要删除的列
 * @return    ret
 */

void SqliteManger::deleteColFromTable(const QString tableName, const QString colName)
{
    if(!isTableExists(tableName)){
        qDebug() << QString("(%1:%2) celete col failed: %3 table is not exists").arg(__func__).arg(__LINE__).arg(tableName);
        return ;
    }
    QList<QString> listAllFields,listAllFieldType;
    /*1.获取整个表的所有字段*/
    getTableInfo(tableName,listAllFields,listAllFieldType);
    listAllFields.removeOne(colName);
    /*2. 创建一个新的表*/
    QString sql;
    QSqlQuery sqlQuery;
    /*create table B as select y,z from A*/
    QString str;
    for(int i = 0; i < listAllFields.count(); i++){
        str += QString("%1,").arg(listAllFields.at(i)); /*select fields*/
    }
    int index = str.lastIndexOf(",");
    str =str.mid(0,index);
    sql = QString("CREATE TABLE temptable AS SELECT %1 FROM %2 ").arg(str).arg(tableName);
    //qDebug() << sql;
    sqlQuery.prepare(sql);
    if(!sqlQuery.exec()){
        qDebug() << sqlQuery.lastError();
        return;
    }else {
        /*3. 删除原来的表*/
        bool isOk = deleteTable(tableName);
        if(!isOk){
            return;
        }
        else {
            /*4. 重命名新建的表*/
            bool isOk = renameTable("temptable",tableName);
            if(!isOk){
                return;
            }
            else {
                qDebug() << QString("(%1:%2) delete col '%3' ..ok ").arg(__func__).arg(__LINE__).arg(colName);
            }
        }
    }
}
/**
 * @funcname  deleteDatabase
 * @brief     删除整个数据库表
 * @param     param1
 * @param     param2
 * @return    ret
 */

void SqliteManger::deleteDatabase()
{
    QFile file(m_dbFilePath);
    if(file.exists()){
        file.remove();
    }

}
/**
 * @funcname  beginTransaction
 * @brief     开启事物系统
 *              用于批量插入数据
 *              transaction
 *              sql.exec
 *              commit
 * @return    ret
 */

bool SqliteManger::beginTransaction()
{
    if(isOpen())
        return m_db.transaction();
    else {
        qDebug() << QString("(%1:%2) database is not open ").arg(__func__).arg(__LINE__);
        return false;
    }
}
/**
 * @funcname  commitTransaction
 * @brief     提交所有记录
 * @return    ret
 */

bool SqliteManger::commitTransaction()
{
    if(isOpen())
        return m_db.commit();
    else {
        qDebug() << QString("(%1:%2) database is not open ").arg(__func__).arg(__LINE__);
        return false;
    }
}
