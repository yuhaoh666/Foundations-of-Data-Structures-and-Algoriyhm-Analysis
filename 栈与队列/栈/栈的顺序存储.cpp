// 顺序栈：用数组存放栈元素，top 指向栈顶元素的下标（-1 表示空栈）
// 入栈、出栈、取栈顶的时间复杂度都是 O(1)
#include <stdio.h>
#define Maxsize 100

typedef struct {
    int data[Maxsize];  // 静态数组存放栈元素
    int top;            // 栈顶指针（栈顶元素的下标）
} SqStack;

// 初始化栈：top = -1 表示空栈
void InitStack(SqStack &S) {
    S.top = -1;
}

// 销毁栈：静态分配的数组由系统回收，逻辑上置空即可
void DestroyStack(SqStack &S) {
    S.top = -1;
}

// 判空
bool EmptyStack(SqStack S) {
    return S.top == -1;
}

// 判满
bool FullStack(SqStack S) {
    return S.top == Maxsize - 1;
}

// 求栈中元素个数
int StackLength(SqStack S) {
    return S.top + 1;
}

// 入栈：先移动栈顶指针再存数据
bool Push(SqStack &S, int x) {
    if (FullStack(S)) {  // 栈满，上溢
        return false;
    }
    S.data[++S.top] = x;
    return true;
}

// 出栈：先用 x 带回栈顶元素，再移动栈顶指针
bool Pop(SqStack &S, int &x) {
    if (EmptyStack(S)) {  // 栈空，下溢
        return false;
    }
    x = S.data[S.top--];
    return true;
}

// 读取栈顶元素（不出栈）
bool GetTop(SqStack S, int &x) {
    if (EmptyStack(S)) {
        return false;
    }
    x = S.data[S.top];
    return true;
}

// 打印栈（从栈底到栈顶）
void PrintStack(SqStack S) {
    printf("栈(长度=%d, 从栈底到栈顶)：", StackLength(S));
    for (int i = 0; i <= S.top; i++) {
        printf("%d ", S.data[i]);
    }
    printf("\n");
}

int main() {
    SqStack S;
    int x;
    InitStack(S);
    printf("初始化后是否为空：%s\n", EmptyStack(S) ? "是" : "否");

    for (int i = 1; i <= 5; i++) {  // 依次入栈 1 2 3 4 5
        Push(S, i);
    }
    PrintStack(S);

    if (GetTop(S, x)) {  // 读取栈顶但不删除
        printf("当前栈顶元素：%d\n", x);
    }
    if (Pop(S, x)) {     // 出栈
        printf("出栈元素：%d\n", x);
    }
    PrintStack(S);

    while (!EmptyStack(S)) {  // 全部出栈
        Pop(S, x);
    }
    printf("全部出栈后是否为空：%s\n", EmptyStack(S) ? "是" : "否");
    printf("空栈出栈返回：%s\n", Pop(S, x) ? "成功" : "失败");

    DestroyStack(S);
    return 0;
}
