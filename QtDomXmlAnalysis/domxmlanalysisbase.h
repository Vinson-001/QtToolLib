/**
* @filename      domxmlanalysisbase.h
* @brief         dom 解析xml
* @author        Wxyang
* @date          2019-08-07
*/
#ifndef DOMXMLANALYSISBASE_H
#define DOMXMLANALYSISBASE_H
#include <QtXml>
#include <QDebug>

class DomXmlAnalysisBase
{
public:
    DomXmlAnalysisBase();
    DomXmlAnalysisBase(QString strfilename);
private:
    QString m_strFileName;

private:

    void addDomXml(QString strId,QList<QString> strListText);
    void deleteDomXml(QString strId);
    void updateDomXml(QString strId,QList<QString> strListText);
    void getDomXmlstrIdList(QList<QString> &strIdList);
public:
    bool createDomXml(QString strRoot);
    void doXml(QString operate,QString strId,QList<QString> strListText);
    bool isDomXmlstrIdexist(QString strId);
    void readDomXml(QString strId, QList<QString> &strListText);
    void readDomXmlTemp(QList<QString> &strListText);
    void readDomXmlAll(QMap<QString,QList<QString>> &mapElement);
};

#endif // DOMXMLANALYSISBASE_H
