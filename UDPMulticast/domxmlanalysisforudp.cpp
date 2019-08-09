#include "domxmlanalysisforudp.h"
/**
* @fileName      DomXmlAnalysisForUnRegister
* @brief         未注册表单
* @author        Wxyang
* @date          2019-08-09
*/
DomXmlAnalysisForUnRegister::DomXmlAnalysisForUnRegister(QString strfilename)
    :DomXmlAnalysisBase (strfilename)
{

    QFileInfo file(strfilename);
    bool isflag = file.exists();
    if(!isflag)
    {
        createDomXml("Devices");
    }
    initUnRegisterXml();
}
DomXmlAnalysisForUnRegister::DomXmlAnalysisForUnRegister()
{

}

void DomXmlAnalysisForUnRegister::initUnRegisterXml()
{
    QString strRootElement = "Device";
    QList<QString> strElementListText;
    strElementListText << "Name" << "Model" << "Sn" << "Mac" << "Private" << "DevIpAddr" << "Status";
    setXmlElement(strRootElement,strElementListText);
}




/**
* @fileName      DomXmlAnalysisForRegister
* @brief         注册表单
* @author        Wxyang
* @date          2019-08-09
*/
DomXmlAnalysisForRegister::DomXmlAnalysisForRegister(QString strfilename)
    :DomXmlAnalysisBase (strfilename),
     m_strFileName(strfilename)
{

    QFileInfo file(strfilename);
    if(!file.exists())
    {
        createDomXml("Devices");
    }
    initRegisterXml();
}
DomXmlAnalysisForRegister::DomXmlAnalysisForRegister()
{

}

/**
 * @funcname  initRegisterXml
 * @brief     插入一个节点
 * @return    void
 * @example
 *           <Devices>
 *               <Device Id="1">
 *                   <Name>test_dev</Name>
 *                   <Model>T-1031</Model>
 *                   <Sn>0123456789</Sn>
 *                   <Mac>00-0c-00-01-14-3d</Mac>
 *                   <Private>this is prvate data </Private>
 *                   <DevIpAddr>193.169.3.111</DevIpAddr>
 *                   <Status>在线</Status>
 *                   <FtpIpAddr>0.0.0.0</FtpIpAddr>
 *                   <FtpCapturePath>/mnt/path</FtpCapturePath>
 *                   <FtpRecogPath>/mnt/path</FtpRecogPath>
 *                   <RtcEventPort>5555</RtcEventPort>
 *                   <warnport>5555</warnport>
 *               </Device>
 *               ...
 *           </Devices>
 */

void DomXmlAnalysisForRegister::initRegisterXml()
{
    QString strRootElement = "Device";
    QList<QString> strElementListText;
    strElementListText << "Name" << "Model" << "Sn" << "Mac" << "Private" << "DevIpAddr" << "Status"\
                           << "FtpIpAddr" << "FtpCapturePath" << "FtpRecogPath" << "RtcEventPort" << "Warnport";
    setXmlElement(strRootElement,strElementListText);
}
/**
 * @funcname  getDevInfo
 * @brief     获取设备信息,用于显示
 * @param     strId
 * @param     strDevInfoList output
 * @return    no
 */

bool DomXmlAnalysisForRegister::getDevInfo(QString strId,QList<QString> &strDevInfoList)
{
    QList<QString> strListText;
    readDomXml(strId,strListText);
    if(strListText.count() != 12)
        return false;
    strDevInfoList.clear();
    strListText.removeLast();
    strListText.removeLast();
    strListText.removeLast();
    strListText.removeLast();
    strListText.removeLast();
    strDevInfoList = strListText;
    return true;
}
/**
 * @funcname  getDevInfo
 * @brief     获取设置信息
 * @param     strId
 * @param     strSysInfoList output
 * @return    no
 */
bool DomXmlAnalysisForRegister::getSysSetInfo(QString strId, QList<QString> &strSysInfoList)
{
    QList<QString> strListText;
    readDomXml(strId,strListText);
    if(strListText.count() != 12)
        return false;
    strSysInfoList.clear();
    strSysInfoList.append(strListText.at(7));
    strSysInfoList.append(strListText.at(8));
    strSysInfoList.append(strListText.at(9));
    strSysInfoList.append(strListText.at(10));
    strSysInfoList.append(strListText.at(11));
    return true;
}
