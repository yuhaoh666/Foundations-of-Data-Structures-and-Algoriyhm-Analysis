// 带头结点的循环单链表：尾结点的 next 指向头结点，整条链表首尾相接成一个环
// 从任一结点出发都能遍历全表；判尾条件不再是 next == NULL，而是 next == 头结点
#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {   // 循环单链表结点
    int data;            // 数据域
    struct LNode *next;  // 指针域
} LNode, *LinkList;

// 初始化：创建头结点，并让它的 next 指向自己
bool InitList(LinkList &L) {
    L = (LNode *)malloc(sizeof(LNode));  // 创建头结点
    if (L == NULL) {
        printf("内存分配失败！\n");
        return false;
    }
    L->next = L;  // 头结点的 next 指向自己，表示空表
    return true;
}

// 判空：头结点的 next 指向自己
bool Empty(LinkList L) {
    return L->next == L;
}

// 判断结点 p 是否为尾结点
bool IsTail(LinkList L, LNode *p) {
    return p->next == L;
}

// 求表长：从首元结点出发，回到头结点即结束
int Length(LinkList L) {
    int len = 0;
    for (LNode *p = L->next; p != L; p = p->next) {
        len++;
    }
    return len;
}

// 按位查找：返回第 i 个结点（位序从 1 开始），不存在返回 NULL
LNode *GetElem(LinkList L, int i) {
    if (i < 1) {
        return NULL;
    }
    LNode *p = L->next;
    int j = 1;
    while (p != L && j < i) {  // 绕回头结点说明已经走完一圈，位序越界
        p = p->next;
        j++;
    }
    return (p == L) ? NULL : p;
}

// 按位插入：在第 i 个位序插入元素 e（头结点视为第 0 个结点）
bool ListInsert(LinkList &L, int i, int e) {
    if (i < 1) {
        return false;
    }
    LNode *p = L;  // 从头结点开始找第 i-1 个结点
    int j = 0;
    while (j < i - 1 && p->next != L) {
        p = p->next;
        j++;
    }
    if (j != i - 1) {  // 位序越界（i > 表长 + 1）
        printf("插入位置 %d 不合法！\n", i);
        return false;
    }
    LNode *s = (LNode *)malloc(sizeof(LNode));
    if (s == NULL) {
        return false;
    }
    s->data = e;
    s->next = p->next;  // 新结点接管 p 的后继
    p->next = s;        // p 的后继指向新结点（i=1 时即为头插）
    return true;
}

// 按位删除：删除第 i 个位序的结点，用 e 返回其值
bool ListDelete(LinkList &L, int i, int &e) {
    if (i < 1) {
        return false;
    }
    LNode *p = L;
    int j = 0;
    while (j < i - 1 && p->next != L) {
        p = p->next;
        j++;
    }
    if (j != i - 1 || p->next == L) {  // 第 i 个结点不存在
        printf("删除位置 %d 不合法！\n", i);
        return false;
    }
    LNode *q = p->next;
    e = q->data;
    p->next = q->next;  // 摘链
    free(q);
    return true;
}

// 打印循环单链表
void PrintList(LinkList L) {
    printf("循环单链表(长度=%d)：HEAD -> ", Length(L));
    for (LNode *p = L->next; p != L; p = p->next) {
        printf("%d -> ", p->data);
    }
    printf("HEAD(回到头结点)\n");
}

// 销毁循环单链表：next 构成环，不能用“p != NULL”作为结束条件
void DestroyList(LinkList &L) {
    if (L == NULL) {
        return;
    }
    LNode *p = L->next;
    while (p != L) {
        LNode *q = p->next;
        free(p);
        p = q;
    }
    free(L);  // 最后释放头结点
    L = NULL;
}

int main() {
    LinkList L;
    int e;
    InitList(L);
    printf("初始化后是否为空：%s\n", Empty(L) ? "是" : "否");

    for (int i = 1; i <= 4; i++) {  // 依次插入 10 20 30 40
        ListInsert(L, i, i * 10);
    }
    PrintList(L);

    ListInsert(L, 1, 5);   // 头插
    ListInsert(L, 6, 50);  // 尾插
    ListInsert(L, 3, 15);  // 中间插入
    ListInsert(L, 99, 0);  // 越界插入应被拦截
    PrintList(L);

    LNode *tail = GetElem(L, Length(L));
    printf("最后一个结点 %d 是尾结点：%s\n", tail->data, IsTail(L, tail) ? "是" : "否");

    if (ListDelete(L, 1, e)) {  // 删除首元结点
        printf("被删除的元素：%d\n", e);
    }
    if (ListDelete(L, Length(L), e)) {  // 删除尾结点
        printf("被删除的元素：%d\n", e);
    }
    PrintList(L);

    DestroyList(L);
    return 0;
}
