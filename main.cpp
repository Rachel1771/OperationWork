#define _CRT_SECURE_NO_WARNINGS
#include "iostream"
#include "struct.h"
#include <cstdio>


//首次适应算法
void FF(struct memLinkList *pmemlist, size_t allocatesize)
{
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        return;
    }
    sortAscAdr(pmemlist);
    searchEmptyMemery(pmemlist, allocatesize, 0);
}

//最佳适应算法
void BF(struct memLinkList *pmemlist, size_t allocatesize)
{
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        return;
    }
    sortAscSize(pmemlist);
    searchEmptyMemery(pmemlist, allocatesize, 0);
}

//最坏适应算法
void WF(struct memLinkList *pmemlist, size_t allocatesize)
{
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        return;
    }
    sortDescSize(pmemlist);
    searchEmptyMemery(pmemlist, allocatesize, 0);
}



//循环首次适应算法
void NF(struct memLinkList *pmemlist, size_t allocatesize)
{
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        //如果需要分配的空间为0 直接返回
        return;
    }
    sortAscAdr(pmemlist);
    searchEmptyMemery(pmemlist, allocatesize, 1);
}



void allocateMemory(struct memLinkList *pmemlist, int n)
{
    size_t allocatesize = 0;
    printf("请输入要分配的内存大小:");
    scanf("%lu", &allocatesize);
    switch (n) {
        case 1:
            //首次适应算法
            FF(pmemlist, allocatesize);
            break;
        case 2:
            //循环首次适应算法
            NF(pmemlist, allocatesize);
            break;
        case 3:
            //最佳适应算法
            BF(pmemlist, allocatesize);
            break;
        case 4:
            //最坏适应算法
            WF(pmemlist, allocatesize);
            break;
        default:
            printf("输入错误！\n");
            break;
    }
}

void recoveryMemory(struct memLinkList *pmemlist)
{
    assert(pmemlist != NULL);
    size_t membegin = 0;
    printf("请输入要回收的内存的起始地址:");
    scanf("%lu", &membegin);
    //知道起始地址以后，在已分配内存链表中查找，找到后，将其取出来，
    //然后将空闲链表按地址升序排序，排序后将要回收的插入链表中，
    //进行可能的合并
    struct Node *ret = searchUsedMemery(pmemlist, membegin);
    //在已分配链表中找到要回收的
    if (ret == NULL) {
        return;
    }
    sortAscAdr(pmemlist);   //按地址排序
    emptyMemeryPush(pmemlist, ret);  //插入　＋ 合并
}



int main()
{
    size_t membegin = 0;
    size_t memsize = 0;
    int i = 0;
    int n = 0;
    struct memLinkList memlist;
    printf("please input memery begin: \n");
    scanf("%lu", &membegin);
    printf("please input memery szie : \n");
    scanf("%lu", &memsize);
    memListInit(&memlist, membegin, memsize);
    printf("请输入选项\n");
    printf("1.首次适应算法  2.循环首次适应算法  3.最佳适应算法  4.最坏适应算法 : \n");
    scanf("%d", &n);       //没有进行输入错误处理，直接当输入的正确
    while (1) {
        printList(&memlist);
        printf("请选择功能: ");
        printf("1.分配空间    2.回收空间   3.退出: ");
        scanf("%d", &i);
        switch (i) {
            case 1:
                allocateMemory(&memlist, n);
                break;
            case 2:
                recoveryMemory(&memlist);
                break;
            case 3:
                printf("程序退出！\n");
                return 0;
            default:
                printf("选项输入有误，请重新输入！\n");
                break;
        }
    }
    return 0;
}
