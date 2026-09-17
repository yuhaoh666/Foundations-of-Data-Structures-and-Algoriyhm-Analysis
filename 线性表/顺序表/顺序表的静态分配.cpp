// 顺序表的静态分配：用定长数组存放元素，容量固定、不可扩展
// 插入/删除的平均时间复杂度为 O(n)，按位查找为 O(1)
#include <stdio.h>
#define MaxSize 10  // 顺序表的最大长度（容量）

typedef struct {        // 顺序表的定义
    int data[MaxSize];  // 静态分配的存储空间
    int length;         // 当前长度（有效元素个数）
} SeqList;

// 初始化顺序表：元素清零、长度置 0
void InitList(SeqList &L) {
    for (int i = 0; i < MaxSize; i++) {
        L.data[i] = 0;  // 这一步不是必须的（用 length 就能控制有效元素），此处为便于调试而清零
    }
    L.length = 0;
}

// 判空：length 为 0 说明表空
bool Empty(SeqList L) {
    return L.length == 0;
}

// 清空/销毁顺序表：静态分配的空间由系统回收，只需逻辑清空
void DestroyList(SeqList &L) {
    InitList(L);
}

// 打印顺序表中的所有元素
void PrintList(SeqList L) {
    printf("顺序表(长度=%d)：", L.length);
    for (int i = 0; i < L.length; i++) {
        printf("%d ", L.data[i]);
    }
    printf("\n");
}

// 按位查找：用 e 返回第 i 个元素（位序 i 从 1 开始）
bool GetElem(SeqList L, int i, int &e) {
    if (i < 1 || i > L.length) {  // 增强健壮性，检查位置是否合法
        return false;
    }
    e = L.data[i - 1];
    return true;
}

// 按值查找：返回第一个值为 e 的元素的位序，未找到返回 0
int LocateElem(SeqList L, int e) {
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] == e) {  // 结构类型的比较需要重载 == 运算符
            return i + 1;      // 注意位序从 1 开始
        }
    }
    return 0;
}

// 插入：在第 i 个位置（位序）插入元素 e，时间复杂度 O(n)
bool ListInsert(SeqList &L, int i, int e) {
    if (i < 1 || i > L.length + 1) {  // 位序合法范围是 1 ~ length+1（可插在表尾之后）
        printf("插入位置 %d 不合法！\n", i);
        return false;
    }
    if (L.length >= MaxSize) {  // 静态分配的表容量固定，表满后无法插入
        printf("顺序表已满，无法插入！\n");
        return false;
    }
    for (int j = L.length; j >= i; j--) {  // 第 i 个位置及之后的元素统一后移一位
        L.data[j] = L.data[j - 1];
    }
    L.data[i - 1] = e;  // 插入元素 e
    L.length++;
    return true;
}

// 删除：删除第 i 个位置的元素，用 e 返回其值，时间复杂度 O(n)
bool ListDelete(SeqList &L, int i, int &e) {
    if (i < 1 || i > L.length) {  // 位序合法范围是 1 ~ length
        printf("删除位置 %d 不合法！\n", i);
        return false;
    }
    e = L.data[i - 1];                    // 先保存被删元素的值
    for (int j = i; j < L.length; j++) {  // 第 i 个位置之后的元素统一前移一位
        L.data[j - 1] = L.data[j];
    }
    L.length--;
    return true;
}

int main() {
    SeqList L;
    InitList(L);
    int e;

    for (int i = 1; i <= 5; i++) {  // 依次插入 10 20 30 40 50
        ListInsert(L, i, i * 10);
    }
    PrintList(L);

    if (ListDelete(L, 2, e)) {  // 删除第 2 个位置的元素
        printf("被删除的元素：%d\n", e);
    } else {
        printf("删除失败！\n");
    }
    PrintList(L);

    if (GetElem(L, 3, e)) {  // 按位查找
        printf("第 3 个元素：%d\n", e);
    }
    printf("值 50 的位序：%d\n", LocateElem(L, 50));  // 按值查找
    printf("表是否为空：%s\n", Empty(L) ? "是" : "否");

    DestroyList(L);
    PrintList(L);
    return 0;
}
