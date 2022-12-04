#include "friendswindow.h"
#include "ui_friendswindow.h"
#include "mainwindow.h"
#include "initializezddialog.h"
#include "setspace.h"
#include "removespace.h"
#include "buddy.h"

int f_ramSize;

//定义变量
int number,n;
Buddy *root,*newnodeLeft,*newnodeRight;
Buddy * freelist[20],*allocateThisNode;
Buddy * rootCopy;
Buddy * temp = NULL;
Buddy * used = NULL;
Buddy * use = NULL;
Buddy * unused = NULL;
Buddy * unuse = NULL;

int allocations=0,internalFragmentation;
int processSize=0,level=0;
int value=1,allocationDenied=0;
int processValue=0;
bool deleteSuccess=false,nodeFound=false;

FriendsWindow::FriendsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FriendsWindow)
{
    ui->setupUi(this);
    ui->f_arrangeButton->setDisabled(1);
    ui->f_recycleButton->setDisabled(1);

    QStringList header; //动态添加表头
    header<<"起始地址"<<"末尾地址"<<"大小";


    ui->f_usedSpaceTable->setShowGrid(true);
    ui->f_usedSpaceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->f_usedSpaceTable->setColumnCount(3);
    ui->f_usedSpaceTable->setRowCount(0);
    ui->f_usedSpaceTable->setHorizontalHeaderLabels(header);
    ui->f_usedSpaceTable->verticalHeader()->setVisible(false);
    ui->f_usedSpaceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->f_usedSpaceTable->horizontalHeader()->setFont(QFont("song", 8));       //设置字体格式

    ui->f_freeSpaceTable->setShowGrid(true);
    ui->f_freeSpaceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->f_freeSpaceTable->setColumnCount(3);
    ui->f_freeSpaceTable->setRowCount(0);
    ui->f_freeSpaceTable->setHorizontalHeaderLabels(header);
    ui->f_freeSpaceTable->verticalHeader()->setVisible(false);
    ui->f_freeSpaceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->f_freeSpaceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->f_freeSpaceTable->horizontalHeader()->setFont(QFont("song", 8));       //设置字体格式

}

FriendsWindow::~FriendsWindow()
{
    delete ui;
}

void FriendsWindow::receiveNewSpaceSize(int spaceSize){
    qDebug("收到f_spaceSize:%d",spaceSize);
    allocateMemoryToProcess(spaceSize);
    printTree(root);
    f_printList();
    freeList();
}

void FriendsWindow::receiveSpaceAddress(int spaceAddress){
    qDebug("收到待回收f_spaceAddress:%d",spaceAddress);
    deallocateMemoryOfProcess(spaceAddress);
    printTree(root);
    f_printList();
    freeList();
}

void FriendsWindow::on_pushButton_clicked()//销毁该窗口并返回
{
    MainWindow *mw = new MainWindow;
    mw->show();
    this->destroy();
}

void FriendsWindow::on_f_initButton_clicked()
{
    f_ramSize = ui->RAMSize->toPlainText().toInt();
    initMemory(f_ramSize);
    qDebug("f_ramSize:%d",f_ramSize);
//    memListInit(&memlist, ramStartAddress, ramSize);
    InitializezdDialog * init_dialog = new InitializezdDialog();
    ui->f_arrangeButton->setDisabled(0);
    ui->f_recycleButton->setDisabled(0);

    init_dialog->show();
    printTree(root);
    f_printList();
    freeList();
}


void FriendsWindow::on_f_arrangeButton_clicked()
{
    setSpace *ss = new setSpace();
    connect(ss,SIGNAL(sendSpaceSize(int)),this,SLOT(receiveNewSpaceSize(int)));
    ss->show();
}

void FriendsWindow::on_f_recycleButton_clicked()
{
    removeSpace *rs = new removeSpace();
    connect(rs,SIGNAL(sendRemoveSpaceAddress(int)),this,SLOT(receiveSpaceAddress(int)));
    rs->show();
}

void FriendsWindow::f_printList(){
    ui->f_freeSpaceTable->setRowCount(0);
    ui->f_freeSpaceTable->setRowCount(0);

    int free_count = 0 ,used_count = 0;
    int i = 0, j = 0;

    while(unuse != NULL){
        free_count++;
        qDebug("unuse:%d",unuse->address);
        ui->f_freeSpaceTable->setRowCount(free_count);
        ui->f_freeSpaceTable->setItem(i,0,new QTableWidgetItem(QString::number(unuse->address)));
        ui->f_freeSpaceTable->setItem(i,1,new QTableWidgetItem(QString::number(unuse->address+unuse->size - 1)));
        ui->f_freeSpaceTable->setItem(i,2,new QTableWidgetItem(QString::number(unuse->size)));
        unuse = unuse->next;
        i++;
    }

    while(use != NULL){
        used_count++;
        qDebug("use:%d",use->address);
        ui->f_usedSpaceTable->setRowCount(used_count);
        ui->f_usedSpaceTable->setItem(j,0,new QTableWidgetItem(QString::number(use->address)));
        ui->f_usedSpaceTable->setItem(j,1,new QTableWidgetItem(QString::number(use->address+use->size - 1)));
        ui->f_usedSpaceTable->setItem(j,2,new QTableWidgetItem(QString::number(use->size)));
        use = use->next;
        j++;
    }

    qDebug("输出成功");

    if(free_count == 0)
        ui->f_freeSpaceTable->setRowCount(0);
    if(used_count == 0)
        ui->f_usedSpaceTable->setRowCount(0);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//数据相关操作



//输出二叉树
void FriendsWindow::printTree(Buddy *n)
{
    static int level = -1; //记录是第几层次
    int i;
    if (NULL == n)
        return;

    level++;
    printTree(n->right);
    level--;

    level++;
    for (i = 0; i < level; i++)
        qDebug("\t");
    if(n->left == NULL && n->right == NULL)
    {
        temp = new Buddy;
        temp->size = n->size;
        temp->address = n->address;
        temp->allocated = n->allocated;
        temp->freeSpace = n->freeSpace;
        temp->level = n->level;
        temp->deleteThis = n->deleteThis;
        temp->parent = NULL;
        temp->right = NULL;
        temp->left = NULL;
        temp->next = NULL;
        temp->previous = NULL;
        qDebug("!"); //标记
        if(n->allocated == true)
        {
            qDebug("$"); //标记
            if(use == NULL)
            {
                used = temp;
                use = used;
                temp = NULL;
//			used = used->next;
            }
            else if(use!=NULL)
            {
                used->next = temp;
                used = used->next;
                temp = NULL;
            }
        }
        else
        {
            if(unuse == NULL)
            {
                unused = temp;
                unuse = unused;
                temp = NULL;
//			used = used->next;
            }
            else if(unuse!=NULL)
            {
                unused->next = temp;
                unused = unused->next;
                temp = NULL;
            }
        }
    }
    qDebug("%2d", n->size);
    qDebug("-----%2d",n->address);
    qDebug("\n");
    printTree(n->left);
    level--;
}

Buddy * FriendsWindow::splitMemory(Buddy *tree)
{
    if(tree==NULL)
        return NULL;

    if(tree->right==NULL && tree->left==NULL && tree->size > processValue && tree->allocated==false && nodeFound==false) {

        newnodeRight = new Buddy;
        newnodeLeft = new Buddy;

        newnodeRight->parent = tree;
        newnodeLeft->parent = tree;
        newnodeRight->size = tree->size / 2;
        newnodeLeft->size = tree->size / 2;
        newnodeRight->freeSpace = tree->size / 2;
        newnodeLeft->freeSpace = tree->size / 2;
        newnodeRight->allocated = false;
        newnodeLeft->allocated = false;
        newnodeRight->deleteThis = false;
        newnodeLeft->deleteThis = false;
        newnodeLeft->address = tree->address;
        newnodeRight->address = (tree->address + tree->size / 2);
        newnodeRight->left = NULL;
        newnodeRight->right = NULL;
        newnodeLeft->left = NULL;
        newnodeLeft->right = NULL;

        //连接
        tree->left = newnodeLeft;
        tree->right = newnodeRight;
        tree->deleteThis = true;

        value = 1;
        level = 0;
        while (value < tree->size) {
            value = value * 2;
            level++;
        }

        while (freelist[level]->deleteThis != true) {
            freelist[level] = freelist[level]->next;
        }

        freelist[level]->deleteThis = false;
        if (freelist[level]->previous != NULL) {
            freelist[level]->previous->next = freelist[level]->next;
            if (freelist[level]->next != NULL)
                freelist[level]->next->previous = freelist[level]->previous;
        } else {
            if (freelist[level]->next != NULL) {
                freelist[level] = freelist[level]->next;
                freelist[level]->previous = NULL;
            } else
                freelist[level] = NULL;
        }

        // 插入节点
        value = 1;
        level = 0;
        while (value < newnodeLeft->size) {
            value = value * 2;
            level++;
        }

        if (freelist[level] != NULL) {
            while (freelist[level]->next != NULL) {
                freelist[level] = freelist[level]->next;
            }

            freelist[level]->next = newnodeLeft;
            newnodeLeft->previous = freelist[level];
            newnodeLeft->next = newnodeRight;
            newnodeRight->previous = newnodeLeft;
            newnodeRight->next = NULL;
        } else {
            freelist[level] = newnodeLeft;
            freelist[level]->previous = NULL;
            freelist[level]->next = newnodeRight;
            newnodeRight->previous = newnodeLeft;
            newnodeRight->next = NULL;
        }


        if (newnodeLeft->size == processValue) {
            nodeFound = true;
            allocateThisNode = tree->left;
        }

    }

    if(nodeFound==false)
    {
        splitMemory(tree->left);
        splitMemory(tree->right);
    }

}

//分配内存给进程
void FriendsWindow::allocateMemoryToProcess(int processSize)
{

    processValue=1;
    level=0;
    while(processValue < processSize)
    {
        processValue = processValue * 2;
        level++;
    }

    allocateThisNode = NULL;
    // search free node in the list
    if(freelist[level]!=NULL)
    {

        if(freelist[level]->size==processValue)
        {

            allocateThisNode = freelist[level];
            if(freelist[level]->next!=NULL)
            {
                freelist[level]=freelist[level]->next;
                freelist[level]->previous=NULL;
            }
            else
                freelist[level]=NULL;

            allocateThisNode->next=NULL;
            allocateThisNode->previous=NULL;
        }

    }// end if
    else
    {	nodeFound=false;
        root=rootCopy;
        splitMemory(root);
        if(allocateThisNode!=NULL)
            allocateThisNode->deleteThis=true;
        else
        {
            allocationDenied++;
            qDebug("1");
            return;
        }
    }
    // we have the node where we'll insert the process
    // but first remove it from the free list
    if(allocateThisNode==NULL){  //此处出现问题，待解决 ，为什么指向了空？
        qDebug("2");
        qDebug("%d",freelist[n-1]->freeSpace) ;
        printTree(root);
        return;
    }


    if(allocateThisNode->deleteThis==true)
    {
        level=0;
        value=1;
        while(allocateThisNode->size > value)
        {
            value = value * 2;
            level++;
        }

        while(freelist[level]->deleteThis!=true)
        {
            freelist[level]=freelist[level]->next;
        }

        if(freelist[level]->previous!=NULL)
        {
            freelist[level]->previous->next=freelist[level]->next;
            if(freelist[level]->next!=NULL)
                freelist[level]->next->previous=freelist[level]->previous;
        }
        else
        {
            if(freelist[level]->next!=NULL)
            {
                freelist[level]=freelist[level]->next;
                freelist[level]->previous=NULL;
            }
            else
                freelist[level]=NULL;
        }

    }
    allocateThisNode->allocated=true;
    allocateThisNode->freeSpace=allocateThisNode->size - processSize;
    allocations++;
    qDebug("\n");
    qDebug("分配成功，大小为：%d",allocateThisNode->size);
    qDebug("\n");

}


void FriendsWindow::recursiveCheck(Buddy * root)
{
    if( root->parent!=NULL && root->parent->left->left==NULL && root->parent->left->right==NULL && root->parent->right->left==NULL && root->parent->right->right==NULL)
    {
        if(root->parent->left->allocated==false && root->parent->right->allocated==false)
        { // merge the two nodes.

            if(root->parent->left==root)
            {
                // remove the right portion from the
                // free list
                root->parent->right->deleteThis=true;
                value=1;
                level=0;
                while(value < root->parent->right->size)
                {
                    value = value * 2;
                    level++;
                }

                while(freelist[level]->deleteThis!=true)
                {
                    freelist[level]=freelist[level]->next;
                }

                // set the delete bit to false again for future use
                freelist[level]->deleteThis=false;

                if(freelist[level]->previous!=NULL)
                {
                    freelist[level]->previous->next=freelist[level]->next;
                    if(freelist[level]->next!=NULL)
                        freelist[level]->next->previous=freelist[level]->previous;
                }// end of if
                else
                {
                    if(freelist[level]->next!=NULL)
                    {
                        freelist[level]=freelist[level]->next;
                        freelist[level]->previous=NULL;
                    }
                    else
                        freelist[level]=NULL;
                }

            }// end of if

            else
            {
                // remove the left portion from the free list
                root->parent->left->deleteThis=true;
                value=1;
                level=0;
                while(value < root->parent->left->size)
                {
                    value = value * 2;
                    level++;
                }

                while(freelist[level]->deleteThis!=true)
                {
                    freelist[level]=freelist[level]->next;
                }

                // set the delete bit to false again for future use
                freelist[level]->deleteThis=false;

                if(freelist[level]->previous!=NULL)
                {
                    freelist[level]->previous->next=freelist[level]->next;
                    if(freelist[level]->next!=NULL)
                        freelist[level]->next->previous=freelist[level]->previous;
                }// end of if
                else
                {
                    if(freelist[level]->next!=NULL)
                    {
                        freelist[level]=freelist[level]->next;
                        freelist[level]->previous=NULL;
                    }
                    else
                        freelist[level]=NULL;
                }


            }// end of else

            root->parent->left=NULL;
            root->parent->right=NULL;

            // recursively check till the root if merge is possible
            // of the all the budds or not
            recursiveCheck(root->parent);
        }// end of if on allocated or not
        else
        {
            // insert into the free list
            if(root->parent->left->allocated==true)
            { // insert the right child in the freelist

                value=1;
                level=0;
                while(value < root->parent->right->size)
                {
                    value = value * 2;
                    level++;
                }

                if(freelist[level]!=NULL)
                {
                    while(freelist[level]->next!=NULL)
                    {
                        freelist[level]=freelist[level]->next;
                    }
                    freelist[level]->next=root->parent->right;
                    root->parent->right->previous=freelist[level];

                }// end if
                else
                {
                    freelist[level]=root->parent->right;
                    freelist[level]->previous=NULL;
                    freelist[level]->next=NULL;
                }


            }// end of if on left
            else
            {
                value=1;
                level=0;
                while(value < root->parent->left->size)
                {
                    value = value * 2;
                    level++;
                }

                if(freelist[level]!=NULL)
                {
                    while(freelist[level]->next!=NULL)
                    {
                        freelist[level]=freelist[level]->next;
                    }
                    freelist[level]->next=root->parent->left;
                    root->parent->left->previous=freelist[level];

                }// end if
                else
                {
                    freelist[level]=root->parent->left;
                    freelist[level]->previous=NULL;
                    freelist[level]->next=NULL;
                }// end of else


            }// end of else



        }// end of else on allocated or not


    }// end of outer if on root->left == NULL ....
    else
    { // we are at the root node so insert the 64k chunk
        // in the free list.

        if(root->parent==NULL)
        {
            value=1;
            level=0;
            while(value < root->size)
            {
                value = value * 2;
                level++;
            }

            if(freelist[level]!=NULL)
            {
                while(freelist[level]->next!=NULL)
                {
                    freelist[level]=freelist[level]->next;
                }
                freelist[level]->next=root;
                root->previous=freelist[level];

            }// end if
            else
            {
                // it is the first node
                freelist[level]=root;
                freelist[level]->previous=NULL;
                freelist[level]->next=NULL;
            }// end of else


        }// end of if


    }// end of else



}// end of function



void FriendsWindow::randomSearchAndDelete(Buddy * root,int address)
{
    if(root==NULL)
        return;

    if(root->left==NULL && root->right==NULL)
    {

        if(root->address == address)
        {
            root->allocated=false;
            root->freeSpace=root->size;
            allocations--;

            qDebug("回收成功");
            deleteSuccess=true;
            if(root->parent!=NULL)
            {   //合并回收
                if(root->parent->left->allocated==false && root->parent->right->allocated==false && root->parent->right->right==NULL && root->parent->left->left==NULL)
                {

                    if(root->parent->left==root)
                    {
                        //回收右边
                        root->parent->right->deleteThis=true;
                        value=1;
                        level=0;
                        while(value < root->parent->right->size)
                        {
                            value = value * 2;
                            level++;
                        }

                        while(freelist[level]->deleteThis!=true)
                        {
                            freelist[level]=freelist[level]->next;
                        }
                        freelist[level]->deleteThis=false;

                        if(freelist[level]->previous!=NULL)
                        {
                            freelist[level]->previous->next=freelist[level]->next;
                            if(freelist[level]->next!=NULL)
                                freelist[level]->next->previous=freelist[level]->previous;
                        }
                        else
                        {
                            if(freelist[level]->next!=NULL)
                            {
                                freelist[level]=freelist[level]->next;
                                freelist[level]->previous=NULL;
                            }
                            else
                                freelist[level]=NULL;
                        }

                    }

                    else
                    {
                        //回收左边
                        root->parent->left->deleteThis=true;
                        value=1;
                        level=0;
                        while(value < root->parent->left->size)
                        {
                            value = value * 2;
                            level++;
                        }

                        while(freelist[level]->deleteThis!=true)
                        {
                            freelist[level]=freelist[level]->next;
                        }
                        freelist[level]->deleteThis=false;

                        if(freelist[level]->previous!=NULL)
                        {
                            freelist[level]->previous->next=freelist[level]->next;
                            if(freelist[level]->next!=NULL)
                                freelist[level]->next->previous=freelist[level]->previous;
                        }// end of if
                        else
                        {
                            if(freelist[level]->next!=NULL)
                            {
                                freelist[level]=freelist[level]->next;
                                freelist[level]->previous=NULL;
                            }
                            else
                                freelist[level]=NULL;
                        }


                    }

                    root->parent->left=NULL;
                    root->parent->right=NULL;


                    //持续搜索到顶，来合并
                    if(root->parent!=NULL)
                        recursiveCheck(root->parent);
                }
                else
                {
                    // 插入到对应的链表层
                    value=1;
                    level=0;
                    while(value < root->size)
                    {
                        value = value * 2;
                        level++;
                    }

                    if(freelist[level]!=NULL)
                    {
                        while(freelist[level]->next!=NULL)
                        {
                            freelist[level]=freelist[level]->next;
                        }
                        freelist[level]->next=root;
                        root->previous=freelist[level];

                    }
                    else
                    {
                        freelist[level]=root;
                        freelist[level]->previous=NULL;
                        freelist[level]->next=NULL;
                    }

                }

            }
        }
    }
    if(deleteSuccess==false)
    {
        randomSearchAndDelete(root->left,address);
        randomSearchAndDelete(root->right,address);
    }


}


//删除内存块然后合并
void FriendsWindow::deallocateMemoryOfProcess(int address)
{
    root = rootCopy;
    deleteSuccess = false;
    if(root->left!=NULL && root->right!=NULL)
    {
        randomSearchAndDelete(root,address);
    }

}




void FriendsWindow::computeInternalFragmentation(Buddy * root)
{
    if(root!=NULL && root->freeSpace < root->size)
        internalFragmentation += root->freeSpace;
    else if(root==NULL)
        return;

    computeInternalFragmentation(root->left);
    computeInternalFragmentation(root->right);


}

void FriendsWindow::initMemory(int ramSize){
    //初始化
    root=NULL;
    rootCopy=NULL;
    newnodeLeft=NULL;
    newnodeRight=NULL;
    allocations=0;
    allocationDenied=0;
    internalFragmentation=0;

    int sum = 1;
        for(int i = 1;i<20;i++){
            sum = sum*2;
            if(sum==ramSize){
                n = i;
                break;
            }
        }

    for(int clear=0;clear<n+1;clear++)
    {
        freelist[clear]=NULL;
    }

    //初始化节点
    root = new Buddy;
    root->size=ramSize;
    root->address=0;
    root->freeSpace=ramSize;
    root->left=NULL;
    root->right=NULL;
    root->parent=NULL;
    root->allocated=false;
    root->deleteThis=false;
    root->next=NULL;
    root->previous=NULL;

    rootCopy=root;
    freelist[n]=root;


}

void FriendsWindow::freeList(){
    used = NULL;
    use = NULL;
    unused = NULL;
    unuse = NULL;
}
