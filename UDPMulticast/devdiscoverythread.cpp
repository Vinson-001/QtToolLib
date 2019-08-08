#include "devdiscoverythread.h"
#define DISCOVERY_SERVER_ADDR "239.255.255.221"
#define DISCOVERY_SERVER_PORT 3188
DevDiscoveryThread::DevDiscoveryThread(QObject *parent) : QObject(parent)
{
    m_timer = NULL;
    m_udpSocket = NULL;
    m_isStop = false;
    initUdp();
}

DevDiscoveryThread::~DevDiscoveryThread()
{
    enableThread(isStop);
    delete m_udpSocket;
    m_udpSocket = NULL;
}
/**
 * @funcname  initUdp
 * @brief     bind udp
 * @return    no
 */

void DevDiscoveryThread::initUdp()
{
    m_timer = new QTimer(this);
    m_udpSocket = new QUdpSocket(this);
    if (m_udpSocket->state() != m_udpSocket->BoundState)
    {
        // Rx not in bound state, attempt to bind
        m_udpSocket->bind(QHostAddress::AnyIPv4,DISCOVERY_SERVER_PORT,QUdpSocket::ShareAddress);
    }
    connect(m_udpSocket, SIGNAL(readyRead()),this, SLOT(udpRecvData()),Qt::DirectConnection);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(udpSendData()),Qt::DirectConnection);
    m_strfileTempPath = QDir::currentPath();
    m_strfileTempPath += "/DevInfoTemp.xml";
    m_strfileRegPath = QDir::currentPath() + "/DevInfoRegister.xml";
    m_strfileUnRegPath = QDir::currentPath() + "/DevInfoUnRegister.xml";
    createTempXmlforRecvData(m_strfileTempPath);
}
/**
 * @funcname  createTempXmlforRecvData
 * @brief     创建临时文件
 * @param     fileName
 * @return    no
 */

void DevDiscoveryThread::createTempXmlforRecvData(QString fileName)
{
    QFileInfo file(fileName);
    if(file.exists() == false) {
        QFile file(fileName);
        int ret = file.open(QIODevice::WriteOnly| QIODevice::Text);
        if(!ret) {
            qDebug() << file.errorString() << "DevInfoTemp.xml failed";
            return;
        }
        file.close();
    }
}
/**
 * @funcname  readDomTempXml
 * @brief     读取xml临时文件
 * @param     strListText output param
 * @return    no
 */

void DevDiscoveryThread::readDomTempXml(QList<QString> &strListText)
{
    DomXmlAnalysisForUdp domXml(m_strfileTempPath);
    domXml.readDomXmlTemp(strListText);
}
/**
 * @funcname  isDomRegisterXml
 * @brief     查询注册表中是否存在id
 * @param     strId
 * @return    ret
 */

bool DevDiscoveryThread::isDomRegisterXml(QString strId)
{
    DomXmlAnalysisForUdp domXml(m_strfileRegPath);
    return domXml.isDomXmlstrIdexist(strId);
}
/**
 * @funcname  updateRegXml
 * @brief     更新注册表信息(name,sn,private,ip,status)
 * @param     strId
 * @param     strListText
 * @return    no
 */

void DevDiscoveryThread::updateRegXml(QString strId, QList<QString> strListText)
{
    DomXmlAnalysisForUdp domXml(m_strfileRegPath);
    domXml.doXml("update",strId,strListText);
}
/**
 * @funcname  addUnRegXml
 * @brief     添加到未注册表单里,添加时已经查重过
 * @param     strId
 * @param     strListText
 * @return    no
 */

void DevDiscoveryThread::addUnRegXml(QString strId, QList<QString> strListText)
{
    DomXmlAnalysisForUdp domXml(m_strfileUnRegPath);
    domXml.doXml("add",strId,strListText);
}
/**
 * @funcname  enableThread
 * @brief     使能线程
 * @param     en true | stop false
 * @return    no
 */

void DevDiscoveryThread::enableThread(bool en)
{
    QMutexLocker locker(&m_stopMutex);
    m_isStop = en;
}


/**
 * @funcname  udpSendData
 * @brief     udp发送设备请求
 * @return    no
 */

void DevDiscoveryThread::udpSendData()
{
    QByteArray datagram = "Device Discovery";
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(),QHostAddress(DISCOVERY_SERVER_ADDR),DISCOVERY_SERVER_PORT);
    qDebug() << datagram.data();
}

void DevDiscoveryThread::udpRecvData()
{
    enableThread(isRun);
    int count = 0;

    if (!m_isStop) {
       qDebug() << "break recvdata";
       return;
    }
    while (m_udpSocket->hasPendingDatagrams()) {
        QThread::msleep(1000);

        QByteArray datagram;
        QHostAddress sendAddress;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size(),&sendAddress);
        QString recv_ip = sendAddress.toString();

        if(sendAddress.protocol() == QAbstractSocket::IPv4Protocol){
           //qDebug() << "[IP]: "<<recv_ip;
           //qDebug() << datagram.data();
               /*1. 保存xml */
               QFile file(m_strfileTempPath);
               if(!file.open(QIODevice::WriteOnly| QIODevice::Text))
                   return;
               file.resize(0);                  /*清空*/
               file.write(datagram);
               file.close();

               /*2. 解析xml */
               /*2.1 解析临时xml */
               QList<QString> strListTemp;
               readDomTempXml(strListTemp);
               /*2.2 保存id：ip */
               strListTemp.append(recv_ip);         /*id,name,sn,private,ip,status*/
               strListTemp.append(tr("在线"));
               //qDebug() << strListTemp;

               /*3. 查询注册表xml*/
               bool isexist = isDomRegisterXml(strListTemp.at(0));
               if(isexist)
               {
                   QString strId = strListTemp.at(0);
                   strListTemp.removeAt(0);
                   QMutexLocker locker(&m_lockForXml);
                   updateRegXml(strId,strListTemp);
               } else {
                   QString strId = strListTemp.at(0);
                   strListTemp.removeAt(0);
                   QMutexLocker locker(&m_lockForXml);
                   addUnRegXml(strId,strListTemp);
                   qDebug() << strListTemp;
                }
               //count++;
           }
       }
       /*5. 发射信号*/

       //isUpdate = true;
       emit startUpdateDevInfo();
       qDebug() << count;

}

void DevDiscoveryThread::startSending()
{
    if(!m_timer->isActive())
        m_timer->start(1000*10);

}
