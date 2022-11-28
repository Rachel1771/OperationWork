#ifndef __HEAD_H__
#define __HEAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//数据类型
struct memeryNode {
	size_t _begin;
	size_t _end;
	size_t _size;
};

typedef struct memeryNode DataType;

//节点结构体
struct Node {
	struct memeryNode _data;
	struct Node* _next;
	struct Node* _prev;
};

//链表结构体
struct LinkList {
	struct Node* _head;
	struct Node* _tail;
};

//内存链表
struct memLinkList {
	struct LinkList _emptylist;     //双向循环链表
	struct LinkList _usedlist;      //双向非循环链表
};

//创建节点
//每个节点里面的数据域应该是起始地址和大小
//
// struct Node {
//    struct memeryNode _data;
//    struct Node* _next;
//    struct Node* _prev;
//};
struct Node* creatNode(size_t begin, size_t size)
{
	struct Node *temp = (struct Node*) malloc(sizeof(struct Node));
	//确保malloc 成功
	assert(temp != NULL);
	temp->_data._begin = begin;
	temp->_data._size = size;
	temp->_data._end = begin + size - 1;
	temp->_next = temp;
	temp->_prev = temp;
	return temp;
}

//输出链表
void printList(struct memLinkList *pmemlist)
{
	assert(pmemlist != NULL);
	struct Node *cur = NULL;
	//输出空闲链表
	cur = pmemlist->_emptylist._head;
	printf("空闲内存链表为：\n");
	while (cur != NULL) {
		printf("|begin:%3lu|    ", cur->_data._begin);
		if (cur == pmemlist->_emptylist._tail) {
			break;
		}
		cur = cur->_next;
	}
	printf("\n");
	cur = pmemlist->_emptylist._head;
	while (cur != NULL) {
		printf("|size :%3lu|", cur->_data._size);
		if (cur == pmemlist->_emptylist._tail) {
			break;
		}
		cur = cur->_next;
		printf("--->");
	}
	printf("\n");
	cur = pmemlist->_emptylist._head;
	while (cur != NULL) {
		printf("|end  :%3lu|    ", cur->_data._end);
		if (cur == pmemlist->_emptylist._tail) {
			break;
		}
		cur = cur->_next;
	}
	printf("\n");
	//输出已分配内存链表
	cur = pmemlist->_usedlist._head;
	printf("已分配内存链表为：\n");
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

//初始化 
//需要初始化　内存的开始地址　和 大小
void memListInit(struct memLinkList *pmemlist, size_t begin, size_t size)
{
	assert(pmemlist != NULL);
	pmemlist->_emptylist._head = NULL;
	pmemlist->_emptylist._tail = NULL;
	pmemlist->_usedlist._head = NULL;
	pmemlist->_usedlist._tail = NULL;
	//先把第一个空的挂在空闲内存链表的第一个
	struct Node *ret = creatNode(begin, size);
	pmemlist->_emptylist._head = ret;
	pmemlist->_emptylist._tail = ret;
}

void Swap(struct Node *pnode1, struct Node *pnode2)
{
	assert(pnode1 != NULL);
	assert(pnode2 != NULL);
	struct memeryNode temp;
	temp = pnode1->_data;
	pnode1->_data = pnode2->_data;
	pnode2->_data = temp;
}

//地址升序排序
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

//内存块大小升序
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


//内存块大小降序
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

//占用中的内存块只需要添加到使用队列的最后一个就行了
void usedMemeryPush(struct memLinkList *pmemlist, struct Node * pnode)
{
	assert(pmemlist != NULL);
	assert(pnode != NULL);
	if (pmemlist->_usedlist._head == NULL) {   //说明已分配的为空
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

struct Node* searchUsedMemery(struct memLinkList *pmemlist, size_t membegin)
{
	struct Node *cur = NULL;
	assert(pmemlist != NULL);
	cur = pmemlist->_usedlist._head;
	while (cur != NULL) {
		if (cur->_data._begin == membegin) {
			//找到了要回收的空间
			if (pmemlist->_usedlist._head == pmemlist->_usedlist._tail) {
				//说明只有一个节点,删除这个节点后链表为空
				pmemlist->_usedlist._head = NULL;
				pmemlist->_usedlist._tail = NULL;
				break;
			}
			if (cur->_prev == NULL) {      //找到的是第一个
				pmemlist->_usedlist._head = cur->_next;
			}
			if (cur->_next == NULL) {      //找到的是最后一个
				pmemlist->_usedlist._tail = cur->_prev;
			}
			if (cur->_next != NULL) {      //当前节点的下一个的prev指向当前节点的上一个
				cur->_next->_prev = cur->_prev;
			}
			if (cur->_prev != NULL) {      //当前节点的上一个的next指向当前节点的下一个
				cur->_prev->_next = cur->_next;
			}
			break;
		}
		cur = cur->_next;
	}
	if (cur == NULL) {
		printf("没有找到需要回收的内存!\n");
	}
	else {
		cur->_prev = NULL;
		cur->_next = NULL;
	}
	return cur;
}

//可能有合并的情况
void mergeMemery(struct memLinkList *pmemlist, struct Node *pnode)
{
	struct Node *del = NULL;
	assert(pnode != NULL);
	if (pmemlist->_emptylist._head == pmemlist->_emptylist._tail) {
		//只有一个节点
		return;
	}
	//因为插入的时候是按地址插入的  所以它的地址肯定是升序的
	//如果可以合并，那么肯定是两个连续的
	if (pnode->_prev->_data._end + 1 == pnode->_data._begin) {
		//两个是可以合并的  并且合并到pnode->prev上去
		//pnode->prev可能为head pnode也可能为 tail
		//
		pnode->_prev->_next = pnode->_next;
		pnode->_prev->_data._size += pnode->_data._size;
		pnode->_prev->_data._end = pnode->_data._end;
		//pnode 被合并了

		pnode->_next->_prev = pnode->_prev;
		//如果pnode刚好是最后一个
		if (pnode == pmemlist->_emptylist._tail) {
			pmemlist->_emptylist._tail = pnode->_prev;
		}
		//释放掉pnode   pnode可能还能和后面的合并 所以 先保存起来 让合并后的再判断一下
		del = pnode;
		pnode = pnode->_prev;
		free(del);
		del = NULL;
	}
	if (pnode->_data._end + 1 == pnode->_next->_data._begin) {
		//两个是可以合并的
		//这里要删除的是pnode的下一个节点
		del = pnode->_next;
		pnode->_data._size += pnode->_next->_data._size;
		pnode->_data._end = pnode->_next->_data._end;

		pnode->_next->_next->_prev = pnode;
		
		if (pmemlist->_emptylist._tail == pnode->_next) {
			//如果pnode的下一个刚好是最后一个节点 
			pmemlist->_emptylist._tail = pnode;
		}

		pnode->_next = pnode->_next->_next;
		free(del);
		del = NULL;
	}
}


//回收的内存块只需要添加到第一个比他地址高的前面就好
void emptyMemeryPush(struct memLinkList *pmemlist, struct Node * pnode)
{
	assert(pmemlist != NULL);
	assert(pnode != NULL);
	if (pmemlist->_emptylist._head == NULL) {   //说明空闲链表的为空
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
				//如果找到一个内存块的起始地址大于pnode的起始地址，说明pnode 在它的前面
				pnode->_next = cur;
				pnode->_prev = cur->_prev;
				cur->_prev->_next = pnode;
				cur->_prev = pnode;
				if (cur == pmemlist->_emptylist._head) {
					//如果pnode 的begin是最小的
					pmemlist->_emptylist._head = pnode;
				}

				//可能有合并的情况
				mergeMemery(pmemlist, pnode);

				break;
			}
			cur = cur->_next;
			if (cur == pmemlist->_emptylist._head) {
				//从头到尾找了一圈 没有找到
				break;
			}
		}
		if (cur == pmemlist->_emptylist._head) {
			//说明没有找到地址比它大的　它是最后一块
			pnode->_prev = pmemlist->_emptylist._tail;
			pnode->_next = pmemlist->_emptylist._tail->_next;
			pmemlist->_emptylist._tail->_next = pnode;
			pmemlist->_emptylist._tail = pnode;
			//可能有合并的情况
			mergeMemery(pmemlist, pnode);
		}
	}
}

struct Node *prevemptynode = NULL;
//定义一个指向上一次操作的节点　在循环首次适应里面会用到
//如果连续两次都选择循环首次适应算法，那么第二次的适应算法查找的起点是上一次的下一个　
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
			//循环适应算法
			//第一次从头开始
			//后面的都操作的起点是上次操作节点的下一个
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
				//找到了需要分配的空间 
				prevemptynode = cur->_next;
				if (cur->_data._size == allocatesize) {
					//如果相等的话，不需要创建新的
					//直接把当前的挂在占用的内存链表的最后一个即可
					if (cur == pmemlist->_emptylist._head) {      //找到的是第一个
						pmemlist->_emptylist._head = cur->_next;
					}
					if (cur == pmemlist->_emptylist._tail) {      //找到的是最后一个
						pmemlist->_emptylist._tail = cur->_prev;
					}
					//当前节点的下一个的prev指向当前节点的上一个
					cur->_next->_prev = cur->_prev;
					//当前节点的上一个的next指向当前节点的下一个
					cur->_prev->_next = cur->_next;
					cur->_prev = NULL;
					cur->_next = NULL;
					if (pmemlist->_emptylist._head == pmemlist->_emptylist._tail && cur == pmemlist->_emptylist._head) {
						//找完之后的头等于尾
						//说明空闲链表中就只有一个 并且被分配出去了
						pmemlist->_emptylist._head = NULL;
						pmemlist->_emptylist._tail = NULL;
						//链表为空以后，prevemptynode也就为空了
						prevemptynode = NULL;
					}
					//cur指向的就是需要加到usedmemlist中去的
					temp = cur;
				}
				else {
					//找到的第一个大于需要的，需要从中分配一段空间 
					temp = creatNode(cur->_data._begin, allocatesize);
					//分配后空闲内存的节点需要发生改变
					cur->_data._begin = cur->_data._begin + allocatesize;
					cur->_data._size -= allocatesize;
					temp->_prev = NULL;
					temp->_next = NULL;
				}
				//尾插到使用的最后一个
				usedMemeryPush(pmemlist, temp);
				break;
			}
			cur = cur->_next;
			if (isloop == 1 && cur == prevemptynode) {
				//能走到这，就说明
				//找了一圈 没找到
				break;
			}
			if (isloop == 0 && cur == pmemlist->_emptylist._head) {
				break;
			}
	}
	if (temp == NULL) {
		printf("没有可分配的内存!\n");
	}
}

#endif
