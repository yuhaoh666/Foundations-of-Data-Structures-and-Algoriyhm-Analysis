// 栈的应用：括号匹配检查
// 扫描表达式：遇到左括号就入栈；遇到右括号则与栈顶的左括号比较，
// 匹配则出栈；扫描结束后栈空且中途没有不匹配，说明括号匹配
#include <stdio.h>
#define Maxsize 100

typedef struct {
    char data[Maxsize];  // 存放左括号
    int top;             // 栈顶指针
} SqStack;

// 初始化栈
void InitStack(SqStack &S) {
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

// 入栈
bool Push(SqStack &S, char c) {
    if (FullStack(S)) {
        return false;
    }
    S.data[++S.top] = c;
    return true;
}

// 出栈
bool Pop(SqStack &S, char &c) {
    if (EmptyStack(S)) {
        return false;
    }
    c = S.data[S.top--];
    return true;
}

// 判断左右括号是否配对
bool Match(char left, char right) {
    return (left == '(' && right == ')') ||
           (left == '[' && right == ']') ||
           (left == '{' && right == '}');
}

// 括号匹配检查：匹配返回 true；不匹配时用 msg 说明原因
bool BracketCheck(const char *str, const char *&msg) {
    SqStack S;
    InitStack(S);
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c == '(' || c == '[' || c == '{') {
            if (!Push(S, c)) {          // 栈满（括号嵌套层数过多）
                msg = "栈满，括号嵌套过深";
                return false;
            }
        } else if (c == ')' || c == ']' || c == '}') {
            if (EmptyStack(S)) {        // 右括号多余
                msg = "右括号多余，出现不匹配";
                return false;
            }
            char left;
            Pop(S, left);
            if (!Match(left, c)) {      // 括号类型不匹配
                msg = "左右括号类型不匹配";
                return false;
            }
        }
        // 其他字符直接跳过
    }
    if (!EmptyStack(S)) {               // 左括号多余
        msg = "左括号多余，出现不匹配";
        return false;
    }
    msg = "匹配成功";
    return true;
}

// 测试用例封装
void TestCheck(const char *str) {
    const char *msg = "";
    bool ok = BracketCheck(str, msg);
    printf("%-24s -> %s（%s）\n", str, ok ? "匹配" : "不匹配", msg);
}

int main() {
    TestCheck("((()))");
    TestCheck("{[()]}");
    TestCheck("((())))[");
    TestCheck("([)]");
    TestCheck("(((");
    TestCheck("))) ");
    TestCheck("a*(b+c)-[d/e]");
    return 0;
}
