// 链栈（不带头结点）：用单链表的头部作为栈顶
// 入栈/出栈都在链表头部进行，时间复杂度 O(1)，不存在栈满（上溢）问题
#include <stdio.h>
#include <stdlib.h>

typedef struct StackNode {
    int data;                 // 数据域
    struct StackNode *next;   // 指针域
} StackNode, *LiStack;

// 初始化：空栈用 NULL 表示（栈顶指针就是链表头指针）
void InitStack(LiStack &S) {
    S = NULL;
}

// 销毁栈：逐个释放结点
void DestroyStack(LiStack &S) {
    while (S != NULL) {
        LiStack temp = S;  // 记住栈顶结点
        S = S->next;       // 栈顶指针下移
        free(temp);
    }
}

// 判空
bool EmptyStack(LiStack S) {
    return S == NULL;
}

// 求栈长
int StackLength(LiStack S) {
    int len = 0;
    for (StackNode *p = S; p != NULL; p = p->next) {
        len++;
    }
    return len;
}

// 入栈：新结点成为新的栈顶
bool Push(LiStack &S, int x) {
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));  // 创建新结点
    if (newNode == NULL) {
        return false;  // 内存分配失败
    }
    newNode->data = x;
    newNode->next = S;  // 新结点指向原栈顶
    S = newNode;        // 栈顶指针移到新结点
    return true;
}

// 出栈：删除栈顶结点，用 x 带回其数据
bool Pop(LiStack &S, int &x) {
    if (EmptyStack(S)) {
        return false;  // 栈空，无法出栈
    }
    StackNode *temp = S;
    x = S->data;
    S = S->next;
    free(temp);
    return true;
}

// 读取栈顶元素（不出栈）
bool GetTop(LiStack S, int &x) {
    if (EmptyStack(S)) {
        return false;
    }
    x = S->data;
    return true;
}

// 打印链栈（从栈顶到栈底）
void PrintStack(LiStack S) {
    printf("链栈(长度=%d, 从栈顶到栈底)：", StackLength(S));
    for (StackNode *p = S; p != NULL; p = p->next) {
        printf("%d -> ", p->data);
    }
    printf("NULL\n");
}

int main() {
    LiStack S;
    int x;
    InitStack(S);
    printf("初始化后是否为空：%s\n", EmptyStack(S) ? "是" : "否");

    for (int i = 1; i <= 5; i++) {  // 依次入栈 1 2 3 4 5
        Push(S, i);
    }
    PrintStack(S);

    if (GetTop(S, x)) {
        printf("当前栈顶元素：%d\n", x);
    }
    if (Pop(S, x)) {
        printf("出栈元素：%d\n", x);
    }
    PrintStack(S);

    while (!EmptyStack(S)) {  // 全部出栈
        Pop(S, x);
    }
    printf("全部出栈后是否为空：%s，空栈出栈返回：%s\n",
           EmptyStack(S) ? "是" : "否", Pop(S, x) ? "成功" : "失败");

    DestroyStack(S);
    return 0;
}
