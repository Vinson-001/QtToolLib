#ifndef DEVINFOSYS_H
#define DEVINFOSYS_H

#include <QDialog>

namespace Ui {
class DevInfoSys;
}

class DevInfoSys : public QDialog
{
    Q_OBJECT

public:
    explicit DevInfoSys(QWidget *parent = nullptr);
    ~DevInfoSys();

private:
    Ui::DevInfoSys *ui;
};

#endif // DEVINFOSYS_H
