#include <stdio.h>
#include <string.h>
#define MAXSIZE 255

typedef struct {
    char ch[MAXSIZE];
    int length;
} SqString;

void StrAssign (SqString &T, char *chars) {
    int i;
    if (strlen(chars) > MAXSIZE) {
        T.length = 0;
        return;
    }
    for (i = 0; chars[i]; i++) {
        T.ch[i] = chars[i];
    }
    T.length = i;
}

bool StrEmpty (SqString S) {
    return S.length == 0;
}

void ClearString (SqString &S) {
    S.length = 0;
}

bool SubString (SqString Sub, SqString S, int pos, int len) {
    if (pos < 1 || pos > S.length || len < 0 || pos + len - 1 > S.length) 
        return false;
    for (int i = 0; i < len; i++) {
        Sub.ch[i] = S.ch[pos + i - 1];
    }
    Sub.length = len;
    return true;
}

int StrCompare (SqString S, SqString T) {
    int i;
    for (i = 0; i < S.length && i < T.length; i++) {
        if (S.ch[i] != T.ch[i]) {
            return S.ch[i] - T.ch[i];
        }
    }
    return S.length - T.length;
}

int Index (SqString S, SqString T) {  // 朴素模式匹配算法
    int i, j;
    for (i = 0; i <= S.length - T.length; i++) {
        for (j = 0; j < T.length; j++) {
            if (S.ch[i + j] != T.ch[j]) {
                break;
            }
        }
        if (j == T.length) {
            return i + 1; // 返回位置，1-based index
        }
    }
    return 0; // 未找到
}

void GetNext (SqString T, int next[]) { // 计算next数组
    int i = 0, j = -1;  
    next[0] = -1;  // 初始化next数组的第一个元素为-1，表示没有公共前后缀
    while (i < T.length - 1) {  // 当i小于T的长度减1时，继续计算next数组
        if (j == -1 || T.ch[i] == T.ch[j]) {  // 当j为-1或当前字符匹配时，i和j都向前移动
            i++;
            j++;
            next[i] = j;  // 将j的值赋给next[i]，表示最长公共前后缀的长度
        } else {
            j = next[j];  // 当字符不匹配时，j回退到next[j]的位置，继续比较
        }
    }
}

void GetNextVal (SqString T, int nextval[]) { // 计算优化的next数组
    int i = 0, j = -1;
    nextval[0] = -1;
    while (i < T.length - 1) {
        if (j == -1 || T.ch[i] == T.ch[j]) {
            i++;
            j++;
            if (T.ch[i] != T.ch[j]) {
                nextval[i] = j;
            } else {
                nextval[i] = nextval[j];
            }
        } else {
            j = nextval[j];
        }
    }
}

int KMPIndex (SqString S, SqString T, int nextval[]) {  // KMP算法
    int i = 0, j = 0;
    while (i < S.length && j < T.length) {
        if (j == -1 || S.ch[i] == T.ch[j]) {
            i++;
            j++;
        } else {
            j = nextval[j];
        }
    }
    if (j == T.length) {
        return i - j + 1; // 返回位置，1-based index
    }
    return 0; // 未找到
}

int main () {
    SqString s; 
    SqString t;
    StrAssign(s, "Hello, World!");
    StrAssign(t, "World");
    int nextval[MAXSIZE];
    GetNextVal(t, nextval);
    int pos = KMPIndex(s, t, nextval);
    printf("Pattern found at position: %d\n", pos);
    return 0;
}