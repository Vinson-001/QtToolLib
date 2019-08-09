#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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

    void on_delete_2_clicked();

    void on_update_clicked();

    void on_read_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
