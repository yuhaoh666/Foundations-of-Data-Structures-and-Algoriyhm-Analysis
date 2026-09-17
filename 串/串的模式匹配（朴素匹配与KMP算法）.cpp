// 串（定长顺序存储）的基本操作与模式匹配：朴素（暴力）匹配与 KMP 匹配
// nextval 采用教材约定：nextval[0] = -1，nextval[j] 表示失配时模式串指针应回退到的位置
#include <stdio.h>
#include <string.h>
#define MAXSIZE 255

typedef struct {
    char ch[MAXSIZE];  // 每个分量存放一个字符
    int length;        // 串的实际长度
} SqString;

// 赋值：把字符串常量 chars 的内容赋给串 T
void StrAssign(SqString &T, const char *chars) {
    int len = (int)strlen(chars);
    if (len >= MAXSIZE) {  // 超出定长数组的容量时截断
        len = MAXSIZE - 1;
    }
    for (int i = 0; i < len; i++) {
        T.ch[i] = chars[i];
    }
    T.length = len;
}

// 判空
bool StrEmpty(SqString S) {
    return S.length == 0;
}

// 清空串（逻辑清空，无需释放空间）
void ClearString(SqString &S) {
    S.length = 0;
}

// 求子串：Sub 返回 S 中从第 pos 个字符起、长度为 len 的子串（位序从 1 开始）
// 注意 Sub 必须传引用，否则结果无法带回调用者
bool SubString(SqString &Sub, SqString S, int pos, int len) {
    if (pos < 1 || pos > S.length || len < 0 || pos + len - 1 > S.length) {
        Sub.length = 0;
        return false;
    }
    for (int i = 0; i < len; i++) {
        Sub.ch[i] = S.ch[pos + i - 1];
    }
    Sub.length = len;
    return true;
}

// 串比较：S > T 返回正数，S < T 返回负数，相等返回 0
int StrCompare(SqString S, SqString T) {
    for (int i = 0; i < S.length && i < T.length; i++) {
        if (S.ch[i] != T.ch[i]) {
            return S.ch[i] - T.ch[i];
        }
    }
    return S.length - T.length;  // 前缀全部相同则较长的串更大
}

// 朴素模式匹配：返回模式串 T 在主串 S 中首次出现的位置（位序从 1 开始），未找到返回 0
// 时间复杂度 O(n*m)（n、m 分别是主串与模式串的长度）
int Index(SqString S, SqString T) {
    int i = 0, j = 0;  // i、j 分别记录主串与模式串当前正在比较的位置
    while (i < S.length && j < T.length) {
        if (S.ch[i] == T.ch[j]) {  // 当前字符相同，比较下一对
            i++;
            j++;
        } else {  // 失配：主串指针回退到本趟匹配起始位置的下一位，模式串指针归零
            i = i - j + 1;
            j = 0;
        }
    }
    return (j == T.length) ? i - T.length + 1 : 0;  // 模式串走完说明匹配成功
}

// 求 next 数组：next[j] = T 的前 j 个字符组成的子串中“最长相等前后缀长度 - 1”
void GetNext(SqString T, int next[]) {
    int i = 0, j = -1;
    next[0] = -1;  // 约定：模式串首字符失配时，主串指针后移一位
    while (i < T.length - 1) {
        if (j == -1 || T.ch[i] == T.ch[j]) {  // 前后缀继续匹配
            i++;
            j++;
            next[i] = j;
        } else {  // 否则回退到更短的前缀继续比较
            j = next[j];
        }
    }
}

// 求 nextval 数组：next 的优化版——若 T[i] == T[j]，回退后仍然失配，可以再往前退
void GetNextVal(SqString T, int nextval[]) {
    int i = 0, j = -1;
    nextval[0] = -1;
    while (i < T.length - 1) {
        if (j == -1 || T.ch[i] == T.ch[j]) {
            i++;
            j++;
            if (T.ch[i] != T.ch[j]) {
                nextval[i] = j;
            } else {  // 回退位置上的字符与当前字符相同，直接继承它的 nextval
                nextval[i] = nextval[j];
            }
        } else {
            j = nextval[j];
        }
    }
}

// KMP 算法：主串指针 i 永不回退，时间复杂度 O(n+m)
int KMPIndex(SqString S, SqString T, int nextval[]) {
    int i = 0, j = 0;
    while (i < S.length && j < T.length) {
        if (j == -1 || S.ch[i] == T.ch[j]) {  // j == -1 表示模式串第 0 个字符就失配
            i++;
            j++;
        } else {
            j = nextval[j];  // 只移动模式串指针
        }
    }
    return (j == T.length) ? i - T.length + 1 : 0;
}

// 打印串
void PrintString(SqString S) {
    for (int i = 0; i < S.length; i++) {
        putchar(S.ch[i]);
    }
}

// 打印某个模式串的 nextval 数组
void PrintNextVal(SqString T) {
    int nextval[MAXSIZE];
    GetNextVal(T, nextval);
    printf("模式串 ");
    PrintString(T);
    printf(" 的 nextval 数组：");
    for (int i = 0; i < T.length; i++) {
        printf("%d ", nextval[i]);
    }
    printf("\n");
}

// 对比一组主串与模式串下两种算法的匹配结果
void TestMatch(const char *s, const char *t) {
    SqString S, T;
    StrAssign(S, s);
    StrAssign(T, t);
    int nextval[MAXSIZE];
    GetNextVal(T, nextval);
    printf("主串 %-14s 模式串 %-6s 朴素匹配位置=%d，KMP 匹配位置=%d\n",
           s, t, Index(S, T), KMPIndex(S, T, nextval));
}

int main() {
    // 1. 基本操作演示
    SqString s, t, sub;
    StrAssign(s, "Hello, World!");
    StrAssign(t, "World");
    printf("主串：");
    PrintString(s);
    printf("（长度 %d）\n", s.length);
    printf("模式串：");
    PrintString(t);
    printf("（长度 %d）\n", t.length);

    if (SubString(sub, s, 8, 5)) {  // 从第 8 个字符起取 5 个字符
        printf("子串（第 8 个字符起，长度 5）：");
        PrintString(sub);
        printf("\n");
    }
    printf("串比较结果（相等为 0）：%d\n", StrCompare(t, sub));

    // 2. 模式匹配演示
    PrintNextVal(t);
    int nextval[MAXSIZE];
    GetNextVal(t, nextval);
    printf("主串 ");
    PrintString(s);
    printf(" 中查找模式串：朴素=%d，KMP=%d\n", Index(s, t), KMPIndex(s, t, nextval));
    TestMatch("ababcabcacbab", "abcac");
    TestMatch("aaaaab", "aaab");
    TestMatch("abcdef", "xyz");
    return 0;
}
