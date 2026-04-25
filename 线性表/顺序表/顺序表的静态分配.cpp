#include <stdio.h>
#define MaxSize 10  // 顺序表的最大长度

typedef struct {  // 顺序表的定义
    int data[MaxSize];
    int length;
} SeqList;

void InitList (SeqList &L)  // 初始化顺序表
{
    for (int i = 0; i < MaxSize; i++) {
        L.data[i] = 0;  //这一步不是必须的，因为我们会通过length来控制顺序表的有效元素个数，但为了清晰起见，我们将所有元素初始化为0。
    }
    L.length = 0;
}

bool Empty (SeqList L)  // 判断顺序表是否为空
{
    return L.length == 0;
}

bool DestroyList (SeqList &L)  // 销毁顺序表
{
    for (int i = 0; i < MaxSize; i++) {
        L.data[i] = 0;
    }
    L.length = 0;
    return true;
}

bool ListInsert (SeqList &L, int i, int e)  // 在顺序表的第i个位置插入元素e
{
    if (i < 1 || i > L.length + 1) {  // 增强健壮性，检查插入位置是否合法
        printf("插入位置不合法！\n");
        return false;
    }
    if (L.length >= MaxSize) {  // 增强健壮性，检查顺序表是否已满
        printf("顺序表已满，无法插入！\n");
        return false;
    }
    for (int j = L.length; j >= i; j--) {  // 将第i个位置及之后的元素向后移动
        L.data[j] = L.data[j - 1];
    }
    L.data[i - 1] = e;  // 插入元素e
    L.length++;  // 更新长度
    return true;
}

bool ListDelete (SeqList &L, int i, int &e)  // 删除顺序表的第i个位置的元素
{
    if (i < 1 || i > L.length) {  // 增强健壮性，检查删除位置是否合法
        printf("删除位置不合法！\n");
        return false;
    }
    e = L.data[i - 1];  // 获取被删除元素的值
    for (int j = i; j < L.length; j++) {  // 将第i个位置之后的元素向前移动
        L.data[j - 1] = L.data[j];
    }
    L.length--;  // 更新长度
    return true;
}

int main () 
{
    SeqList L;
    InitList (L);
    int e;
    
    ListInsert(L, 1, 10);  // 在第1个位置插入10
    ListInsert(L, 2, 20);  // 在第2个位置插入20
    ListInsert(L, 3, 30);  // 在第3个位置插入30
    printf("顺序表的长度: %d\n", L.length);

    if (ListDelete(L, 2, e))  // 删除第2个位置的元素
        printf("被删除的元素: %d\n", e);
    else
        printf("删除失败！\n");

    printf("顺序表的长度: %d\n", L.length);
    return 0;
}