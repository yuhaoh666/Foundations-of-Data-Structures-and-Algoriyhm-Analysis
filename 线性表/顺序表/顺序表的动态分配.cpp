#include <stdio.h>
#include <stdlib.h>
#define InitSize 10

typedef struct {  // 顺序表的定义
    int *data;  // 存储空间基址
    int length;  // 当前长度
    int size;    // 当前分配的存储容量
} SeqList;

void InitList (SeqList &L)  // 初始化顺序表
{
    L.data = (int *)malloc(InitSize * sizeof(int));  // 分配存储空间
    if (!L.data) {
        printf("内存分配失败！\n");
        exit(-1);
    }
    L.length = 0;  // 初始长度为0
    L.size = InitSize;  // 初始容量为InitSize
}

void IncreaseSize (SeqList &L, int increment)  // 增加顺序表的存储容量
{
    int *newData = (int *)malloc((L.size + increment) * sizeof(int));  // 分配新的存储空间
    if (!newData) {
        printf("内存分配失败！\n");
        exit(-1);
    }
    for (int i = 0; i < L.length; i++) {  // 将原有元素复制到新空间
        newData[i] = L.data[i];
    }
    free(L.data);  // 释放原有存储空间
    L.data = newData;  // 更新基址
    L.size += increment;  // 更新容量
}

int GetElem (SeqList L, int i)  // 获取顺序表的第i个元素
{
    if (i < 1 || i > L.length) {  // 增强健壮性，检查位置是否合法
        printf("位置不合法！\n");
        return -1;  // 返回-1表示失败
    }
    return L.data[i - 1];  // 返回元素值
}

int LocateElem (SeqList L, int e)  // 查找顺序表中第一个值为e的元素
{
    for (int j = 0; j < L.length; j++) {
        if (L.data[j] == e) {  // 结构类型的比较需要重载==运算符
            return j + 1;  // 返回元素位序，注意位置从1开始
        }
    }
    return 0;  // 返回0表示未找到
}
int main () 
{
    SeqList L;
    InitList (L);
    
    // 增加顺序表的存储容量
    IncreaseSize(L, 5);
    
    free(L.data);  // 释放顺序表的存储空间
    return 0;
}

