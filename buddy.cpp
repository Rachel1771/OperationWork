#include <conio.h>
#include "iostream"
using namespace std;
#include <stdlib.h>
#include <stdio.h>
struct Buddy
{
    int size;
    int address;
    Buddy *parent;
    Buddy *right;
    Buddy *left;
    Buddy *next;
    Buddy *previous;
    bool allocated;
    int freeSpace;
    int level;
    bool deleteThis;
};

//定义变量
Buddy *root,*newnodeLeft,*newnodeRight;
int memory,number,n;
Buddy  * freelist[20],*allocateThisNode;
Buddy * rootCopy;
Buddy * temp = NULL;
Buddy * used = NULL;
Buddy * use = NULL;
Buddy * unused = NULL;
Buddy * unuse = NULL;


int allocations=0,internalFragmentation;
int processSize=0,level=0;
int value=1,allocationDenied=0;
int luck=0,processValue=0;
bool deleteSuccess=false,nodeFound=false;

//输出二叉树
void printTree(Buddy *n)
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
        printf("\t");
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
        printf("!"); //标记
        if(n->allocated == true)
        {
            printf("$"); //标记
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
    printf("%2d", n->size);
    printf("-----%2d",n->address);
    printf("\n");
    printTree(n->left);
    level--;
}

Buddy * splitMemory(Buddy *tree)
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
void allocateMemoryToProcess(int processSize)
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
            printf("1");
            return;
        }
    }

    // we have the node where we'll insert the process
    // but first remove it from the free list
    if(allocateThisNode==NULL){  //此处出现问题，待解决 ，为什么指向了空？
        printf("2");
        printf("%d",freelist[n-1]->freeSpace) ;
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
    printf("\n");
    printf("分配成功，大小为：%d",allocateThisNode->size);
    printf("\n");

}


void recursiveCheck(Buddy * root)
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



void randomSearchAndDelete(Buddy * root,int address)
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

            printf("回收成功");
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
void deallocateMemoryOfProcess(int address)
{
    root = rootCopy;
    deleteSuccess = false;
    if(root->left!=NULL && root->right!=NULL)
    {
        randomSearchAndDelete(root,address);
    }

}




void computeInternalFragmentation(Buddy * root)
{
    if(root!=NULL && root->freeSpace < root->size)
        internalFragmentation += root->freeSpace;
    else if(root==NULL)
        return;

    computeInternalFragmentation(root->left);
    computeInternalFragmentation(root->right);


}

void buddySystem()
{
    //初始化
    root=NULL;
    rootCopy=NULL;
    newnodeLeft=NULL;
    newnodeRight=NULL;
    allocations=0;
    allocationDenied=0;
    internalFragmentation=0;


    for(int clear=0;clear<n+1;clear++)
    {
        freelist[clear]=NULL;
    }

    //初始化节点
    root = new Buddy;
    root->size=memory;
    root->address=0;
    root->freeSpace=memory;
    root->left=NULL;
    root->right=NULL;
    root->parent=NULL;
    root->allocated=false;
    root->deleteThis=false;
    root->next=NULL;
    root->previous=NULL;

    rootCopy=root;
    freelist[n]=root;

    int a,b;
    while(1){
        printf("请选择：1.分配内存 2.回收内存\n");
        scanf("%d",&b);
        if(b == 1){
            printf("输入你要分配的内存大小：");
            scanf("%d",&a);
            allocateMemoryToProcess(a);
            printTree(root);
            printf("占用链表：");
            while(use != NULL){
                printf("%d-",use->address);
                use = use->next;
            }
            printf("\n");
            printf("空闲链表：");
            while(unuse != NULL){
                printf("%d-",unuse->address);
                unuse = unuse->next;
            }
            printf("\n");
            use = NULL;
            used = NULL;
            unuse = NULL;
            unused = NULL;
        }
        if(b == 2){
            printf("输入回收内存的地址：");
            scanf("%d",&a);
            deallocateMemoryOfProcess(a);
            printTree(root);
        }
        if(b == 3){
            while(use !=NULL){
                printf("%d-",use->address);
                use = use->next;
            }
        }
    }

}

int main()
{

    while (1)
    {

        printf("\n输入初始化内存大小：");
        scanf("%d",&memory);
        int sum = 1;
        for(int i = 1;i<20;i++){
            sum = sum*2;
            if(sum==memory){
                n = i;
                break;
            }
        }
        buddySystem();
    }


    return 0;
}