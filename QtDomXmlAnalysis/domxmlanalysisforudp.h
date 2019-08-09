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
class DomXmlAnalysisForUnRegister : public DomXmlAnalysisBase
{
public:
    DomXmlAnalysisForUnRegister(QString strfilename);
    DomXmlAnalysisForUnRegister();


private:

};

#endif // DOMXMLANALYSISFORUDP_H
