// 不带头结点的单链表：头指针直接指向第一个数据结点
// 因此对表头元素的插入/删除需要单独处理（一般都要修改头指针 L 本身）
#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {   // 单链表的结点定义
    int data;            // 数据域
    struct LNode *next;  // 指针域，指向下一个结点
} LNode, *LinkList;

// 初始化：用 NULL 表示空表
bool InitList(LinkList &L) {
    L = NULL;
    return true;
}

// 判空
bool Empty(LinkList L) {
    return L == NULL;
}

// 求表长，时间复杂度 O(n)
int Length(LinkList L) {
    int len = 0;
    for (LNode *p = L; p != NULL; p = p->next) {
        len++;
    }
    return len;
}

// 按位查找：返回第 i 个结点（位序从 1 开始），不存在返回 NULL
LNode *GetElem(LinkList L, int i) {
    if (i < 1) {  // 增强健壮性，检查位置是否合法
        return NULL;
    }
    LNode *p = L;
    int j = 1;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    return p;
}

// 按值查找：返回第一个数据域等于 e 的结点，未找到返回 NULL
LNode *LocateElem(LinkList L, int e) {
    LNode *p = L;
    while (p != NULL && p->data != e) {
        p = p->next;
    }
    return p;
}

// 按值查找位序：返回第一个值为 e 的元素的位序，未找到返回 0
int LocatePos(LinkList L, int e) {
    int i = 1;
    for (LNode *p = L; p != NULL; p = p->next, i++) {
        if (p->data == e) {
            return i;
        }
    }
    return 0;
}

// 后插：在结点 p 之后插入元素 e，时间复杂度 O(1)
bool InsertNextNode(LNode *p, int e) {
    if (p == NULL) {
        return false;
    }
    LNode *s = (LNode *)malloc(sizeof(LNode));  // 创建新结点
    if (s == NULL) {
        printf("内存分配失败！\n");
        return false;
    }
    s->data = e;
    s->next = p->next;  // 新结点的 next 指向 p 的后继
    p->next = s;        // p 的 next 指向新结点
    return true;
}

// 按位插入：在第 i 个位序插入元素 e
bool ListInsert(LinkList &L, int i, int e) {
    if (i < 1) {  // 增强健壮性，检查插入位置是否合法
        return false;
    }
    if (i == 1) {  // 插入到表头，必须修改头指针 L
        LNode *s = (LNode *)malloc(sizeof(LNode));
        if (s == NULL) {
            printf("内存分配失败！\n");
            return false;
        }
        s->data = e;
        s->next = L;  // 新结点的 next 指向原首元结点
        L = s;        // 头指针指向新结点
        return true;
    }
    LNode *p = GetElem(L, i - 1);  // 找到第 i-1 个结点
    return InsertNextNode(p, e);   // 在其后插入（p 为 NULL 时返回 false）
}

// 按位删除：删除第 i 个位序的结点，用 e 返回其值
bool ListDelete(LinkList &L, int i, int &e) {
    if (i < 1) {  // 增强健壮性，检查删除位置是否合法
        return false;
    }
    if (i == 1) {  // 删除表头元素，必须修改头指针 L
        if (L == NULL) {  // 空表无法删除
            printf("链表为空，无法删除！\n");
            return false;
        }
        LNode *p = L;  // 保存要删除的结点
        e = p->data;
        L = L->next;   // 头指针后移
        free(p);       // 释放被删除结点的内存
        return true;
    }
    LNode *p = GetElem(L, i - 1);        // 找到第 i-1 个结点
    if (p == NULL || p->next == NULL) {  // 第 i 个结点不存在
        printf("删除位置 %d 不合法！\n", i);
        return false;
    }
    LNode *q = p->next;  // q 指向被删除结点
    e = q->data;
    p->next = q->next;   // 摘链
    free(q);
    return true;
}

// 用数组建表（尾插法），便于测试
void CreateListByArray(LinkList &L, const int a[], int n) {
    InitList(L);
    LNode *r = NULL;  // r 指向尾结点
    for (int i = 0; i < n; i++) {
        if (r == NULL) {  // 第一个结点需要修改头指针
            L = (LNode *)malloc(sizeof(LNode));
            if (L == NULL) {
                return;
            }
            L->data = a[i];
            L->next = NULL;
            r = L;
        } else {
            InsertNextNode(r, a[i]);
            r = r->next;
        }
    }
}

// 打印单链表
void PrintList(LinkList L) {
    printf("单链表(长度=%d)：", Length(L));
    for (LNode *p = L; p != NULL; p = p->next) {
        printf("%d -> ", p->data);
    }
    printf("NULL\n");
}

// 销毁单链表
void DestroyList(LinkList &L) {
    LNode *p = L;
    while (p != NULL) {
        LNode *q = p->next;  // 先记住后继，再释放当前结点
        free(p);
        p = q;
    }
    L = NULL;
}

int main() {
    LinkList L;
    int e;
    int a[] = {10, 20, 30};

    CreateListByArray(L, a, 3);  // 尾插法建表：10 20 30
    PrintList(L);

    ListInsert(L, 1, 5);   // 头插，得到 5 10 20 30
    ListInsert(L, 5, 40);  // 尾插，得到 5 10 20 30 40
    ListInsert(L, 3, 15);  // 中间插入，得到 5 10 15 20 30 40
    PrintList(L);

    printf("值 20 的位序：%d\n", LocatePos(L, 20));

    if (ListDelete(L, 1, e)) {  // 删除表头元素
        printf("被删除的元素：%d\n", e);
    }
    ListDelete(L, 99, e);  // 越界删除应被拦截
    PrintList(L);

    DestroyList(L);
    printf("销毁后表是否为空：%s\n", Empty(L) ? "是" : "否");
    return 0;
}
