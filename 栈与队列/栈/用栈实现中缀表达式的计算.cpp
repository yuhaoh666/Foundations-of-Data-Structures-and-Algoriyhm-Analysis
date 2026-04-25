#include <stdio.h>
#define Maxsize 300

typedef struct {
    char data[Maxsize];  // 存储空间
    int top;  // 栈顶指针
} Stack;

void InitStack(Stack &S) {
    S.top = -1;  // 初始化栈顶指针
}

bool EmptyStack(Stack S) {
    return S.top == -1;  // 判断栈是否为空
}

bool FullStack(Stack S) {
    return S.top == Maxsize - 1;  // 判断栈是否已满
}

bool Push(Stack &S, char e) {
    if (FullStack(S)) {
        return false;  // 栈已满，无法入栈
    }
    S.data[++S.top] = e;  // 将元素入栈
    return true;
}

bool Pop(Stack &S, char &e) {
    if (EmptyStack(S)) {
        return false;  // 栈为空，无法出栈
    }
    e = S.data[S.top--];  // 将栈顶元素出栈
    return true;
}

// 判断运算符优先级
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;  // 加减运算符优先级为1
        case '*':
        case '/':
            return 2;  // 乘除运算符优先级为2
        default:
            return 0;  // 非运算符优先级为0
    }
}

// 用栈计算中缀表达式
int evaluateInfix(const char *expression) {
    Stack opStack;  // 运算符栈
    Stack valStack;  // 操作数栈
    InitStack(opStack);
    InitStack(valStack);        
    // 中缀表达式转换为后缀表达式并计算
    for (int i = 0; expression[i] != '\0'; i++) {
        char token = expression[i];
        if (token >= '0' && token <= '9') {  // 如果是数字，直接入操作数栈
            Push(valStack, token - '0');
        } else if (token == '(') {  // 左括号入运算符栈
            Push(opStack, token);
        } else if (token == ')') {  // 右括号，弹出运算符直到遇到左括号
            char op;
            while (!EmptyStack(opStack) && (op = opStack.data[opStack.top]) != '(') {
                Pop(opStack, op);
                char val2, val1;
                Pop(valStack, val2);
                Pop(valStack, val1);
                int result = 0;
                switch (op) {
                    case '+': result = val1 + val2; break;
                    case '-': result = val1 - val2; break;
                    case '*': result = val1 * val2; break;
                    case '/': result = val1 / val2; break;
                }
                Push(valStack, result);
            }
            Pop(opStack, op);  // 弹出左括号
        } else {  // 运算符
            while (!EmptyStack(opStack) && precedence(opStack.data[opStack.top]) >= precedence(token)) {
                char op;
                Pop(opStack, op);
                char val2, val1;
                Pop(valStack, val2);
                Pop(valStack, val1);
                int result = 0;
                switch (op) {   
                    case '+': result = val1 + val2; break;
                    case '-': result = val1 - val2; break;
                    case '*': result = val1 * val2; break;
                    case '/': result = val1 / val2; break;
                }
                Push(valStack, result);
            }
            Push(opStack, token);  // 当前运算符入栈
        }
    }
    // 处理剩余的运算符
    while (!EmptyStack(opStack)) {
        char op;
        Pop(opStack, op);
        char val2, val1;
        Pop(valStack, val2);
        Pop(valStack, val1);
        int result = 0;
        switch (op) {
            case '+': result = val1 + val2; break;
            case '-': result = val1 - val2; break;
            case '*': result = val1 * val2; break;
            case '/': result = val1 / val2; break;
        }
        Push(valStack, result);
    }
    char finalResult;
    Pop(valStack, finalResult);  // 最终结果
    return finalResult;
}

int main() {
    
    char expression[Maxsize];
    printf("Enter an infix expression: ");
    int result = evaluateInfix(expression);
    printf("The result of the expression %s is: %d\n", expression, result);
    return 0;
}

