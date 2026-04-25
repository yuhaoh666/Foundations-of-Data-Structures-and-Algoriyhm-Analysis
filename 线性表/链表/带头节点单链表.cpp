#include <stdio.h>
#include <stdlib.h>

typedef struct Node {  // 单链表的定义
    int data;
    struct Node *next;
} Node, *LinkList;

bool InitList (LinkList &L)  // 初始化带头节点单链表
{
    L = (Node*)malloc(sizeof(Node));  // 创建头结点
    if (!L) {
        printf("内存分配失败！\n");
        return false;
    }
    L->next = NULL;  // 头结点的next指针初始化为NULL
    return true;
}

bool Empty (LinkList L)  // 判断单链表是否为空
{
    return L->next == NULL;  
}

Node* GetElem (LinkList L, int i)  // 获取单链表的第i个元素
{
    if (i < 1)  // 增强健壮性，检查位置是否合法
        return NULL;
    Node* p = L->next;  // 从第一个节点开始查找
    int j = 1;
    while (p && j < i) {
        p = p->next;
        j++;
    }
    if (!p)  // 如果第i个节点不存在，返回NULL
        return NULL;
    return p;  // 返回第i个节点的指针
}

Node* LocateElem (LinkList L, int e)  // 查找单链表中第一个值为e的节点
{
    Node* p = L->next;  // 从第一个节点开始查找
    while (p) {
        if (p->data == e) {  // 结构类型的比较需要重载==运算符
            return p;  // 返回找到的节点的指针
        }
        p = p->next;
    }
    return NULL;  // 返回NULL表示未找到
}

bool ListInsertNextNode (Node* p, int e)  // 指定节点的后插操作
{
    if (!p) 
        return false;
    Node* s = (Node*)malloc(sizeof(Node));  // 创建新节点
    if (!s) 
        return false;
    s->data = e;  // 设置新节点的数据域
    s->next = p->next;  // 新节点的next指向p的下一个节点
    p->next = s;  // p的next指向新节点
    return true;
}

bool ListInsertPriorNode (Node* p, int e)  // 指定节点的前插操作
{
    if (!p) 
        return false;
    Node* s = (Node*)malloc(sizeof(Node));  // 创建新节点
    if (!s) 
        return false;
    s->data = p->data;  // 将p节点的数据域值赋给新节点
    s->next = p->next;  // 新节点的next指向p的下一个节点
    p->next = s;  // p的next指向新节点
    p->data = e;  // 将e赋值给p节点的数据域，实现前插操作
    return true;
}

bool ListInsert (LinkList &L, int i, int e)  // 在单链表的第i个位序插入元素e
{
    Node* p = GetElem(L, i - 1);  // 获取第i-1个节点的指针
    if (!p)  // 如果第i-1个节点不存在，插入失败
        return false;
    return ListInsertNextNode(p, e);  // 在第i-1个节点之后插入元素e
}

bool DeleteNode (Node* p)  // 删除指定节点，但不能删除最后一个节点
{
    if (!p || !p->next)  // 如果p节点不存在或p节点是最后一个节点，删除失败
        return false;
    Node* q = p->next;  
    p->data = q->data;  
    p->next = q->next;  
    free(q); 
    return true;
}

bool ListDelete (LinkList &L, int i, int &e)  // 删除单链表的第i个位序的元素
{
    Node* p = GetElem(L, i - 1);  // 获取第i-1个节点的指针
    if (!p)  // 如果第i-1个节点不存在，删除失败
        return false;
    Node* q = p->next;  // q指向第i个节点
    e = q->data;  // 获取被删除节点的数据域值
    p->next = q->next;  // 将第i-1个节点的next指针指向第i+1个节点
    free(q);  // 释放被删除节点的内存空间
    return true;
}

LinkList CreateListHead (LinkList &L)  // 头插法创建单链表
{
    InitList(L);  // 初始化链表
    int x;
    scanf("%d", &x);
    while (x != 9999) {  // 输入9999表示结束
        ListInsertNextNode(L, x);  // 在链表头部插入新节点
        scanf("%d", &x);
    }
    return L;
}

LinkList CreateListTail (LinkList &L)  // 尾插法创建单链表
{
    InitList(L);  // 初始化链表
    Node* r = L;  // r指向链表的最后一个节点，初始时指向头结点
    int x;
    scanf("%d", &x);
    while (x != 9999) {  // 输入9999表示结束
        Node* s = (Node*)malloc(sizeof(Node));  // 创建新节点
        if (!s)
            return NULL;
        s->data = x;  // 设置新节点的数据域
        r->next = s;  // 将当前最后一个节点的next指针指向新节点
        r = s;  // r更新为新的最后一个节点
        scanf("%d", &x);
    }
    r->next = NULL;  // 最后一个节点的next指针设置为NULL
    return L;
}

int main () {
    LinkList L;
    InitList (L);
    CreateListHead(L);
    return 0;
}