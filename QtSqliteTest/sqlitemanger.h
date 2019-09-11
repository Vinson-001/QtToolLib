#ifndef SQLITEMANGER_H
#define SQLITEMANGER_H

#include <QSqlDatabase>
#include <QMap>

class SqliteManger
{
public:
    SqliteManger(const QString path);
    ~SqliteManger();


    /*create*/
    bool isOpen() const;
    bool createTable(QString tableName, QString sqlFiled);
    bool deleteTable(const QString tableName);
    bool renameTable(const QString oldtableName,const QString newtableName);
    void close();
    /*getTable*/
    bool isTableExists(QString tableName);
    void getTableInfo(QString tableName, QList<QString> &listAllfields, QList<QString> &listFieldsType);
    bool getAllTables(QList<QString> &listAllTabels);
    int getAllBlobfields(const QString tableName, QList<QString> &listBlob );
    int getMaxIdFromTable(const QString tableName);

    /*insert */
    bool insertOneRecordForId(const QString tableName, const QString values); /*inser one record no blob*/
    bool insertOneRecordForId(const QString tableName, const QString values,QList<QByteArray> blobData,quint8 blobCount); /*insert record include blob*/


    /*update */
    bool updateOneFieldFromId(const QString tableName, const QString strId, const QString strField, const QString value); /*update TEXT or int data*/
    bool updateOneFieldFromId(const QString tableName, const QString strId, const QString strField, const QByteArray blobData);/*update blob data*/
    bool updateAllblobsFromId(const QString tableName, const QString strId, QList<QByteArray> blobData);/*update all blob data*/

    /*query*/
    QString queryOneFieldFromId(const QString tableName, const QString strId, const QString strField);  /*query one field TEXT or int*/
    QByteArray queryOneBlobFieldFromId(const QString tableName, const QString strId, const QString strField);/*query one blob data*/
    QList<QByteArray> queryAllBlobFieldFromId(const QString tableName, const QString strId);


    /*expand*/
    void addColForTable(const QString tableName,const QString colName, const QString colType);
    void deleteColFromTable(const QString tableName,const QString colName);

    bool beginTransaction();
    bool commitTransaction();

private:
    //void getAllFields(QString tableName, QList<QString> &fields);


private:
    QSqlDatabase m_db;
    QString m_dbFilePath;
};

#endif // SQLITEMANGER_H
