#ifndef __STRUCT_H__
#define __STRUCT_H__
#include "iostream"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//��������
struct memeryNode {
    size_t _begin;
    size_t _end;
    size_t _size;
};

typedef struct memeryNode DataType;

//�ڵ�ṹ��
struct Node {
    struct memeryNode _data; //DataType
    struct Node* _next;
    struct Node* _prev;
};

//����ṹ��
struct LinkList {
    struct Node* _head;
    struct Node* _tail;
};

//�ڴ�����
struct memLinkList {
    struct LinkList _emptylist;     //˫��ѭ������
    struct LinkList _usedlist;      //˫���ѭ������
};

//�����ڵ�
//ÿ���ڵ������������Ӧ������ʼ��ַ�ʹ�С
//
// struct Node {
//    struct memeryNode _data;
//    struct Node* _next;
//    struct Node* _prev;
//};
struct Node* creatNode(size_t begin, size_t size)
{
    struct Node *temp = (struct Node*) malloc(sizeof(struct Node));
    //ȷ��malloc �ɹ�
    assert(temp != NULL);
    temp->_data._begin = begin;
    temp->_data._size = size;
    temp->_data._end = begin + size - 1;
    temp->_next = temp;
    temp->_prev = temp;
    return temp;
}

//�������
void printList(struct memLinkList *pmemlist)
{
    assert(pmemlist != NULL);
    struct Node *cur = NULL;
    //�����������
    cur = pmemlist->_emptylist._head;
    printf("���е�����Ϊ: \n");
    while (cur != NULL) {
        printf("|begin:%3lu|    ", cur->_data._begin);	//�����ʼ��ַ
        if (cur == pmemlist->_emptylist._tail) {
            break;
        }
        cur = cur->_next;
    }
    printf("\n");
    cur = pmemlist->_emptylist._head;
    while (cur != NULL) {
        printf("|size :%3lu|", cur->_data._size);	//�����С
        if (cur == pmemlist->_emptylist._tail) {
            break;
        }
        cur = cur->_next;
        printf("--->");
    }
    printf("\n");
    cur = pmemlist->_emptylist._head;
    while (cur != NULL) {
        printf("|end  :%3lu|    ", cur->_data._end);	//���������ַ
        if (cur == pmemlist->_emptylist._tail) {
            break;
        }
        cur = cur->_next;
    }
    printf("\n");
    //����ѷ����ڴ�����
    cur = pmemlist->_usedlist._head;
    printf("�ѷ��������Ϊ: \n");
    while (cur != NULL) {
        printf("|begin:%3lu|    ", cur->_data._begin);
        cur = cur->_next;
    }
    printf("\n");
    cur = pmemlist->_usedlist._head;
    while (cur != NULL) {
        printf("|size :%3lu|", cur->_data._size);
        if (cur == pmemlist->_usedlist._tail) {
            break;
        }
        cur = cur->_next;
        printf("--->");
    }
    printf("\n");
    cur = pmemlist->_usedlist._head;
    while (cur != NULL) {
        printf("|end  :%3lu|    ", cur->_data._end);
        cur = cur->_next;
    }
    printf("\n");
}

//��ʼ��
//��Ҫ��ʼ�����ڴ�Ŀ�ʼ��ַ���� ��С
void memListInit(struct memLinkList *pmemlist, size_t begin, size_t size)
{
    assert(pmemlist != NULL);
    pmemlist->_emptylist._head = NULL;
    pmemlist->_emptylist._tail = NULL;
    pmemlist->_usedlist._head = NULL;
    pmemlist->_usedlist._tail = NULL;
    //�Ȱѵ�һ���յĹ��ڿ����ڴ�����ĵ�һ��
    struct Node *ret = creatNode(begin, size);
    pmemlist->_emptylist._head = ret;
    pmemlist->_emptylist._tail = ret;
}

//����data
void Swap(struct Node *pnode1, struct Node *pnode2)
{
    assert(pnode1 != NULL);
    assert(pnode2 != NULL);
    struct memeryNode temp;
    temp = pnode1->_data;
    pnode1->_data = pnode2->_data;
    pnode2->_data = temp;
}

//��ַ��������
void sortAscAdr(struct memLinkList *pmemlist)
{
    assert(pmemlist != NULL);
    struct Node *cur = NULL;
    if (pmemlist->_emptylist._head == NULL) {
        return;
    }
    cur = pmemlist->_emptylist._head;
    while (cur != pmemlist->_emptylist._tail) {
        if (cur->_data._begin > cur->_next->_data._begin) {
            Swap(cur, cur->_next);
        }
        cur = cur->_next;
    }
}

//�ڴ���С����
void sortAscSize(struct memLinkList *pmemlist)
{
    assert(pmemlist != NULL);
    struct Node *cur = NULL;
    cur = pmemlist->_emptylist._head;
    while (cur != pmemlist->_emptylist._tail) {
        if (cur->_data._size > cur->_next->_data._size) {
            Swap(cur, cur->_next);
        }
        cur = cur->_next;
    }
}


//�ڴ���С����
void sortDescSize(struct memLinkList *pmemlist)
{
    assert(pmemlist != NULL);
    struct Node *cur = NULL;
    cur = pmemlist->_emptylist._head;
    while (cur != pmemlist->_emptylist._tail) {
        if (cur->_data._size < cur->_next->_data._size) {
            Swap(cur, cur->_next);
        }
        cur = cur->_next;
    }
}

//ռ���е��ڴ��ֻ��Ҫ��ӵ�ʹ�ö��е����һ��������
void usedMemeryPush(struct memLinkList *pmemlist, struct Node * pnode)
{
    assert(pmemlist != NULL);
    assert(pnode != NULL);
    if (pmemlist->_usedlist._head == NULL) {   //˵���ѷ����Ϊ��
        pmemlist->_usedlist._head = pnode;
        pmemlist->_usedlist._tail = pnode;
        pnode->_prev = NULL;
        pnode->_next = NULL;
    }
    else {
        pnode->_prev = pmemlist->_usedlist._tail;
        pnode->_next = NULL;
        pmemlist->_usedlist._tail->_next = pnode;
        pmemlist->_usedlist._tail = pnode;
    }
}

//��ѯ�Ѿ�ʹ�õ��ڴ�
struct Node* searchUsedMemery(struct memLinkList *pmemlist, size_t membegin)
{
    struct Node *cur = NULL;
    assert(pmemlist != NULL);
    cur = pmemlist->_usedlist._head;
    while (cur != NULL) {
        if (cur->_data._begin == membegin) {
            //�ҵ���Ҫ���յĿռ�
            if (pmemlist->_usedlist._head == pmemlist->_usedlist._tail) {
                //˵��ֻ��һ���ڵ�,ɾ������ڵ������Ϊ��
                pmemlist->_usedlist._head = NULL;
                pmemlist->_usedlist._tail = NULL;
                break;
            }
            if (cur->_prev == NULL) {      //�ҵ����ǵ�һ��
                pmemlist->_usedlist._head = cur->_next;
            }
            if (cur->_next == NULL) {      //�ҵ��������һ��
                pmemlist->_usedlist._tail = cur->_prev;
            }
            if (cur->_next != NULL) {      //��ǰ�ڵ����һ����prevָ��ǰ�ڵ����һ��
                cur->_next->_prev = cur->_prev;
            }
            if (cur->_prev != NULL) {      //��ǰ�ڵ����һ����nextָ��ǰ�ڵ����һ��
                cur->_prev->_next = cur->_next;
            }
            break;
        }
        cur = cur->_next;
    }
    if (cur == NULL) {
        printf("û���ҵ���Ҫ���յ��ڴ�!\n");
    }
    else {
        cur->_prev = NULL;
        cur->_next = NULL;
    }
    return cur;
}

//�����кϲ������
void mergeMemery(struct memLinkList *pmemlist, struct Node *pnode)
{
    struct Node *del = NULL;
    assert(pnode != NULL);
    if (pmemlist->_emptylist._head == pmemlist->_emptylist._tail) {
        //ֻ��һ���ڵ�
        return;
    }
    //��Ϊ�����ʱ���ǰ���ַ�����  �������ĵ�ַ�϶��������
    //������Ժϲ�����ô�϶�������������
    if (pnode->_prev->_data._end + 1 == pnode->_data._begin) {
        //�����ǿ��Ժϲ���  ���Һϲ���pnode->prev��ȥ
        //pnode->prev����Ϊhead pnodeҲ����Ϊ tail
        //
        pnode->_prev->_next = pnode->_next;
        pnode->_prev->_data._size += pnode->_data._size;
        pnode->_prev->_data._end = pnode->_data._end;
        //pnode ���ϲ���

        pnode->_next->_prev = pnode->_prev;
        //���pnode�պ������һ��
        if (pnode == pmemlist->_emptylist._tail) {
            pmemlist->_emptylist._tail = pnode->_prev;
        }
        //�ͷŵ�pnode   pnode���ܻ��ܺͺ���ĺϲ� ���� �ȱ������� �úϲ�������ж�һ��
        del = pnode;
        pnode = pnode->_prev;
        free(del);
        del = NULL;
    }
    if (pnode->_data._end + 1 == pnode->_next->_data._begin) {
        //�����ǿ��Ժϲ���
        //����Ҫɾ������pnode����һ���ڵ�
        del = pnode->_next;
        pnode->_data._size += pnode->_next->_data._size;
        pnode->_data._end = pnode->_next->_data._end;

        pnode->_next->_next->_prev = pnode;

        if (pmemlist->_emptylist._tail == pnode->_next) {
            //���pnode����һ���պ������һ���ڵ�
            pmemlist->_emptylist._tail = pnode;
        }

        pnode->_next = pnode->_next->_next;
        free(del);
        del = NULL;
    }
}


//���յ��ڴ��ֻ��Ҫ��ӵ���һ��������ַ�ߵ�ǰ��ͺ�
void emptyMemeryPush(struct memLinkList *pmemlist, struct Node * pnode)
{
    assert(pmemlist != NULL);
    assert(pnode != NULL);
    if (pmemlist->_emptylist._head == NULL) {   //˵�����������Ϊ��
        pmemlist->_emptylist._head = pnode;
        pmemlist->_emptylist._tail = pnode;
        pnode->_prev = pnode;
        pnode->_next = pnode;
    }
    else {
        struct Node *cur = NULL;
        cur = pmemlist->_emptylist._head;
        while (cur != NULL) {
            if (cur->_data._begin > pnode->_data._begin) {
                //����ҵ�һ���ڴ�����ʼ��ַ����pnode����ʼ��ַ��˵��pnode ������ǰ��
                pnode->_next = cur;
                pnode->_prev = cur->_prev;
                cur->_prev->_next = pnode;
                cur->_prev = pnode;
                if (cur == pmemlist->_emptylist._head) {
                    //���pnode ��begin����С��
                    pmemlist->_emptylist._head = pnode;
                }

                //�����кϲ������
                mergeMemery(pmemlist, pnode);

                break;
            }
            cur = cur->_next;
            if (cur == pmemlist->_emptylist._head) {
                //��ͷ��β����һȦ û���ҵ�
                break;
            }
        }
        if (cur == pmemlist->_emptylist._head) {
            //˵��û���ҵ���ַ������ġ��������һ��
            pnode->_prev = pmemlist->_emptylist._tail;
            pnode->_next = pmemlist->_emptylist._tail->_next;
            pmemlist->_emptylist._tail->_next = pnode;
            pmemlist->_emptylist._tail = pnode;
            //�����кϲ������
            mergeMemery(pmemlist, pnode);
        }
    }
}

struct Node *prevemptynode = NULL;
//����һ��ָ����һ�β����Ľڵ㡡��ѭ���״���Ӧ������õ�
//����������ζ�ѡ��ѭ���״���Ӧ�㷨����ô�ڶ��ε���Ӧ�㷨���ҵ��������һ�ε���һ����
void searchEmptyMemery(struct memLinkList *pmemlist, size_t allocatesize, int isloop)
{
    struct Node *cur = NULL;
    struct Node *temp = NULL;
    assert(pmemlist != NULL);
    if (allocatesize == 0) {
        return;
    }
    if (isloop) {
        if (prevemptynode == NULL) {
            //ѭ����Ӧ�㷨
            //��һ�δ�ͷ��ʼ
            //����Ķ�������������ϴβ����ڵ����һ��
            cur = pmemlist->_emptylist._head;
            prevemptynode = pmemlist->_emptylist._head;
        }
        else {
            cur = prevemptynode;
        }
    }
    else {
        cur = pmemlist->_emptylist._head;
    }
    while (cur != NULL) {
        if (cur->_data._size >= allocatesize) {
            //�ҵ�����Ҫ����Ŀռ�
            prevemptynode = cur->_next;
            if (cur->_data._size == allocatesize) {
                //�����ȵĻ�������Ҫ�����µ�
                //ֱ�Ӱѵ�ǰ�Ĺ���ռ�õ��ڴ���������һ������
                if (cur == pmemlist->_emptylist._head) {      //�ҵ����ǵ�һ��
                    pmemlist->_emptylist._head = cur->_next;
                }
                if (cur == pmemlist->_emptylist._tail) {      //�ҵ��������һ��
                    pmemlist->_emptylist._tail = cur->_prev;
                }
                //��ǰ�ڵ����һ����prevָ��ǰ�ڵ����һ��
                cur->_next->_prev = cur->_prev;
                //��ǰ�ڵ����һ����nextָ��ǰ�ڵ����һ��
                cur->_prev->_next = cur->_next;
                cur->_prev = NULL;
                cur->_next = NULL;
                if (pmemlist->_emptylist._head == pmemlist->_emptylist._tail && cur == pmemlist->_emptylist._head) {
                    //����֮���ͷ����β
                    //˵�����������о�ֻ��һ�� ���ұ������ȥ��
                    pmemlist->_emptylist._head = NULL;
                    pmemlist->_emptylist._tail = NULL;
                    //����Ϊ���Ժ�prevemptynodeҲ��Ϊ����
                    prevemptynode = NULL;
                }
                //curָ��ľ�����Ҫ�ӵ�usedmemlist��ȥ��
                temp = cur;
            }
            else {
                //�ҵ��ĵ�һ��������Ҫ�ģ���Ҫ���з���һ�οռ�
                temp = creatNode(cur->_data._begin, allocatesize);
                //���������ڴ�Ľڵ���Ҫ�����ı�
                cur->_data._begin = cur->_data._begin + allocatesize;
                cur->_data._size -= allocatesize;
                temp->_prev = NULL;
                temp->_next = NULL;
            }
            //β�嵽ʹ�õ����һ��
            usedMemeryPush(pmemlist, temp);
            break;
        }
        cur = cur->_next;
        if (isloop == 1 && cur == prevemptynode) {
            //���ߵ��⣬��˵��
            //����һȦ û�ҵ�
            break;
        }
        if (isloop == 0 && cur == pmemlist->_emptylist._head) {
            break;
        }
    }
    if (temp == NULL) {
        printf("û�пɷ�����ڴ�!\n");
    }
}

#endif
