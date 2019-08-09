#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mytableview.h"
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
    void on_create_clicked();

    void on_add_clicked();

    void on_remove_clicked();

    void on_removeAll_clicked();

    void on_getValue_clicked();

private:
    Ui::Widget *ui;
    MyTableView *table;
};

#endif // WIDGET_H
