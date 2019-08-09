#include "mytableview.h"
#include <QDebug>
#include "mytableviewdelegate.h"
#include "devinfosys.h"
MyTableView::MyTableView(QWidget *parent)
    :QTableView(parent),m_model(NULL)
{
    m_strListHeadData << tr("序号") << tr("设备ID") << tr("设备名称") << tr("设备序列号") << tr("私有数据") << tr("IP地址") << tr("状态");
    initTableView();
}

MyTableView::~MyTableView()
{
    if(m_model){
        delete m_model;
        m_model = NULL;
    }
}
/**
 * @funcname  initTabelView
 * @brief     初始化表格
 * @param     colCount
 * @param     strListHeadData
 * @return    no
 */

void MyTableView::initTableView()
{
    int colCount = m_strListHeadData.count();
    if(m_strListHeadData.isEmpty())
    {
        qDebug() << "initTabelView failed!";
        return;
    }
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(colCount+1);
    for(int i = 0; i < colCount; i++)
    {
        m_model->setHeaderData(i,Qt::Horizontal,m_strListHeadData.at(i));
    }
 #if 0
    m_tableviewModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
    m_tableviewModel->setHeaderData(1,Qt::Horizontal,tr("设备ID"));
    m_tableviewModel->setHeaderData(2,Qt::Horizontal,tr("设备名称"));
    m_tableviewModel->setHeaderData(3,Qt::Horizontal,tr("设备序列号"));
    m_tableviewModel->setHeaderData(4,Qt::Horizontal,tr("私有数据"));
    m_tableviewModel->setHeaderData(5,Qt::Horizontal,tr("IP地址"));
    m_tableviewModel->setHeaderData(6,Qt::Horizontal,tr("状态"));
#endif
    //单元格格式
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter |Qt::AlignHCenter );    //表头信息居中
    this->horizontalHeader()->setObjectName("hHeader");
    this->verticalHeader()->setObjectName("vHeader");
    this->setAlternatingRowColors(true);                                                 /* 奇偶行颜色区分 */
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );                  /*设置表格自适应宽度*/

    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    //ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); /*自适应行高*/
    this->setEditTriggers(QAbstractItemView::NoEditTriggers); /*禁止编辑*/
    this->setMouseTracking(true);
    this->setModel(m_model);
    this->setItemDelegate(new MyTableviewDelegate);
    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotRowDoubleClicked(const QModelIndex &)));
}
/**
 * @funcname  addRowTextForTable
 * @brief     添加一行显示
 * @param     rowConut
 * @param     strTextList
 * @return    no
 */

void MyTableView::addRowTextForTable(int rowConut, QList<QString> strTextList)
{
    QString strNum;
    strNum = QString("%1").arg(rowConut);
    strTextList.prepend(strNum);
    for(int i = 0; i < strTextList.count(); i++) {
        m_model->setItem(rowConut, i, new QStandardItem(strTextList[i]));
        m_model->item(rowConut,i)->setTextAlignment(Qt::AlignCenter);
    }
}
/**
 * @funcname  addRowForTable
 * @brief     新增一行
 * @param     strTexList(不需要插入序号值)
 * @return    no
 */

void MyTableView::addRowForTable(QList<QString> strTextList)
{
    int rowIndex = m_model->rowCount();
    if(m_model == NULL)
        return;
    m_model->insertRow(rowIndex);
    addRowTextForTable(rowIndex,strTextList);
}
/**
 * @funcname  removeRowForTable
 * @brief     删除选中的行
 * @return    no
 */

void MyTableView::removeRowForTable()
{
    int rowIndex = this->currentIndex().row();
    if(m_model == NULL)
        return;
    if (rowIndex != -1)
        m_model->removeRow(rowIndex);
    sortColForTable();
}
/**
 * @funcname  removeAllRowForTable
 * @brief     删除所有的行
 * @return    ret
 */

void MyTableView::removeAllRowForTable()
{
    if(m_model == NULL)
        return;
    m_model->removeRows(0,m_model->rowCount());
}
/**
 * @funcname  sortColForTable
 * @brief     排序
 * @return    no
 */

void MyTableView::sortColForTable()
{

    int rowcount = m_model->rowCount();
    for (int i = 0; i < rowcount; ++i) {
       QModelIndex index= m_model->index(i, 0);
       m_model->setData(index, QVariant::fromValue(i));
    }

}
/**
 * @funcname  doTable
 * @brief     gaiyao
 * @param     operate
 * @param     strTextList
 * @return    no
 */

void MyTableView::doTable(QString operate, QList<QString> strTextList)
{
    if(operate == "add") {
        addRowForTable(strTextList);
    } else if(operate == "remove")
    {
        removeRowForTable();
    } else if(operate == "removeAll")
    {
        removeAllRowForTable();
    }
}
/**
 * @funcname  getValueFromRowAndCol
 * @brief     获取单元格的值(row,col)
 * @param     row
 * @param     col
 * @return    QString
 */

QString MyTableView::getValueFromRowAndCol(int row, int col)
{
    QModelIndex index=m_model->index(row-1,col-1,QModelIndex());//rowNum,columnNum为行列号 从0开始
    QString str= index.data().toString();
    qDebug() << str;
    return str;
}
/**
 * @funcname  slotRowDoubleClicked
 * @brief     双击
 * @param     index
 */

void MyTableView::slotRowDoubleClicked(const QModelIndex &index)
{
    //QModelIndex index = this->currentIndex();
    int row = index.row();
    if (!index.isValid())
    {
        return;
    }
    //qDebug() << row;
    DevInfoSys dlg(this);
    dlg.exec();

}
