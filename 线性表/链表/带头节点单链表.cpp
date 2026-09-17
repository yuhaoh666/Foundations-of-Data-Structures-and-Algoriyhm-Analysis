// 带头结点的单链表：头结点不存放数据，使表头与其他位置的插入/删除操作得到统一
#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {   // 单链表的结点定义
    int data;            // 数据域
    struct LNode *next;  // 指针域，指向下一个结点
} LNode, *LinkList;

// 初始化带头结点单链表：创建头结点
bool InitList(LinkList &L) {
    L = (LNode *)malloc(sizeof(LNode));  // 创建头结点
    if (L == NULL) {
        printf("内存分配失败！\n");
        return false;
    }
    L->next = NULL;  // 头结点的 next 指针初始化为 NULL
    return true;
}

// 判空：头结点的 next 为 NULL 说明表空
bool Empty(LinkList L) {
    return L->next == NULL;
}

// 求表长：时间复杂度 O(n)
int Length(LinkList L) {
    int len = 0;
    for (LNode *p = L->next; p != NULL; p = p->next) {
        len++;
    }
    return len;
}

// 按位查找：返回第 i 个结点，不存在返回 NULL；时间复杂度 O(n)
// 约定头结点是“第 0 个结点”，所以 i 可以取 0（返回头结点），按位插入/删除正是靠它统一处理表头
LNode *GetElem(LinkList L, int i) {
    if (i < 0) {  // 增强健壮性，检查位置是否合法
        return NULL;
    }
    LNode *p = L;  // 从头结点开始查找
    int j = 0;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    return p;  // 第 i 个结点不存在时 p 恰好为 NULL
}

// 按值查找：返回第一个数据域等于 e 的结点，未找到返回 NULL
LNode *LocateElem(LinkList L, int e) {
    LNode *p = L->next;
    while (p != NULL && p->data != e) {  // 结构类型的比较需要重载 != 运算符
        p = p->next;
    }
    return p;
}

// 后插：在结点 p 之后插入值为 e 的新结点，时间复杂度 O(1)
bool InsertNextNode(LNode *p, int e) {
    if (p == NULL) {
        return false;
    }
    LNode *s = (LNode *)malloc(sizeof(LNode));  // 创建新结点
    if (s == NULL) {
        return false;
    }
    s->data = e;        // 填入数据
    s->next = p->next;  // 新结点的 next 指向 p 的后继
    p->next = s;        // p 的 next 指向新结点
    return true;
}

// 前插：在结点 p 之前插入值为 e 的新结点，时间复杂度 O(1)
// 技巧：把新结点插到 p 之后，再交换 p 与新结点的数据域，效果等价于前插
bool InsertPriorNode(LNode *p, int e) {
    if (p == NULL) {
        return false;
    }
    LNode *s = (LNode *)malloc(sizeof(LNode));
    if (s == NULL) {
        return false;
    }
    s->next = p->next;  // 先让新结点接管 p 的后继
    p->next = s;
    s->data = p->data;  // 再交换数据域
    p->data = e;
    return true;
}

// 按位插入：在第 i 个位序插入元素 e，时间复杂度 O(n)
bool ListInsert(LinkList &L, int i, int e) {
    if (i < 1) {
        return false;
    }
    LNode *p = GetElem(L, i - 1);  // 找到第 i-1 个结点（头结点可视为第 0 个结点）
    return InsertNextNode(p, e);   // 在其后插入
}

// 删除指定结点 p 本身（p 不能是尾结点），时间复杂度 O(1)
bool DeleteNode(LNode *p) {
    if (p == NULL || p->next == NULL) {  // 尾结点无法用此方法删除（找不到前驱）
        return false;
    }
    LNode *q = p->next;
    p->data = q->data;  // 用后继结点的数据覆盖 p
    p->next = q->next;  // 摘下后继结点 q
    free(q);
    return true;
}

// 按位删除：删除第 i 个位序的结点，用 e 返回其值，时间复杂度 O(n)
bool ListDelete(LinkList &L, int i, int &e) {
    if (i < 1) {
        return false;
    }
    LNode *p = GetElem(L, i - 1);  // 找到第 i-1 个结点
    if (p == NULL || p->next == NULL) {  // 第 i 个结点不存在（位序越界）
        printf("删除位置 %d 不合法！\n", i);
        return false;
    }
    LNode *q = p->next;   // q 指向被删除结点
    e = q->data;          // 保存被删除结点的数据
    p->next = q->next;    // 摘链
    free(q);              // 释放被删除结点的内存
    return true;
}

// 头插法建立单链表：每读入一个数就插在头结点之后，得到的链表与输入顺序相反
LinkList CreateListHead(LinkList &L) {
    InitList(L);
    int x;
    scanf("%d", &x);
    while (x != 9999) {           // 输入 9999 表示结束
        InsertNextNode(L, x);     // 插在头结点之后即为头插
        scanf("%d", &x);
    }
    return L;
}

// 尾插法建立单链表：用 r 跟踪尾结点，得到的链表与输入顺序一致
LinkList CreateListTail(LinkList &L) {
    InitList(L);
    LNode *r = L;  // r 始终指向链表的尾结点
    int x;
    scanf("%d", &x);
    while (x != 9999) {  // 输入 9999 表示结束
        if (!InsertNextNode(r, x)) {  // 在尾结点之后插入
            return NULL;
        }
        r = r->next;  // 更新尾指针
        scanf("%d", &x);
    }
    return L;
}

// 用给定数组以尾插法建表，便于测试
void CreateListByArray(LinkList &L, const int a[], int n) {
    InitList(L);
    LNode *r = L;
    for (int i = 0; i < n; i++) {
        InsertNextNode(r, a[i]);
        r = r->next;
    }
}

// 打印单链表
void PrintList(LinkList L) {
    printf("单链表(长度=%d)：", Length(L));
    for (LNode *p = L->next; p != NULL; p = p->next) {
        printf("%d -> ", p->data);
    }
    printf("NULL\n");
}

// 销毁单链表：连同头结点一起释放
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
    int a[] = {10, 20, 30, 40};

    CreateListByArray(L, a, 4);  // 尾插法建表：10 20 30 40
    PrintList(L);

    ListInsert(L, 1, 5);   // 头插，得到 5 10 20 30 40
    ListInsert(L, 6, 50);  // 尾插，得到 5 10 20 30 40 50
    PrintList(L);

    InsertPriorNode(LocateElem(L, 30), 25);  // 在 30 之前插入 25
    DeleteNode(GetElem(L, 2));               // 删除第 2 个结点（结点间“偷梁换柱”）
    PrintList(L);

    if (ListDelete(L, 1, e)) {               // 删除表头结点之后的首个数据结点
        printf("被删除的元素：%d\n", e);
    }
    ListDelete(L, 100, e);                   // 越界删除应被拦截
    PrintList(L);

    DestroyList(L);
    return 0;
}
// 需要从键盘输入建表时，可改用：CreateListHead(L) 或 CreateListTail(L)，输入 9999 结束
