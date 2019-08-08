#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "devdiscoverythread.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_start_clicked();

private:
    Ui::Widget *ui;
    QThread *m_SendThread;
    DevDiscoveryThread *m_SendObjThread;
private:
    void startDevThread();
    void stopDevThread();
signals:
    void startUdpSendData();

};

#endif // WIDGET_H
