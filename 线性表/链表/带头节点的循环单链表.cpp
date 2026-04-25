#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {  // 循环单链表
    int data;  // 数据域
    struct LNode *next;  // 指针域
} LNode, *LinkList;

bool InitList (LinkList &L)  // 初始化循环单链表
{
    L = (LNode*)malloc(sizeof(LNode));  // 创建头结点
    if (!L) {
        printf("内存分配失败！\n");
        return false;
    }
    L->next = L;  // 头结点的next指针指向自己，形成循环
    return true;
}

bool Empty (LinkList L)  // 判断循环单链表是否为空
{
    return L->next == L;  // 如果头结点的next指针指向自己，说明链表为空
}

bool isTail (LinkList L, LNode* p)  // 判断p是否为循环单链表的尾节点
{
    return p->next == L;  // 如果p的next指针指向头结点，说明p是尾节点
}

int main () {
    LinkList L;
    InitList (L);
    return 0;
}