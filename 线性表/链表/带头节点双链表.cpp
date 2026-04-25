#include <stdio.h>
#include <stdlib.h>

typedef struct DNode {  // 双链表的结点定义
    int data;  // 数据域
    struct DNode *prior, *next;  // 指针域
} DNode, *DLinkList;

bool InitList (DLinkList &L)  // 初始化双链表
{
    L = (DNode*)malloc(sizeof(DNode));  // 创建头结点
    if (!L) {
        printf("内存分配失败！\n");
        return false;
    }
    L->prior = NULL;  // 头结点的prior指针初始化为NULL
    L->next = NULL;  // 头结点的next指针初始化为NULL
    return true;
}

bool Empty (DLinkList L)  // 判断双链表是否为空
{
    return L->next == NULL;  
}

bool InsertNextDNode (DNode* p, int e)  // 指定节点的后插操作
{
    if (!p) 
        return false;
    DNode* s = (DNode*)malloc(sizeof(DNode));  // 创建新节点
    if (!s) 
        return false;
    s->data = e;  // 设置新节点的数据域
    s->next = p->next;  // 新节点的next指向p的下一个节点
    if (p->next)  // 如果p的下一个节点存在，更新其prior指针
        p->next->prior = s;
    s->prior = p;  // 新节点的prior指向p
    p->next = s;  // p的next指向新节点
    return true;
}

bool InsertPriorDNode (DNode* p, int e)  // 指定节点的前插操作
{
    if (!p || !p->prior)  // 如果p不存在或p没有前驱节点，前插失败
        return false;
    DNode* s = (DNode*)malloc(sizeof(DNode));  // 创建新节点
    if (!s) 
        return false;
    s->data = e;  // 设置新节点的数据域
    InsertNextDNode(p->prior, e);  // 在p的前一个节点之后插入新节点
    return true;
}

bool DeleteDNode (DNode* p)  // 删除指定节点
{
    if (!p || !p->prior)  // 如果p不存在或p没有前驱节点，删除失败
        return false;
    p->prior->next = p->next;  // 更新p的前一个节点的next指针
    if (p->next)  // 如果p的下一个节点存在，更新其prior指针
        p->next->prior = p->prior;
    free(p);  // 释放被删除节点的内存
    return true;
}
