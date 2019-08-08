#include "domxmlanalysisforudp.h"

DomXmlAnalysisForUdp::DomXmlAnalysisForUdp(QString strfilename)
    :DomXmlAnalysisBase (strfilename)
{
    m_strTempFileName = strfilename;
    QFileInfo file(strfilename);
    if(!file.exists())
    {
        createDomXml("Devices");
    }
}
DomXmlAnalysisForUdp::DomXmlAnalysisForUdp()
{

}
/**
 * @funcname  readDomXmlTemp
 * @brief     解析接收udp临时数据
 * @param     strListText output
 * @return    no
 */

void DomXmlAnalysisForUdp::readDomXmlTemp(QList<QString> &strListText)
{
    QFile file(m_strTempFileName);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement docElem = doc.documentElement();            /*device*/
    QDomNode n = docElem.firstChild();
    if(strListText.isEmpty())
        strListText.clear();
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();

             strListText.append(e.text());
        }
        n = n.nextSibling();
    }
    //qDebug() << m_strLisTemp ;
}
