#include <stdio.h>
#define Maxsize 100

typedef struct {
    char data[Maxsize];  // 存储空间
    int top;  // 栈顶指针
} SqStack;

void InitStack(SqStack &S) {
    S.top = -1;  // 初始化栈顶指针
}

bool EmptyStack(SqStack S) {
    return S.top == -1;  // 判断栈是否为空
}

int main() {
    SqStack S;
    InitStack(S);
    return 0;
}