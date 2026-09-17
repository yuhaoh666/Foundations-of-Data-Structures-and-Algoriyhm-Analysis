// 栈的应用：用栈计算中缀表达式（同时完成“中缀转后缀”与“后缀求值”）
// 需要两个栈：运算符栈 opStack 存放运算符和括号，操作数栈 valStack 存放整数
// 本程序支持多位整数与圆括号，例如 (12+3)*4-5/2 应得 58（5/2 为整除，结果为 2）
#include <stdio.h>
#define Maxsize 300

// ==================== 操作数栈（存整数） ====================
typedef struct {
    int data[Maxsize];
    int top;  // 栈顶指针，-1 表示空栈
} IntStack;

void InitIntStack(IntStack &S) {
    S.top = -1;
}

bool EmptyIntStack(IntStack S) {
    return S.top == -1;
}

bool PushInt(IntStack &S, int e) {
    if (S.top == Maxsize - 1) {  // 栈满
        return false;
    }
    S.data[++S.top] = e;
    return true;
}

bool PopInt(IntStack &S, int &e) {
    if (EmptyIntStack(S)) {  // 栈空
        return false;
    }
    e = S.data[S.top--];
    return true;
}

// ==================== 运算符栈（存字符） ====================
typedef struct {
    char data[Maxsize];
    int top;
} CharStack;

void InitCharStack(CharStack &S) {
    S.top = -1;
}

bool EmptyCharStack(CharStack S) {
    return S.top == -1;
}

bool PushChar(CharStack &S, char e) {
    if (S.top == Maxsize - 1) {
        return false;
    }
    S.data[++S.top] = e;
    return true;
}

bool PopChar(CharStack &S, char &e) {
    if (EmptyCharStack(S)) {
        return false;
    }
    e = S.data[S.top--];
    return true;
}

// 读取栈顶运算符（调用前须保证栈非空）
char GetTopChar(CharStack S) {
    return S.data[S.top];
}

// ==================== 表达式的计算 ====================
// 运算符优先级：'+'、'-' 为 1，'*'、'/' 为 2，括号等其他字符为 0
int Precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

// 弹出两个操作数完成一次运算，并把结果压回操作数栈
// 注意：先弹出的栈顶是右操作数 b，后弹出的次栈顶是左操作数 a
bool ApplyOp(IntStack &valStack, char op) {
    int b, a;
    if (!PopInt(valStack, b) || !PopInt(valStack, a)) {
        printf("表达式错误：操作数不足！\n");
        return false;
    }
    int result = 0;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if (b == 0) {  // 处理除零
                printf("表达式错误：除数为 0！\n");
                return false;
            }
            result = a / b;
            break;
        default:
            printf("表达式错误：非法运算符 '%c'！\n", op);
            return false;
    }
    return PushInt(valStack, result);
}

// 计算中缀表达式；ok 返回计算是否成功（失败原因由函数内部打印）
int EvaluateInfix(const char *expression, bool &ok) {
    IntStack valStack;  // 操作数栈
    CharStack opStack;  // 运算符栈
    InitIntStack(valStack);
    InitCharStack(opStack);
    ok = false;

    for (int i = 0; expression[i] != '\0'; i++) {
        char token = expression[i];

        if (token == ' ') {  // 跳过空白字符
            continue;
        } else if (token >= '0' && token <= '9') {
            int num = 0;  // 连续读入数字字符，拼出多位整数
            while (expression[i] >= '0' && expression[i] <= '9') {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            i--;  // 回退一位，外层 for 循环还会自增
            PushInt(valStack, num);
        } else if (token == '(') {  // 左括号无条件入栈
            PushChar(opStack, token);
        } else if (token == ')') {  // 右括号：弹出运算符直到遇到左括号
            char op;
            while (!EmptyCharStack(opStack) && GetTopChar(opStack) != '(') {
                PopChar(opStack, op);
                if (!ApplyOp(valStack, op)) {
                    return 0;
                }
            }
            if (EmptyCharStack(opStack)) {  // 没有与之配对的左括号
                printf("表达式错误：括号不匹配！\n");
                return 0;
            }
            PopChar(opStack, op);  // 弹出左括号
        } else if (token == '+' || token == '-' || token == '*' || token == '/') {
            // 栈顶运算符优先级不低于当前运算符时先出栈计算（保证同级的左结合）
            while (!EmptyCharStack(opStack) && GetTopChar(opStack) != '(' &&
                   Precedence(GetTopChar(opStack)) >= Precedence(token)) {
                char op;
                PopChar(opStack, op);
                if (!ApplyOp(valStack, op)) {
                    return 0;
                }
            }
            PushChar(opStack, token);  // 当前运算符入栈
        } else {
            printf("表达式错误：出现非法字符 '%c'！\n", token);
            return 0;
        }
    }

    // 扫描结束后，弹出栈中剩余的所有运算符
    while (!EmptyCharStack(opStack)) {
        char op;
        PopChar(opStack, op);
        if (op == '(') {
            printf("表达式错误：括号不匹配！\n");
            return 0;
        }
        if (!ApplyOp(valStack, op)) {
            return 0;
        }
    }

    int result;
    if (!PopInt(valStack, result)) {  // 没有可计算的操作数
        printf("表达式错误：没有可计算的操作数！\n");
        return 0;
    }
    if (!EmptyIntStack(valStack)) {   // 操作数没有被全部用完
        printf("表达式错误：操作数多余！\n");
        return 0;
    }
    ok = true;
    return result;
}

int main() {
    // 1. 自动测试若干表达式
    const char *tests[] = {"(12+3)*4-5/2", "3*(7-2)", "100/5+6*2", "(1+2)*(3+4)", "1+2*3-4/2"};
    printf("=== 自动测试 ===\n");
    for (int i = 0; i < 5; i++) {
        bool ok;
        int result = EvaluateInfix(tests[i], ok);
        if (ok) {
            printf("%-14s = %d\n", tests[i], result);
        }
    }

    // 2. 交互输入（若无输入则自动跳过，便于非交互方式运行）
    char expression[Maxsize];
    printf("\n请输入中缀表达式（只含整数、+ - * / 和圆括号）：");
    if (scanf("%299s", expression) == 1) {
        bool ok;
        int result = EvaluateInfix(expression, ok);
        if (ok) {
            printf("%s = %d\n", expression, result);
        }
    } else {
        printf("(无输入，跳过交互演示)\n");
    }
    return 0;
}
