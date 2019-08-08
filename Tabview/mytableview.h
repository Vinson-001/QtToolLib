/**
* @fileName      MyTableView.h
* @brief         自定义tabelview表
* @author        Wxyang
* @date          2019-08-08
*/
#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
class MyTableView : public QTableView
{
public:
    explicit MyTableView(QList<QString> strListHeadData);
    ~MyTableView();

private:
    QStandardItemModel *m_model;
    QList<QString> m_strListHeadData;

private:
    void initTableView();
    void addRowTextForTable(int rowConut,QList<QString> strTextList);
    void addRowForTable(QList<QString> strTextList);
    void removeRowForTable();
    void removeAllRowForTable();
    void sortColForTable();

public:
    void doTable(QString operate, QList<QString> strTextList);
    QString getValueFromRowAndCol(int row,int col);
};

#endif // MYTABLEVIEW_H
