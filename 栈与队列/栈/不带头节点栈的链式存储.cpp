#include <stdio.h>
#include <stdlib.h>

typedef struct StackNode {
    int data;  // 数据域
    struct StackNode *next;  // 指针域
} StackNode, *LiStack;

void InitStack(LiStack &S) {
    S = NULL;  // 初始化栈顶指针
}

void DestroyStack(LiStack &S) {
    while (S != NULL) {
        LiStack temp = S;  // 临时指针
        S = S->next;  // 移动栈顶指针
        free(temp);  // 释放节点内存
    }
}

bool EmptyStack(LiStack S) {
    return S == NULL;  // 判断栈是否为空
}

bool Push(LiStack &S, int x) {
    StackNode* newNode = (StackNode *)malloc(sizeof(struct StackNode));  // 创建新节点
    if (newNode == NULL) {
        return false;  // 内存分配失败
    }
    newNode->data = x;  // 设置数据域
    newNode->next = S;  // 将新节点指向当前栈顶
    S = newNode;  // 更新栈顶指针
    return true;
}

bool Pop(LiStack &S, int &x) {
    if (EmptyStack(S)) {
        return false;  // 栈空，无法出栈
    }
    StackNode* temp = S;  // 临时指针
    x = S->data;  // 获取栈顶元素
    S = S->next;  // 更新栈顶指针
    free(temp);  // 释放节点内存
    return true;
}

int main() {
    LiStack S;
    InitStack(S);
    return 0;
}
