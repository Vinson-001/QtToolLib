/**
* @fileName      DomXmlAnalysisforUdp.h
* @brief         用于UDP传输 xml 解析
* @author        Wxyang
* @date          2019-08-07
*/
#ifndef DOMXMLANALYSISFORUDP_H
#define DOMXMLANALYSISFORUDP_H
#include "./domxmlanalysisbase.h"
#include <QFileInfo>
class DomXmlAnalysisForUdp : public DomXmlAnalysisBase
{
public:
    DomXmlAnalysisForUdp(QString strfilename);
    DomXmlAnalysisForUdp();

public:
    void readDomXmlTemp(QList<QString> &strListText);
private:
    QString m_strTempFileName;
};

#endif // DOMXMLANALYSISFORUDP_H
