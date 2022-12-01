#include "mainwindow.h"
#include "initializezddialog.h"
#include "ui_mainwindow.h"
#include "struct.h"
#include "setspace.h"
#include "removespace.h"

int ramSize;
int ramStartAddress;
QButtonGroup *btg;
struct memLinkList memlist;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    btg = new QButtonGroup(this);
    btg->addButton(ui->FF,1);
    btg->addButton(ui->NF,2);
    btg->addButton(ui->BF,3);
    btg->addButton(ui->WF,4);
    btg->addButton(ui->radioButton_5,5);
    ui->FF->setChecked(1);

    ui->arrangeButton->setDisabled(1);
    ui->recycleButton->setDisabled(1);
    ui->resetButton->setDisabled(1);

    QStringList header; //动态添加表头
    header<<"起始地址"<<"末尾地址"<<"大小";


    ui->usedSpaceTable->setShowGrid(true);
    ui->usedSpaceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->usedSpaceTable->setColumnCount(3);
    ui->usedSpaceTable->setRowCount(0);
    ui->usedSpaceTable->setHorizontalHeaderLabels(header);
    ui->usedSpaceTable->verticalHeader()->setVisible(false);
    ui->usedSpaceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->usedSpaceTable->horizontalHeader()->setFont(QFont("song", 8));       //设置字体格式

    ui->freeSpaceTable->setShowGrid(true);
    ui->freeSpaceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->freeSpaceTable->setColumnCount(3);
    ui->freeSpaceTable->setRowCount(0);
    ui->freeSpaceTable->setHorizontalHeaderLabels(header);
    ui->freeSpaceTable->verticalHeader()->setVisible(false);
    ui->freeSpaceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->freeSpaceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->freeSpaceTable->horizontalHeader()->setFont(QFont("song", 8));       //设置字体格式

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveNewSpaceSize(int spaceSize){
    qDebug("收到spaceSize:%d",spaceSize);
    allocateMemory(&memlist,btg->checkedId(),spaceSize);
    printList(&memlist);
}

void MainWindow::receiveSpaceAddress(int spaceAddress){
    qDebug("收到待回收spaceAddress:%d",spaceAddress);
    recoveryMemory(&memlist,spaceAddress);
    printList(&memlist);
}

void MainWindow::on_initButton_clicked()
{
    ramSize = ui->RAMSize->toPlainText().toInt();
    ramStartAddress = ui->RAMStartAddress->toPlainText().toInt();
    qDebug("ramSize:%d",ramSize);
    qDebug("ramStartAddress:%d",ramStartAddress);
    qDebug("nowMode:%d",btg->checkedId());
    memListInit(&memlist, ramStartAddress, ramSize);
    InitializezdDialog * init_dialog = new InitializezdDialog();
    ui->arrangeButton->setDisabled(0);
    ui->recycleButton->setDisabled(0);
    ui->resetButton->setDisabled(0);

    init_dialog->show();
    printList(&memlist);
}

void MainWindow::on_resetButton_clicked()
{
    ui->FF->setChecked(1);
    memListInit(&memlist, ramStartAddress, ramSize);
    recoveryAllMemory(&memlist);
    printList(&memlist);
}

void MainWindow::on_arrangeButton_clicked()
{
    setSpace *ss = new setSpace();
    connect(ss,SIGNAL(sendSpaceSize(int)),this,SLOT(receiveNewSpaceSize(int)));
    ss->show();
}

void MainWindow::on_recycleButton_clicked()
{
    removeSpace *rs = new removeSpace();
    connect(rs,SIGNAL(sendRemoveSpaceAddress(int)),this,SLOT(receiveSpaceAddress(int)));
    rs->show();
}

//输出链表
void MainWindow::printList(struct memLinkList *pmemlist)
{
    assert(pmemlist != NULL);
    struct Node *cur = NULL;
    //输出空闲链表
    cur = pmemlist->_emptylist._head;
    qDebug("空闲的链表为: \n");
    int free_count = 0;//记录链表元素个数
    int i = 0;//表格的行标记
    
    while (cur != NULL) {
        qDebug("|begin:%3lu|    ", cur->_data._begin);	//输出起始地址
        free_count++;
        qDebug("count++");
        if (cur == pmemlist->_emptylist._tail) {
            break;
        }
        cur = cur->_next;
    }
    qDebug("\n");
    cur = pmemlist->_emptylist._head;
    while (cur != NULL) {
        ui->freeSpaceTable->setRowCount(free_count);
        ui->freeSpaceTable->setItem(i,0,new QTableWidgetItem(QString::number(cur->_data._begin)));
        ui->freeSpaceTable->setItem(i,1,new QTableWidgetItem(QString::number(cur->_data._end)));
        ui->freeSpaceTable->setItem(i,2,new QTableWidgetItem(QString::number(cur->_data._size)));
        qDebug("|size :%3lu|", cur->_data._size);	//输出大小

        if (cur == pmemlist->_emptylist._tail) {
            break;
        }

        cur = cur->_next;
        i++;
        qDebug("--->");
    }
    qDebug("\n");
    cur = pmemlist->_emptylist._head;
    while (cur != NULL) {
        qDebug("|end  :%3lu|    ", cur->_data._end);	//输出结束地址
        if (cur == pmemlist->_emptylist._tail) {
            break;
        }
        cur = cur->_next;
    }
    qDebug("\n");
    qDebug("free count:%d",free_count);
    if(free_count == 0)
        ui->freeSpaceTable->setRowCount(0);

    //输出已分配内存链表
    int j = 0;
    int used_count = 0;
    cur = pmemlist->_usedlist._head;
    qDebug("已分配的链表为: \n");
    while (cur != NULL) {
        qDebug("|begin  :%3lu|    ", cur->_data._begin);
        cur = cur->_next;
        used_count++;
    }
    qDebug("\n");
    cur = pmemlist->_usedlist._head;
    while (cur != NULL) {
        ui->usedSpaceTable->setRowCount(used_count);
        ui->usedSpaceTable->setItem(j,0,new QTableWidgetItem(QString::number(cur->_data._begin)));
        ui->usedSpaceTable->setItem(j,1,new QTableWidgetItem(QString::number(cur->_data._end)));
        ui->usedSpaceTable->setItem(j,2,new QTableWidgetItem(QString::number(cur->_data._size)));
        qDebug("|size :%3lu|", cur->_data._size);

        if (cur == pmemlist->_usedlist._tail) {
            break;
        }

        cur = cur->_next;
        qDebug("--->");
        j++;
    }
    qDebug("\n");
    cur = pmemlist->_usedlist._head;
    while (cur != NULL) {
        qDebug("|end  :%3lu|    ", cur->_data._end);
        cur = cur->_next;
    }
    qDebug("\n");
    qDebug("used count:%d",used_count);
    if(used_count == 0)
        ui->usedSpaceTable->setRowCount(0);
}
