// 共享栈：两个顺序栈共享同一片存储空间，分别从两端向中间生长
// 只有当 top0 + 1 == top1 时才真正“栈满”，比两个独立栈更充分地利用了空间
#include <stdio.h>
#define Maxsize 100

typedef struct {
    int data[Maxsize];  // 两个栈共用的存储空间
    int top0;           // 0 号栈的栈顶指针，初始为 -1（从低地址向高地址生长）
    int top1;           // 1 号栈的栈顶指针，初始为 Maxsize（从高地址向低地址生长）
} ShStack;

// 初始化：两个栈顶指针分别指向各自方向的“栈底之外”
void InitStack(ShStack &S) {
    S.top0 = -1;
    S.top1 = Maxsize;
}

// 判满：两个栈顶指针相邻（中间已经没有空位）
bool IsFull(ShStack S) {
    return S.top0 + 1 == S.top1;
}

// 判空：两个栈都空才算整体为空
bool IsEmpty(ShStack S) {
    return S.top0 == -1 && S.top1 == Maxsize;
}

// 0 号栈入栈
bool Push0(ShStack &S, int x) {
    if (IsFull(S)) {
        return false;
    }
    S.data[++S.top0] = x;
    return true;
}

// 0 号栈出栈
bool Pop0(ShStack &S, int &x) {
    if (S.top0 == -1) {
        return false;
    }
    x = S.data[S.top0--];
    return true;
}

// 1 号栈入栈
bool Push1(ShStack &S, int x) {
    if (IsFull(S)) {
        return false;
    }
    S.data[--S.top1] = x;
    return true;
}

// 1 号栈出栈
bool Pop1(ShStack &S, int &x) {
    if (S.top1 == Maxsize) {
        return false;
    }
    x = S.data[S.top1++];
    return true;
}

// 打印两个栈的内容
void PrintStack(ShStack S) {
    printf("0 号栈(从栈底到栈顶)：");
    for (int i = 0; i <= S.top0; i++) {
        printf("%d ", S.data[i]);
    }
    printf("\n1 号栈(从栈底到栈顶)：");
    for (int i = Maxsize - 1; i >= S.top1; i--) {
        printf("%d ", S.data[i]);
    }
    printf("\n");
}

int main() {
    ShStack S;
    int x;
    InitStack(S);
    printf("初始化后是否为空：%s\n", IsEmpty(S) ? "是" : "否");

    for (int i = 1; i <= 3; i++) {  // 0 号栈入栈 1 2 3
        Push0(S, i);
    }
    for (int i = 9; i >= 7; i--) {  // 1 号栈入栈 7 8 9
        Push1(S, i);
    }
    PrintStack(S);

    if (Pop0(S, x)) {
        printf("0 号栈出栈元素：%d\n", x);
    }
    if (Pop1(S, x)) {
        printf("1 号栈出栈元素：%d\n", x);
    }
    PrintStack(S);

    while (Push0(S, 0)) {  // 不断向 0 号栈压入元素，直到两个栈相遇
    }
    printf("填充到栈满后，IsFull = %s\n", IsFull(S) ? "true" : "false");

    return 0;
}
