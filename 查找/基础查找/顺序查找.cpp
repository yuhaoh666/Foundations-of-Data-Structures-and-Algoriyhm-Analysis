// 顺序查找（线性查找）：从表的一端开始逐个比较关键字，直到找到或查遍全表
//   时间复杂度 O(n)，平均查找长度 ASL(成功) = (n+1)/2，ASL(失败) = n+1
//   优点：对存储结构没有要求（顺序表、链表都可以），也不要求表有序
//   缺点：n 较大时效率低
// 两种写法：
//   1. 普通写法：循环中既要判断下标越界（i <= n），又要比较关键字
//   2. 哨兵写法：把待查关键字先放到下标 0 作为“哨兵”，循环一定会因命中哨兵而停下，
//      于是可以省掉每次的越界判断，当 n 很大时能明显减少判断次数
#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int data[MAXSIZE + 1];  // 下标 0 空出来放哨兵，有效元素从下标 1 开始
    int length;             // 元素个数 n
} SSTable;

// 初始化：建立一个长度为 n 的线性表，元素依次为 3, 5, 7, ...（与位序一一对应，便于观察）
void InitList(SSTable &L, int n) {
    for (int i = 1; i <= n; i++) {
        L.data[i] = i * 2 + 1;
    }
    L.length = n;
}

// 打印线性表
void PrintList(SSTable L) {
    printf("线性表(长度=%d)：", L.length);
    for (int i = 1; i <= L.length; i++) {
        printf("%d ", L.data[i]);
    }
    printf("\n");
}

// 普通顺序查找：返回找到元素的位序（从 1 开始），未找到返回 0；cmp 带回关键字比较次数
int SearchSeq(SSTable L, int key, int &cmp) {
    cmp = 0;
    for (int i = 1; i <= L.length; i++) {
        cmp++;  // 既要判断 i 是否越界，又要比较一次关键字
        if (L.data[i] == key) {
            return i;
        }
    }
    return 0;  // 查遍全表都没有找到
}

// 带哨兵的顺序查找（从后往前找）：data[0] 先存放待查关键字
// 循环必然在 data[i] == key 时结束（最坏是在下标 0 命中哨兵），因此不必写越界判断
int SearchSentry(SSTable L, int key, int &cmp) {
    L.data[0] = key;  // 设置哨兵（这里按值传递，不会破坏原表）
    cmp = 0;
    int i = L.length;
    while (true) {
        cmp++;  // 只需比较关键字，省掉了 i >= 1 的判断
        if (L.data[i] == key) {
            break;
        }
        i--;
    }
    return i;  // 返回 0 表示查找失败（命中的是哨兵）
}

// 统计并打印（实测的）平均查找长度，与理论值对照
void PrintASL(SSTable L) {
    int total = 0, totalSentry = 0;
    for (int i = 1; i <= L.length; i++) {  // 表中每个元素各查一次
        int cmp, cmp2;
        SearchSeq(L, L.data[i], cmp);
        SearchSentry(L, L.data[i], cmp2);
        total += cmp;
        totalSentry += cmp2;
    }
    printf("ASL(成功)：普通写法 = %d/%d ≈ %.2f；哨兵写法 = %d/%d ≈ %.2f；理论值 (n+1)/2 = %.2f\n",
           total, L.length, (double)total / L.length, totalSentry, L.length,
           (double)totalSentry / L.length, (L.length + 1) / 2.0);

    // 查找失败：普通写法必须比较 n 次，哨兵写法要比较 n+1 次（最后与哨兵比较一次）
    // 教材上常写 ASL(失败) = n+1，指的就是带哨兵写法下失败时的比较次数
    int cmp1, cmp2;
    SearchSeq(L, -1, cmp1);
    SearchSentry(L, -1, cmp2);
    printf("ASL(失败)：普通写法 %d 次（= n），哨兵写法 %d 次（= n+1）\n", cmp1, cmp2);
}

int main() {
    SSTable L;
    InitList(L, 10);  // 元素为 3 5 7 9 11 13 15 17 19 21
    PrintList(L);

    // 1. 查找成功：普通写法从表头开始找，比较次数正好等于元素的位序；
    //    哨兵写法从表尾往回找，比较次数 = n - 位序 + 1（两种写法的平均比较次数是一样的）
    printf("\n=== 查找成功 ===\n");
    int keys[] = {3, 11, 21, 7};
    for (int i = 0; i < 4; i++) {
        int cmp1, cmp2;
        int pos1 = SearchSeq(L, keys[i], cmp1);
        int pos2 = SearchSentry(L, keys[i], cmp2);
        printf("查找 %-3d：普通写法比较 %d 次（位序 %d）；哨兵写法比较 %d 次（位序 %d）\n",
               keys[i], cmp1, pos1, cmp2, pos2);
    }

    // 2. 查找失败：普通写法比较 n 次，哨兵写法比较 n+1 次，两者都返回 0
    printf("\n=== 查找失败 ===\n");
    int absent[] = {1, 8, 100};
    for (int i = 0; i < 3; i++) {
        int cmp1, cmp2;
        int pos1 = SearchSeq(L, absent[i], cmp1);
        int pos2 = SearchSentry(L, absent[i], cmp2);
        printf("查找 %-3d：普通写法比较 %d 次（返回 %d）；哨兵写法比较 %d 次（返回 %d）\n",
               absent[i], cmp1, pos1, cmp2, pos2);
    }

    // 3. 平均查找长度
    printf("\n=== 平均查找长度 ===\n");
    PrintASL(L);

    printf("\n结论：顺序查找的 ASL 与 n 成正比，n 较大时效率低；\n");
    printf("      它的优点是既不要求顺序存储，也不要求元素有序，必要时还可以使用哨兵简化循环条件。\n");
    return 0;
}
