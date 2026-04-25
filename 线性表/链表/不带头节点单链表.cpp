#include <stdio.h>
#include <stdlib.h>

typedef struct Node {  // 单链表的结点定义
    int data;  // 数据域
    struct Node *next;  // 指针域，指向下一个结点
} Node, *LinkList;

bool InitList (LinkList &L)  // 初始化不带头节点单链表
{
    L = NULL;  // 将链表指针初始化为NULL，表示链表为空
    return true;
}

bool Empty (LinkList L)  // 判断单链表是否为空
{
    return L == NULL;  // 如果链表指针为NULL，说明链表为空
}

bool ListInsert (LinkList &L, int i, int e)  // 在单链表的第i个位序插入元素e
{
    if (i < 1)  // 增强健壮性，检查插入位置是否合法
        return false;
    if (i == 1) {  // 插入到链表头部
        Node* s = (Node*)malloc(sizeof(Node));  // 创建新节点
        if (!s) {
            printf("内存分配失败！\n");
            return false;
        }
        s->data = e;  // 设置新节点的数据域
        s->next = L;  // 新节点的next指向原链表头部
        L = s;  // 更新链表头指针
        return true;
    }
    Node* p = L;  // 从链表头部开始查找第i-1个节点
    int j = 1;
    while (p && j < i - 1) {
        p = p->next;
        j++;
    }
    if (!p)  // 如果第i-1个节点不存在，插入失败
        return false;
    Node* s = (Node*)malloc(sizeof(Node));  // 创建新节点
    if (!s) {
        printf("内存分配失败！\n");
        return false;
    }
    s->data = e;  // 设置新节点的数据域
    s->next = p->next;  // 新节点的next指向第i-1个节点的下一个节点
    p->next = s;  // 第i-1个节点的next指向新节点
    return true;
}

bool ListDelete (LinkList &L, int i, int &e)  // 删除单链表的第i个位置的元素
{
    if (i < 1)  // 增强健壮性，检查删除位置是否合法
        return false;
    if (i == 1) {  // 删除链表头部
        if (!L) {  // 如果链表为空，删除失败
            printf("链表为空，无法删除！\n");
            return false;
        }
        Node* p = L;  // 保存要删除的节点指针
        e = p->data;  // 获取被删除节点的数据值
        L = L->next;  // 更新链表头指针
        free(p);  // 释放被删除节点的内存
        return true;
    }
    Node* p = L;  // 从链表头部开始查找第i-1个节点
    int j = 1;
    while (p && j < i - 1) {
        p = p->next;
        j++;
    }
    if (!p || !p->next) {  // 如果第i-1个节点不存在或第i个节点不存在，删除失败
        printf("位置不合法！\n");
        return false;
    }
    Node* q = p->next;  // 保存要删除的节点指针
    e = q->data;  // 获取被删除节点的数据值
    p->next = q->next;  // 第i-1个节点的next指向第i+1个节点
    free(q);  // 释放被删除节点的内存
    return true;
}

int main () 
{
    LinkList L;
    InitList (L);
    
    // 插入元素
    ListInsert (L, 1, 10);  // 在第1个位序插入10
    ListInsert (L, 2, 20);  // 在第2个位序插入20
    ListInsert (L, 3, 30);  // 在第3个位序插入30
    
    // 输出链表元素
    Node* p = L;
    while (p) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
    
    return 0;
}