// 散列表（哈希表）的开放定址法：用散列函数算出地址，若该位置已被占用，就按探测序列另找空位
//   线性探测法：d_i = 1, 2, 3, ...，依次往后找
//   平方探测法：d_i = 1², -1², 2², -2², ...，先往后、再往前跳着找，可以缓解“聚集”
// 三个要点：
//   1. 装填因子 α = 表中记录数 / 表长，α 越大冲突越多、ASL 越大（开放定址法必须 α < 1）
//   2. 查找时“遇到空位置就可以断定失败”，因此删除元素不能直接把位置清空，
//      必须留下“墓碑”标记，否则会截断别的关键字的探测链
//   3. 实际 ASL 由探测次数统计而来：
//      ASL(成功) 统计每个已有关键字的探测次数；ASL(失败) 统计从每个地址出发探测到空位置为止的次数
#include <stdio.h>
#define MAXSIZE 30
#define EMPTY 0    // 该位置从未使用过
#define ACTIVE 1   // 该位置存有记录
#define DELETED 2  // 该位置曾有记录，现已被删除（墓碑）

typedef struct {
    int key;    // 关键字（状态不是 ACTIVE 时无意义）
    int state;  // 状态标记：EMPTY / ACTIVE / DELETED
} HashSlot;

typedef struct {
    HashSlot slot[MAXSIZE];  // 散列表的存储空间
    int m;                   // 表长（地址范围 0 ~ m-1）
    int p;                   // 散列函数 H(key) = key % p，p 通常取不超过表长的最大质数
    int count;               // 当前记录个数
    int method;              // 0 表示线性探测，1 表示平方探测
} HashTable;

// 初始化散列表：所有位置均为空
void InitHashTable(HashTable &H, int m, int p, int method) {
    for (int i = 0; i < m; i++) {
        H.slot[i].key = 0;
        H.slot[i].state = EMPTY;
    }
    H.m = m;
    H.p = p;
    H.count = 0;
    H.method = method;
}

// 散列函数：除留余数法
int Hash(HashTable H, int key) {
    return key % H.p;
}

// 求第 i 次探测的地址（i 从 1 开始计）：
//   线性探测：base+1, base+2, base+3, ...
//   平方探测：base+1², base-1², base+2², base-2², ...
int ProbePos(HashTable H, int base, int i) {
    if (H.method == 0) {
        return (base + i) % H.m;
    }
    int half = (i + 1) / 2;      // i=1、2 用 1；i=3、4 用 2；以此类推
    int offset = half * half;    // 偏移量的平方
    if (i % 2 == 1) {            // 奇数次探测：往后跳
        return (base + offset) % H.m;
    }
    return ((base - offset) % H.m + H.m) % H.m;  // 偶数次探测：往前跳（注意负数取模）
}

// 插入：返回是否插入成功；probe 返回探测次数
bool InsertKey(HashTable &H, int key, int &probe) {
    probe = 0;
    if (H.count >= H.m) {
        printf("  散列表已满，插入 %d 失败！\n", key);
        return false;
    }
    int base = Hash(H, key);
    for (int i = 1; i <= H.m; i++) {  // 探测次数不超过表长，保证一定终止
        int pos = ProbePos(H, base, i);
        probe++;
        if (H.slot[pos].state != ACTIVE) {  // 空位置或墓碑都可以放
            H.slot[pos].key = key;
            H.slot[pos].state = ACTIVE;
            H.count++;
            return true;
        }
    }
    return false;
}

// 查找：找到返回下标，未找到返回 -1；probe 返回探测（比较）次数
int SearchKey(HashTable H, int key, int &probe) {
    probe = 0;
    int base = Hash(H, key);
    for (int i = 1; i <= H.m; i++) {
        int pos = ProbePos(H, base, i);
        probe++;
        if (H.slot[pos].state == EMPTY) {
            return -1;  // 探测链上遇到空位置，说明此关键字不存在
        }
        if (H.slot[pos].state == ACTIVE && H.slot[pos].key == key) {
            return pos;
        }
        // 墓碑位置要继续往后探测，不能在这里停下
    }
    return -1;
}

// 删除：只把状态改成“墓碑”，不能真正清空，否则会截断探测链
bool DeleteKey(HashTable &H, int key) {
    int probe;
    int pos = SearchKey(H, key, probe);
    if (pos == -1) {
        return false;
    }
    H.slot[pos].state = DELETED;
    H.count--;
    return true;
}

// 打印散列表：ACTIVE 显示关键字，EMPTY 显示“空”，DELETED 显示“墓碑”
void PrintTable(HashTable H, const char *title) {
    printf("%s（表长 %d，记录数 %d，装填因子 α = %d/%d ≈ %.2f）：\n", title, H.m, H.count,
           H.count, H.m, (double)H.count / H.m);
    for (int i = 0; i < H.m; i++) {
        printf("  [%2d] ", i);
        if (H.slot[i].state == ACTIVE) {
            printf("%-4d", H.slot[i].key);
        } else if (H.slot[i].state == EMPTY) {
            printf("%-4s", "空");
        } else {
            printf("%-4s", "墓碑");
        }
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

// 查找并打印结果（含探测次数）
void PrintSearch(HashTable H, int key) {
    int probe;
    int pos = SearchKey(H, key, probe);
    if (pos == -1) {
        printf("查找 %-3d：未找到（探测 %d 次）", key, probe);
    } else {
        printf("查找 %-3d：找到（地址 %d，探测 %d 次）", key, pos, probe);
    }
}

// 插入并打印落点，便于观察冲突与探测过程
void InsertAndPrint(HashTable &H, int key) {
    int base = Hash(H, key);
    int probe;
    InsertKey(H, key, probe);
    int pos = -1;
    for (int i = 0; i < H.m; i++) {  // 找到刚插入记录所在的位置
        if (H.slot[i].state == ACTIVE && H.slot[i].key == key) {
            pos = i;
            break;
        }
    }
    printf("  插入 %-3d：H(%d) = %-2d，探测 %d 次后落在地址 %d\n", key, key, base, probe, pos);
}

// 统计平均查找长度：
//   ASL(成功)：对每个已有关键字查找一次，求探测次数的平均值
//   ASL(失败)：假设散列地址均匀分布，从每个地址出发按探测序列走到第一个空位置为止，求平均值
void PrintASL(HashTable H, const char *title) {
    int total = 0, cnt = 0;
    for (int i = 0; i < H.m; i++) {
        if (H.slot[i].state == ACTIVE) {
            int probe;
            SearchKey(H, H.slot[i].key, probe);
            total += probe;
            cnt++;
        }
    }
    printf("  %s ASL(成功) = %d/%d ≈ %.2f\n", title, total, cnt, (double)total / cnt);

    int failTotal = 0;
    for (int base = 0; base < H.m; base++) {  // 每个地址都可能是一个失败查找的起点
        int probe = 0;
        for (int i = 1; i <= H.m; i++) {
            int pos = ProbePos(H, base, i);
            probe++;
            if (H.slot[pos].state == EMPTY) {  // 探测到空位置，判定失败
                break;
            }
        }
        failTotal += probe;
    }
    printf("  %s ASL(失败) = %d/%d ≈ %.2f\n", title, failTotal, H.m, (double)failTotal / H.m);
}

int main() {
    // 教材上的经典例子：H(key) = key % 13，表长 m = 16
    int keys[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    int n = sizeof(keys) / sizeof(keys[0]);
    int m = 16, p = 13;

    printf("散列函数：H(key) = key %% %d，表长 m = %d\n", p, m);
    printf("关键字序列（共 %d 个）：", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", keys[i]);
    }
    printf("\n");

    // 1. 用线性探测法依次插入
    HashTable H1;
    InitHashTable(H1, m, p, 0);  // method = 0 表示线性探测
    printf("\n=== 1. 线性探测法建表 ===\n");
    for (int i = 0; i < n; i++) {
        InsertAndPrint(H1, keys[i]);
    }
    PrintTable(H1, "线性探测法的散列表");

    // 2. 用平方探测法依次插入
    HashTable H2;
    InitHashTable(H2, m, p, 1);  // method = 1 表示平方探测
    printf("\n=== 2. 平方探测法建表 ===\n");
    for (int i = 0; i < n; i++) {
        InsertAndPrint(H2, keys[i]);
    }
    PrintTable(H2, "平方探测法的散列表");

    // 3. 查找：比较两种方法的探测次数
    printf("\n=== 3. 查找 ===\n");
    int findKeys[] = {23, 79, 12, 1, 44, 90};
    for (int i = 0; i < 6; i++) {
        printf("  线性探测法：");
        PrintSearch(H1, findKeys[i]);
        printf("\n  平方探测法：");
        PrintSearch(H2, findKeys[i]);
        printf("\n");
    }

    // 4. 平均查找长度与装填因子
    double alpha = (double)n / m;
    printf("\n=== 4. 平均查找长度（装填因子 α = %d/%d ≈ %.2f）===\n", n, m, alpha);
    PrintASL(H1, "线性探测法：");
    PrintASL(H2, "平方探测法：");
    printf("  线性探测的理论近似值（Knuth）：ASL(成功) ≈ ½(1 + 1/(1-α)) = %.2f，"
           "ASL(失败) ≈ ½(1 + 1/(1-α)²) = %.2f\n",
           0.5 * (1 + 1 / (1 - alpha)), 0.5 * (1 + 1 / ((1 - alpha) * (1 - alpha))));

    // 5. 删除与“墓碑”标记
    printf("\n=== 5. 删除与“墓碑” ===\n");
    printf("  删除 27（线性探测法）：%s\n", DeleteKey(H1, 27) ? "成功" : "失败");
    printf("  删除 68（平方探测法）：%s\n", DeleteKey(H2, 68) ? "成功" : "失败");
    PrintTable(H1, "删除后的线性探测散列表");
    printf("  再查找 55：");
    PrintSearch(H1, 55);  // 55 的探测链会经过 27 原来的地址，靠“墓碑”才能继续往后找
    printf("\n  再查找 27：");
    PrintSearch(H1, 27);
    printf("\n");

    printf("\n结论：装填因子 α 越大，冲突越多、ASL 越大，开放定址法必须保证 α < 1；\n");
    printf("      线性探测实现简单但容易“堆积”，平方探测能缓解堆积，代价是探测序列要跳跃着算；\n");
    printf("      删除元素必须打“墓碑”，否则会截断探测链，导致本应找得到的关键字查不到。\n");
    return 0;
}
