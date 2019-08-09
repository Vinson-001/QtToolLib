#include "devinfosys.h"
#include "ui_devinfosys.h"

DevInfoSys::DevInfoSys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevInfoSys)
{
    ui->setupUi(this);
}

DevInfoSys::~DevInfoSys()
{
    delete ui;
}
