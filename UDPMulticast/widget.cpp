#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    m_SendThread = NULL;
    m_SendObjThread = NULL;
    ui->setupUi(this);
    startDevThread();
}

Widget::~Widget()
{
    stopDevThread();
    delete ui;
}

void Widget::on_start_clicked()
{
    emit startUdpSendData();
}

void Widget::startDevThread()
{
    if(m_SendThread)
    {
        return;
    }
    m_SendThread= new QThread();
    m_SendObjThread = new DevDiscoveryThread();
    m_SendObjThread->moveToThread(m_SendThread);
    connect(m_SendThread,&QThread::finished,m_SendThread,&QObject::deleteLater);
    connect(m_SendThread,&QThread::finished,m_SendObjThread,&QObject::deleteLater);
    //绑定设备请求
    //connect(this,&DeviceMangerWindow::stopDevSendThreadSignal,m_SendObjThread,&DevSendThread::stop);
    connect(this,&Widget::startUdpSendData,m_SendObjThread,&DevDiscoveryThread::startSending);
    //connect(m_SendObjThread,&DevSendThread::startUpdateDevInfo,this,&Widget::updateDevInfoDisplay);


    m_SendThread->start();
}

void Widget::stopDevThread()
{
    qDebug() << "stop m_SendThread";

    if(m_SendThread)
    {
        if(m_SendObjThread){
            m_SendObjThread->enableThread(isStop);
        }
        m_SendThread->quit();
    }
    m_SendThread->wait();
    qDebug() << "end m_SendThread";
}
