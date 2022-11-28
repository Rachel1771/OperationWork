#define _CRT_SECURE_NO_WARNINGS
#include "iostream"
#include "struct.h"
#include <cstdio>


//�״���Ӧ�㷨
void FF(struct memLinkList *pmemlist, size_t allocatesize)
{
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        return;
    }
    sortAscAdr(pmemlist);
    searchEmptyMemery(pmemlist, allocatesize, 0);
}

//�����Ӧ�㷨
void BF(struct memLinkList *pmemlist, size_t allocatesize)
{
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        return;
    }
    sortAscSize(pmemlist);
    searchEmptyMemery(pmemlist, allocatesize, 0);
}

//���Ӧ�㷨
void WF(struct memLinkList *pmemlist, size_t allocatesize)
{
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        return;
    }
    sortDescSize(pmemlist);
    searchEmptyMemery(pmemlist, allocatesize, 0);
}



//ѭ���״���Ӧ�㷨
void NF(struct memLinkList *pmemlist, size_t allocatesize)
{
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        //�����Ҫ����Ŀռ�Ϊ0 ֱ�ӷ���
        return;
    }
    sortAscAdr(pmemlist);
    searchEmptyMemery(pmemlist, allocatesize, 1);
}



void allocateMemory(struct memLinkList *pmemlist, int n)
{
    size_t allocatesize = 0;
    printf("������Ҫ������ڴ��С:");
    scanf("%lu", &allocatesize);
    switch (n) {
        case 1:
            //�״���Ӧ�㷨
            FF(pmemlist, allocatesize);
            break;
        case 2:
            //ѭ���״���Ӧ�㷨
            NF(pmemlist, allocatesize);
            break;
        case 3:
            //�����Ӧ�㷨
            BF(pmemlist, allocatesize);
            break;
        case 4:
            //���Ӧ�㷨
            WF(pmemlist, allocatesize);
            break;
        default:
            printf("�������\n");
            break;
    }
}

void recoveryMemory(struct memLinkList *pmemlist)
{
    assert(pmemlist != NULL);
    size_t membegin = 0;
    printf("������Ҫ���յ��ڴ����ʼ��ַ:");
    scanf("%lu", &membegin);
    //֪����ʼ��ַ�Ժ����ѷ����ڴ������в��ң��ҵ��󣬽���ȡ������
    //Ȼ�󽫿���������ַ�������������Ҫ���յĲ��������У�
    //���п��ܵĺϲ�
    struct Node *ret = searchUsedMemery(pmemlist, membegin);
    //���ѷ����������ҵ�Ҫ���յ�
    if (ret == NULL) {
        return;
    }
    sortAscAdr(pmemlist);   //����ַ����
    emptyMemeryPush(pmemlist, ret);  //���롡�� �ϲ�
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
    printf("������ѡ��\n");
    printf("1.�״���Ӧ�㷨  2.ѭ���״���Ӧ�㷨  3.�����Ӧ�㷨  4.���Ӧ�㷨 : \n");
    scanf("%d", &n);       //û�н������������ֱ�ӵ��������ȷ
    while (1) {
        printList(&memlist);
        printf("��ѡ����: ");
        printf("1.����ռ�    2.���տռ�   3.�˳�: ");
        scanf("%d", &i);
        switch (i) {
            case 1:
                allocateMemory(&memlist, n);
                break;
            case 2:
                recoveryMemory(&memlist);
                break;
            case 3:
                printf("�����˳���\n");
                return 0;
            default:
                printf("ѡ�������������������룡\n");
                break;
        }
    }
    return 0;
}
