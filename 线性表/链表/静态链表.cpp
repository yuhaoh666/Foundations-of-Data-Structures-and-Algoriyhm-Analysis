// 静态链表（链表的游标实现）：用一维数组模拟链表，结点的“指针”就是数组下标
// 适用于不支持指针的编程语言；约定 space[0] 作为备用链表的头结点
#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int data;  // 数据域
    int next;  // 游标（下一个结点的数组下标），-1 表示链尾
} SLinkList[MAXSIZE];

// 初始化备用链表：把所有结点串成一条空闲链表，结束时 next = -1
void InitSpace(SLinkList space) {
    for (int i = 0; i < MAXSIZE - 1; i++) {
        space[i].next = i + 1;  // 下标 i 指向 i+1，形成备用链表
    }
    space[MAXSIZE - 1].next = -1;  // 最后一个结点之后没有空闲结点了
}

// 分配一个空闲结点，返回其下标；没有空闲结点时返回 -1
int MallocSpace(SLinkList space) {
    int i = space[0].next;  // 备用链表的第一个结点
    if (i != -1) {
        space[0].next = space[i].next;  // 从备用链表中摘下结点 i
    }
    return i;
}

// 回收下标为 k 的结点：把它重新插入备用链表的头部
void FreeSpace(SLinkList space, int k) {
    space[k].next = space[0].next;
    space[0].next = k;
}

// 初始化静态链表：申请一个头结点，用 head 返回它的下标
bool InitList(SLinkList space, int &head) {
    InitSpace(space);
    head = MallocSpace(space);
    if (head == -1) {
        printf("空间分配失败！\n");
        return false;
    }
    space[head].next = -1;  // 头结点的 next 为 -1，表示空表
    return true;
}

// 按位插入：在第 i 个位序插入元素 e（头结点视为第 0 个结点）
bool ListInsert(SLinkList space, int head, int i, int e) {
    if (i < 1) {
        return false;
    }
    int p = head;                       // 从头结点出发找第 i-1 个结点
    for (int j = 1; j < i && p != -1; j++) {
        p = space[p].next;
    }
    if (p == -1) {                      // 位序越界
        printf("插入位置 %d 不合法！\n", i);
        return false;
    }
    int s = MallocSpace(space);         // 申请一个新结点
    if (s == -1) {
        printf("空间已满，无法插入！\n");
        return false;
    }
    space[s].data = e;
    space[s].next = space[p].next;      // 把新结点插到 p 之后
    space[p].next = s;
    return true;
}

// 按位删除：删除第 i 个位序的结点，用 e 返回其值
bool ListDelete(SLinkList space, int head, int i, int &e) {
    if (i < 1) {
        return false;
    }
    int p = head;
    for (int j = 1; j < i && p != -1; j++) {
        p = space[p].next;
    }
    if (p == -1 || space[p].next == -1) {  // 第 i 个结点不存在
        printf("删除位置 %d 不合法！\n", i);
        return false;
    }
    int q = space[p].next;  // q 为被删除结点
    e = space[q].data;
    space[p].next = space[q].next;  // 摘链
    FreeSpace(space, q);            // 把结点归还给备用链表
    return true;
}

// 求表长：沿游标一直走到 -1
int Length(SLinkList space, int head) {
    int len = 0;
    for (int p = space[head].next; p != -1; p = space[p].next) {
        len++;
    }
    return len;
}

// 打印静态链表中的所有元素
void PrintList(SLinkList space, int head) {
    printf("静态链表(长度=%d)：", Length(space, head));
    for (int p = space[head].next; p != -1; p = space[p].next) {
        printf("%d -> ", space[p].data);
        if (p == space[p].next) {  // 防御：避免游标成环导致死循环
            printf("[游标异常] ");
            break;
        }
    }
    printf("-1\n");
}

int main() {
    SLinkList space;  // 数组作为函数实参会自动退化为指针，函数内的修改对外可见
    int head;
    int e;

    if (!InitList(space, head)) {
        return 0;
    }
    printf("初始化后表长：%d\n", Length(space, head));

    for (int i = 1; i <= 5; i++) {  // 依次插入 10 20 30 40 50
        ListInsert(space, head, i, i * 10);
    }
    PrintList(space, head);

    ListInsert(space, head, 3, 25);  // 在第 3 个位序插入 25
    PrintList(space, head);

    if (ListDelete(space, head, 1, e)) {  // 删除首元结点
        printf("被删除的元素：%d\n", e);
    }
    ListDelete(space, head, 99, e);  // 越界删除应被拦截
    PrintList(space, head);

    printf("当前空闲结点数：%d\n", Length(space, 0));
    return 0;
}
