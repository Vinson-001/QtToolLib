#include "domxmlanalysisbase.h"

DomXmlAnalysisBase::DomXmlAnalysisBase(QString strfilename)
{
    m_strFileName = strfilename;

}
/**
 * @funtion   createDomXml
 * @brief     create xml
 * @param     strRoot Root 节点
 * @return    ret
 */

bool DomXmlAnalysisBase::createDomXml(QString strRoot)
{
    QDomDocument doc;

     // 添加处理指令即XML说明
     QDomProcessingInstruction instruction;
     instruction = doc.createProcessingInstruction("xml",
                                                "version=\"1.0\" encoding=\"UTF-8\"");
     doc.appendChild(instruction);

     // 添加根元素
     QDomElement root = doc.createElement(strRoot);
     doc.appendChild(root);
     //
     QFile file(m_strFileName);
     if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
         return false;
     }
     QTextStream out(&file);

     // 将文档保存到文件，4为子元素缩进字符数
     doc.save(out, 4);
     file.close();
     return true;
}
/**
 * @funcname  doXml
 * @brief     [add | delete | update] xml
 * @param     operate
 * @param     strId id
 * @param     strListText
 * @return    no
 */

void DomXmlAnalysisBase::doXml(QString operate, QString strId, QList<QString> strListText)
{
    if(operate == "add"){
        bool flag = false;
        flag = isDomXmlstrIdexist(strId);
        if(!flag) {
            addDomXml(strId,strListText);
        }
        else {
            qDebug() << strId << "exist";
            updateDomXml(strId,strListText);
        }
    } else if (operate == "delete") {
        deleteDomXml(strId);
    } else if (operate == "update") {
        updateDomXml(strId,strListText);
    }
}
/**
 * @funcname  isDomXmlstrIdexist
 * @brief     判断strId 是否存在
 * @param     strId
 * @return    bool
 */

bool DomXmlAnalysisBase::isDomXmlstrIdexist(QString strId)
{
    QList<QString> strListId;
    getDomXmlstrIdList(strListId);
    return strListId.contains(strId);
}
/**
 * @funcname  readDomXml
 * @brief     根据id获取其下元素值
 * @param     strId
 * @param     strListText output param
 * @return    n0
 */

void DomXmlAnalysisBase::readDomXml(QString strId, QList<QString> & strListText)
{
    QFile file(m_strFileName);
    if(!strListText.isEmpty())
        strListText.clear();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();

    // 以标签名进行查找
    QDomNodeList list = doc.elementsByTagName("Device");

    for(int i=0; i<list.count(); i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute("Id") == strId)
        {   // 如果元素的“编号”属性值与我们所查的相同

            // 如果是更新操作
            QDomNodeList child = list.at(i).childNodes();

            // 将它子节点的首个子节点（就是文本节点）的内容更新
            for(int j = 0;j < child.count(); j++) {
                QString strText;
                strText = child.at(j).toElement().text();
                strListText.append(strText);
            }
            break;
        }
    }
}
/**
 * @funcname  readDomXmlAll
 * @brief     遍历整张表元素
 * @param     mapElement <id,Element> id
 * @return    no
 */

void DomXmlAnalysisBase::readDomXmlAll(QMap<QString, QList<QString>> &mapElement)
{
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    if(!mapElement.isEmpty())
        mapElement.clear();
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            //ui->listWidget->addItem(e.tagName() + e.attribute("编号"));
            QDomNodeList list = e.childNodes();
            QList<QString> strlistElement;
            //strlistElement.insert(0,e.attribute("Id"));
            QString strId = e.attribute("Id");
            for (int i=0; i<list.count(); i++)
            {
                QDomNode node = list.at(i);
                if(node.isElement()) {
                    strlistElement.insert(i,node.toElement().text());
                }
            }
            //qDebug() << strlist;
            //m_strListListText.append(strlist);
            mapElement.insert(strId,strlistElement);
        }
        n = n.nextSibling();
    }
    //qDebug() << mapElement;
}
/**
 * @funcname  addDomXml
 * @brief     插入一个节点
 * @param     strId 节点
 * @param     strListText <元素值>
 * @return    void
 * @example
 *            <Devices>
 *                  <Device Id="001">
 *                      <Name></Name>
 *                      <Sn></Sn>
 *                      <Private></Private>
 *                      <IpAddr></IpAddr>
 *                      <Status></Status>
 *                  </Device>
 *            </Devices>
 */

void DomXmlAnalysisBase::addDomXml(QString strId, QList<QString> strListText)
{
    if(strListText.isEmpty())
        return;
    QString strName = strListText.at(0);
    QString strSn = strListText.at(1);
    QString strPrivate = strListText.at(2);
    QString strIp = strListText.at(3);
    QString strStaus = strListText.at(4);
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qDebug() << "(AddElementToXmlForUdp:line358)doc.setContent failed";
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomElement device = doc.createElement("Device");
    QDomAttr id = doc.createAttribute("Id");
    QDomElement name = doc.createElement("Name");
    QDomElement sn = doc.createElement("Sn");
    QDomElement prviate = doc.createElement("Prviate");
    QDomElement ip = doc.createElement("IpAddr");
    QDomElement status = doc.createElement("Status");
    QDomText text;

    // 我们获得了最后一个孩子结点的编号，然后加1，便是新的编号
    //QString num = root.lastChild().toElement().attribute("Id");
    //int count = num.toInt() +1;
    //id.setValue(QString::number(count));
    id.setValue(strId);
    device.setAttributeNode(id);
    text = doc.createTextNode(strName);
    name.appendChild(text);

    text = doc.createTextNode(strSn);
    sn.appendChild(text);

    text = doc.createTextNode(strPrivate);
    prviate.appendChild(text);
    text = doc.createTextNode(strIp);
    ip.appendChild(text);
    text = doc.createTextNode(strStaus);
    status.appendChild(text);


    device.appendChild(name);
    device.appendChild(sn);
    device.appendChild(prviate);
    device.appendChild(ip);
    device.appendChild(status);
    root.appendChild(device);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return ;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
    qDebug() << "add Element xml";
}
/**
 * @funcname  deleteDomXml
 * @brief     delete a Element
 * @param     strId
 * @return    no
 */

void DomXmlAnalysisBase::deleteDomXml(QString strId)
{
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();

    // 以标签名进行查找
    QDomNodeList list = doc.elementsByTagName("Device");
    for(int i=0; i<list.count(); i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute("Id") == strId)                  //如果相等测更新或者删除元素值
        {   // 如果元素的“编号”属性值与我们所查的相同

            // 如果是删除操作
            QDomElement root = doc.documentElement();

            // 从根节点上删除该节点
            root.removeChild(list.at(i));
            QFile file(m_strFileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
                return ;
            QTextStream out(&file);
            doc.save(out,4);
            file.close();
            qDebug() << "delete Element xml";
        }

    }
}
/**
 * @funcname  updateDomXml
 * @brief     update Element
 * @param     strId
 * @param     strListText
 * @return    no
 */

void DomXmlAnalysisBase::updateDomXml(QString strId, QList<QString> strListText)
{
    if(strListText.count()<5)
        return ;
    QString strName = strListText.at(0);
    QString strSn = strListText.at(1);
    QString strPrivate = strListText.at(2);
    QString strIp = strListText.at(3);
    QString strStatua = strListText.at(4);
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();

    // 以标签名进行查找
    QDomNodeList list = doc.elementsByTagName("Device");

    for(int i=0; i<list.count(); i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute("Id") == strId)
        {   // 如果元素的“编号”属性值与我们所查的相同

            // 如果是更新操作
            QDomNodeList child = list.at(i).childNodes();

            // 将它子节点的首个子节点（就是文本节点）的内容更新，离线时只更新状态

            if(child.at(0).toElement().text() != strName)
                child.at(0).toElement().firstChild().setNodeValue(strName);

            if(child.at(1).toElement().text() != strSn)
                child.at(1).toElement().firstChild().setNodeValue(strSn);

            if(child.at(2).toElement().text() != strPrivate)
            child.at(2).toElement().firstChild().setNodeValue(strPrivate);

            if(child.at(3).toElement().text() != strIp)
                child.at(3).toElement().firstChild().setNodeValue(strIp);

            if(child.at(4).toElement().text() != strStatua)
            child.at(4).toElement().firstChild().setNodeValue(strStatua);
            QFile file(m_strFileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return ;
            QTextStream out(&file);
            doc.save(out,4);
            file.close();
            qDebug() << "update Element xml";
        }
    }
}
/**
 * @funcname  getDomXmlstrIdList
 * @brief     遍历获取所有id
 * @param     strIdList output param
 * @return    ret
 */

void DomXmlAnalysisBase::getDomXmlstrIdList(QList<QString> &strIdList)
{
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    if(!strIdList.isEmpty())
        strIdList.clear();
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            //ui->listWidget->addItem(e.tagName() + e.attribute("编号"));
            QDomNodeList list = e.childNodes();
            strIdList.append(e.attribute("Id"));
        }
        n = n.nextSibling();
    }
}
