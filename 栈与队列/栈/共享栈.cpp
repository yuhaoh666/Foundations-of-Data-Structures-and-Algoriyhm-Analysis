#include <stdio.h>
#define Maxsize 100

typedef struct {
    int data[Maxsize];  // 存储空间
    int top0;  
    int top1;  // 栈顶指针
} ShStack;

void InitStack(ShStack &S) {
    S.top0 = -1;  // 初始化栈顶指针
    S.top1 = Maxsize;  // 初始化栈顶指针
}

bool IsFull(ShStack S) {
    return S.top0 + 1 == S.top1;  // 判断栈是否已满
}