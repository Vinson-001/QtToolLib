#include "domxmlanalysisforudp.h"

DomXmlAnalysisForUnRegister::DomXmlAnalysisForUnRegister(QString strfilename)
    :DomXmlAnalysisBase (strfilename)
{

    QFileInfo file(strfilename);
    if(!file.exists())
    {
        createDomXml("Devices");
    }
}
DomXmlAnalysisForUnRegister::DomXmlAnalysisForUnRegister()
{

}



