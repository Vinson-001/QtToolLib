#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString OpenFile, OpenFilePath;
    QImage image;
    //打开文件夹中的图片文件
    OpenFile = QFileDialog::getOpenFileName(this,
                                              "Please choose an image file",
                                              "",
                                              "Image Files(*.jpg *.jpeg *.png *.bmp *.pgm *.pbm);;All(*.*)");


    QPixmap icon(OpenFile);
    ui->label->setPixmap(icon);
    ui->label->resize(icon.width(),icon.height());
        if( OpenFile != "" )
        {
            if( image.load(OpenFile) )
            {
                //ui->label->setPixmap(QPixmap::fromImage(image));
            }
        }

        //显示所示图片的路径
        QFileInfo OpenFileInfo;
        OpenFileInfo = QFileInfo(OpenFile);
        OpenFilePath = OpenFileInfo.filePath();
        qint64 size = OpenFileInfo.size();
        qDebug() << icon.width() << icon.height();

        ui->lineEdit->setText(OpenFilePath);
   }
