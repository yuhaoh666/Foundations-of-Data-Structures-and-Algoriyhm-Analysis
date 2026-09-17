// 带头结点的双链表：每个结点既有前驱指针又有后继指针，可以双向遍历
// 按位查找仍需 O(n)，但找到结点后，前插/后插/删除都只需 O(1)
#include <stdio.h>
#include <stdlib.h>

typedef struct DNode {           // 双链表的结点定义
    int data;                    // 数据域
    struct DNode *prior, *next;  // 前驱、后继指针域
} DNode, *DLinkList;

// 初始化：创建头结点，其 prior 与 next 均置为 NULL
bool InitList(DLinkList &L) {
    L = (DNode *)malloc(sizeof(DNode));  // 创建头结点
    if (L == NULL) {
        printf("内存分配失败！\n");
        return false;
    }
    L->prior = NULL;
    L->next = NULL;
    return true;
}

// 判空
bool Empty(DLinkList L) {
    return L->next == NULL;
}

// 按位查找：返回第 i 个结点（位序从 1 开始），不存在返回 NULL
DNode *GetElem(DLinkList L, int i) {
    if (i < 1) {
        return NULL;
    }
    DNode *p = L->next;
    int j = 1;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    return p;
}

// 后插：在结点 p 之后插入元素 e，时间复杂度 O(1)
bool InsertNextDNode(DNode *p, int e) {
    if (p == NULL) {
        return false;
    }
    DNode *s = (DNode *)malloc(sizeof(DNode));
    if (s == NULL) {
        return false;
    }
    s->data = e;
    s->next = p->next;             // 新结点的后继指向 p 的原后继
    if (p->next != NULL) {         // 若 p 有后继，还要让它的 prior 回指新结点
        p->next->prior = s;
    }
    s->prior = p;                  // 新结点的前驱指向 p
    p->next = s;                   // p 的后继指向新结点
    return true;
}

// 前插：在结点 p 之前插入元素 e
// 双链表可以“向前看”，因此直接复用后插操作即可，不必像单链表那样交换数据域
bool InsertPriorDNode(DNode *p, int e) {
    if (p == NULL || p->prior == NULL) {  // 头结点之前不能插入
        return false;
    }
    return InsertNextDNode(p->prior, e);
}

// 删除 p 结点的后继结点
bool DeleteNextDNode(DNode *p) {
    if (p == NULL || p->next == NULL) {
        return false;
    }
    DNode *q = p->next;
    p->next = q->next;          // 让 p 越过 q 指向其后继
    if (q->next != NULL) {      // 若 q 有后继，让它的 prior 回指 p
        q->next->prior = p;
    }
    free(q);
    return true;
}

// 删除结点 p 本身（p 不能是头结点）
bool DeleteDNode(DNode *p) {
    if (p == NULL || p->prior == NULL) {
        return false;
    }
    return DeleteNextDNode(p->prior);  // 等价于“删除前驱的后继”
}

// 按位插入：在第 i 个位序插入元素 e
bool ListInsert(DLinkList &L, int i, int e) {
    if (i < 1) {
        return false;
    }
    DNode *p = (i == 1) ? L : GetElem(L, i - 1);  // i=1 时插在头结点之后（即头插）
    if (p == NULL) {
        return false;
    }
    return InsertNextDNode(p, e);
}

// 按位删除：删除第 i 个位序的结点，用 e 返回其值
bool ListDelete(DLinkList &L, int i, int &e) {
    if (i < 1) {
        return false;
    }
    DNode *p = GetElem(L, i);  // 直接找到第 i 个结点
    if (p == NULL) {
        printf("删除位置 %d 不合法！\n", i);
        return false;
    }
    e = p->data;  // 先保存数据，再释放结点
    return DeleteDNode(p);
}

// 求表长
int Length(DLinkList L) {
    int len = 0;
    for (DNode *p = L->next; p != NULL; p = p->next) {
        len++;
    }
    return len;
}

// 正向打印（借助 next 指针）
void PrintList(DLinkList L) {
    printf("双链表(正向, 长度=%d)：HEAD <-> ", Length(L));
    for (DNode *p = L->next; p != NULL; p = p->next) {
        printf("%d <-> ", p->data);
    }
    printf("NULL\n");
}

// 取尾结点（空表返回 NULL）
DNode *GetTail(DLinkList L) {
    DNode *p = L;
    while (p->next != NULL) {
        p = p->next;
    }
    return (p == L) ? NULL : p;
}

// 逆向打印（借助 prior 指针从尾结点回退，这是双链表的优势）
void PrintListReverse(DLinkList L) {
    printf("双链表(逆向)：NULL <-> ");
    for (DNode *p = GetTail(L); p != NULL && p != L; p = p->prior) {  // 走到头结点即停
        printf("%d <-> ", p->data);
    }
    printf("HEAD\n");
}

// 销毁双链表
void DestroyList(DLinkList &L) {
    DNode *p = L;
    while (p != NULL) {
        DNode *q = p->next;
        free(p);
        p = q;
    }
    L = NULL;
}

int main() {
    DLinkList L;
    int e;
    InitList(L);

    for (int i = 1; i <= 4; i++) {  // 依次插入 10 20 30 40
        ListInsert(L, i, i * 10);
    }
    PrintList(L);

    InsertNextDNode(GetElem(L, 2), 25);  // 在 20 之后插入 25
    PrintList(L);

    InsertPriorDNode(GetElem(L, 1), 5);  // 在 10 之前插入 5
    PrintList(L);

    DeleteDNode(GetElem(L, 3));  // 删除第 3 个结点（15）
    PrintList(L);

    DeleteNextDNode(L);  // 删除首元结点（5）
    PrintList(L);

    if (ListDelete(L, 2, e)) {  // 按位删除
        printf("被删除的元素：%d\n", e);
    }
    ListDelete(L, 100, e);  // 越界删除应被拦截
    PrintList(L);
    PrintListReverse(L);

    DestroyList(L);
    return 0;
}
