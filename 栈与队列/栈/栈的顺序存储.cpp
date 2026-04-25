#include <stdio.h>
#define Maxsize 100

typedef struct {
    int data[Maxsize];  // 存储空间
    int top;  // 栈顶指针
} SqStack;

void InitStack(SqStack &S) {
    S.top = -1;  // 初始化栈顶指针
}

void DestroyStack(SqStack &S) {
    S.top = -1;  // 销毁栈，重置栈顶指针
}

bool EmptyStack(SqStack S) {
    return S.top == -1;  // 判断栈是否为空
}

bool Push(SqStack &S, int x) {
    if (S.top == Maxsize - 1) {  // 判断栈是否已满
        return false;  // 栈满，无法入栈
    }
    S.data[++S.top] = x;  // 将元素入栈
    return true;
}

bool Pop(SqStack &S, int &x) {
    if (EmptyStack(S)) {  // 判断栈是否为空
        return false;  // 栈空，无法出栈
    }
    x = S.data[S.top--];  // 将栈顶元素出栈
    return true;
}

bool GetTop(SqStack S, int &x) {
    if (EmptyStack(S)) {  // 判断栈是否为空
        return false;  // 栈空，无法获取栈顶元素
    }
    x = S.data[S.top];  // 获取栈顶元素
    return true;
}

int main() {
    SqStack S;
    InitStack(S);
    return 0;
}