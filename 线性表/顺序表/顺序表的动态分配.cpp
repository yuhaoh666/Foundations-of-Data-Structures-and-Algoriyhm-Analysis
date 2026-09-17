// 顺序表的动态分配：用 malloc 在堆上申请空间，容量不够时可以扩容
// 插入/删除平均时间复杂度 O(n)，按位查找 O(1)
#include <stdio.h>
#include <stdlib.h>
#define InitSize 10  // 顺序表的初始容量

typedef struct {  // 顺序表的定义
    int *data;    // 指示动态分配数组的指针（存储空间基址）
    int length;   // 当前长度（有效元素个数）
    int size;     // 当前分配的存储容量
} SeqList;

// 初始化顺序表：按初始容量申请空间
void InitList(SeqList &L) {
    L.data = (int *)malloc(InitSize * sizeof(int));  // 分配存储空间
    if (L.data == NULL) {
        printf("内存分配失败！\n");
        exit(-1);
    }
    L.length = 0;       // 初始长度为 0
    L.size = InitSize;  // 初始容量为 InitSize
}

// 扩容：容量增加 increment，时间复杂度 O(n)（需要搬运 length 个元素）
void IncreaseSize(SeqList &L, int increment) {
    int *newData = (int *)malloc((L.size + increment) * sizeof(int));  // 申请更大的新空间
    if (newData == NULL) {
        printf("内存分配失败！\n");
        exit(-1);
    }
    for (int i = 0; i < L.length; i++) {  // 把原有元素按原顺序复制到新空间
        newData[i] = L.data[i];
    }
    free(L.data);         // 释放旧空间，避免内存泄漏
    L.data = newData;     // 更新基址
    L.size += increment;  // 更新容量
}

// 销毁顺序表：归还动态申请的空间，并置空指针防止野指针
void DestroyList(SeqList &L) {
    free(L.data);
    L.data = NULL;
    L.length = 0;
    L.size = 0;
}

// 按位查找：返回第 i 个元素的值，失败返回 -1
int GetElem(SeqList L, int i) {
    if (i < 1 || i > L.length) {  // 增强健壮性，检查位置是否合法
        printf("位置 %d 不合法！\n", i);
        return -1;  // 返回 -1 表示失败
    }
    return L.data[i - 1];
}

// 按值查找：返回第一个值为 e 的元素的位序，未找到返回 0
int LocateElem(SeqList L, int e) {
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] == e) {  // 结构类型的比较需要重载 == 运算符
            return i + 1;
        }
    }
    return 0;
}

// 插入：在第 i 个位置插入元素 e；容量不足时先自动扩容
bool ListInsert(SeqList &L, int i, int e) {
    if (i < 1 || i > L.length + 1) {
        printf("插入位置 %d 不合法！\n", i);
        return false;
    }
    if (L.length >= L.size) {  // 空间已满，先扩容再插入
        IncreaseSize(L, InitSize);
    }
    for (int j = L.length; j >= i; j--) {  // 元素后移，腾出插入位置
        L.data[j] = L.data[j - 1];
    }
    L.data[i - 1] = e;
    L.length++;
    return true;
}

// 删除：删除第 i 个位置的元素，用 e 返回其值
bool ListDelete(SeqList &L, int i, int &e) {
    if (i < 1 || i > L.length) {
        printf("删除位置 %d 不合法！\n", i);
        return false;
    }
    e = L.data[i - 1];                    // 保存被删元素
    for (int j = i; j < L.length; j++) {  // 元素前移，填补删除位置
        L.data[j - 1] = L.data[j];
    }
    L.length--;
    return true;
}

// 打印顺序表
void PrintList(SeqList L) {
    printf("顺序表(长度=%d, 容量=%d)：", L.length, L.size);
    for (int i = 0; i < L.length; i++) {
        printf("%d ", L.data[i]);
    }
    printf("\n");
}

int main() {
    SeqList L;
    InitList(L);

    for (int i = 1; i <= 15; i++) {  // 插入 15 个元素，会触发一次自动扩容
        ListInsert(L, i, i);
    }
    PrintList(L);

    int e;
    if (ListDelete(L, 1, e)) {  // 删除表头元素
        printf("被删除的元素：%d\n", e);
    }
    PrintList(L);
    printf("元素 15 的位序：%d，第 3 个元素：%d\n", LocateElem(L, 15), GetElem(L, 3));

    DestroyList(L);  // 释放顺序表的存储空间
    return 0;
}
