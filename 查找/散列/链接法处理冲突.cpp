// 散列表的链接法（拉链法）处理冲突：把散列到同一地址的所有记录用一条单链表串起来
//   散列表本身是一个指针数组，每个元素指向一条“同义词链表”，初始时全为空
// 与开放定址法相比：
//   1. 不会产生“堆积”，删除时也不需要墓碑，直接摘链即可
//   2. 装填因子 α 可以大于 1，查找成功的平均查找长度约 1 + α/2，失败约 α
//   3. 缺点是要额外占用指针空间，而且链表结点不连续，缓存不友好
// 本文件采用头插法建链（新记录插在链表最前面，插入只需 O(1)），查找与删除都要遍历链表
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 30

typedef struct HashNode {
    int key;                // 关键字
    struct HashNode *next;  // 指向同义词链表中的下一个结点
} HashNode;

typedef struct {
    HashNode *head[MAXSIZE];  // 表长 m 个“桶”，每个桶存一条链表的头指针
    int m;                    // 表长
    int p;                    // 散列函数 H(key) = key % p
    int count;                // 记录总数
} HashTable;

// 初始化：所有桶都为空
void InitHashTable(HashTable &H, int m, int p) {
    for (int i = 0; i < m; i++) {
        H.head[i] = NULL;
    }
    H.m = m;
    H.p = p;
    H.count = 0;
}

// 散列函数：除留余数法
int Hash(HashTable H, int key) {
    return key % H.p;
}

// 新建一个链表结点
HashNode *CreateNode(int key) {
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->key = key;
    node->next = NULL;
    return node;
}

// 插入：同一个桶里用头插法（不要求桶内有序，插到最前面最省事）
// 关键字已存在时插入失败；bucket 带回它所在的桶号
bool InsertKey(HashTable &H, int key, int &bucket) {
    bucket = Hash(H, key);
    for (HashNode *p = H.head[bucket]; p != NULL; p = p->next) {  // 先检查是否重复
        if (p->key == key) {
            return false;
        }
    }
    HashNode *node = CreateNode(key);
    if (node == NULL) {
        return false;
    }
    node->next = H.head[bucket];  // 头插
    H.head[bucket] = node;
    H.count++;
    return true;
}

// 查找：找到返回结点指针，未找到返回 NULL；cmp 带回比较次数
HashNode *SearchNode(HashTable H, int key, int &cmp) {
    int bucket = Hash(H, key);
    cmp = 0;
    for (HashNode *p = H.head[bucket]; p != NULL; p = p->next) {
        cmp++;
        if (p->key == key) {
            return p;
        }
    }
    return NULL;
}

// 删除：找到后摘掉链表结点并释放内存
bool DeleteKey(HashTable &H, int key) {
    int bucket = Hash(H, key);
    HashNode *pre = NULL;
    for (HashNode *p = H.head[bucket]; p != NULL; pre = p, p = p->next) {
        if (p->key != key) {
            continue;
        }
        if (pre == NULL) {          // 删除的是链表第一个结点
            H.head[bucket] = p->next;
        } else {                    // 删除的是中间或末尾结点
            pre->next = p->next;
        }
        free(p);
        H.count--;
        return true;
    }
    return false;
}

// 打印散列表：每个桶一行，空桶显示“空”，桶内用 -> 连接同义词
void PrintTable(HashTable H, const char *title) {
    printf("%s（表长 %d，记录数 %d，装填因子 α = %d/%d ≈ %.2f）：\n", title, H.m, H.count,
           H.count, H.m, (double)H.count / H.m);
    for (int i = 0; i < H.m; i++) {
        printf("  桶[%2d] ", i);
        if (H.head[i] == NULL) {
            printf("空");
        }
        for (HashNode *p = H.head[i]; p != NULL; p = p->next) {
            printf("%d", p->key);
            if (p->next != NULL) {
                printf(" -> ");
            }
        }
        printf("\n");
    }
}

// 查找并打印结果（含比较次数）
void PrintSearch(HashTable H, int key) {
    int cmp;
    HashNode *p = SearchNode(H, key, cmp);
    if (p == NULL) {
        printf("查找 %-3d：未找到（比较了 %d 次）", key, cmp);
    } else {
        printf("查找 %-3d：找到（桶 %d，比较了 %d 次）", key, Hash(H, key), cmp);
    }
}

// 统计平均查找长度：
//   ASL(成功)：对每个已有关键字查找一次，求比较次数的平均值，理论值约 1 + α/2
//   ASL(失败)：从每个桶出发沿链一直走到末尾，比较次数就是该条链的长度，理论值约 α
void PrintASL(HashTable H) {
    int total = 0, cnt = 0;
    for (int i = 0; i < H.m; i++) {
        for (HashNode *p = H.head[i]; p != NULL; p = p->next) {
            int cmp;
            SearchNode(H, p->key, cmp);
            total += cmp;
            cnt++;
        }
    }
    printf("  ASL(成功) = %d/%d ≈ %.2f（理论值 1 + α/2 ≈ %.2f）\n", total, cnt,
           (double)total / cnt, 1 + (double)H.count / H.m / 2);

    int failTotal = 0;
    for (int i = 0; i < H.m; i++) {
        for (HashNode *p = H.head[i]; p != NULL; p = p->next) {
            failTotal++;  // 把整条链走完才能判定失败，比较次数即链长
        }
    }
    printf("  ASL(失败) = %d/%d ≈ %.2f（即平均链长 α ≈ %.2f）\n", failTotal, H.m,
           (double)failTotal / H.m, (double)H.count / H.m);
}

// 销毁散列表：释放所有链表结点
void DestroyHashTable(HashTable &H) {
    for (int i = 0; i < H.m; i++) {
        HashNode *p = H.head[i];
        while (p != NULL) {
            HashNode *t = p;
            p = p->next;
            free(t);
        }
        H.head[i] = NULL;
    }
    H.count = 0;
}

int main() {
    // 用与“开放定址法处理冲突.cpp”相同的关键字序列，便于比较两种冲突处理方式
    int keys[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    int n = sizeof(keys) / sizeof(keys[0]);
    int m = 13, p = 13;  // 拉链法的表长可以直接取质数，装填因子 α = 12/13 也不会溢出

    printf("散列函数：H(key) = key %% %d，表长 m = %d（拉链法允许 α 大于 1）\n", p, m);
    printf("关键字序列（共 %d 个）：", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", keys[i]);
    }
    printf("\n");

    HashTable H;
    InitHashTable(H, m, p);

    // 1. 依次插入
    printf("\n=== 1. 依次插入 ===\n");
    for (int i = 0; i < n; i++) {
        int bucket;
        InsertKey(H, keys[i], bucket);
        printf("  插入 %-3d：H(%d) = %-2d，放入桶 %-2d（头插到链表最前面）\n", keys[i], keys[i],
               bucket, bucket);
    }
    int tmpBucket;
    printf("  再插入一次 79：%s\n",
           InsertKey(H, 79, tmpBucket) ? "插入成功" : "插入失败（关键字已存在）");
    PrintTable(H, "拉链法散列表");

    // 2. 查找
    printf("\n=== 2. 查找 ===\n");
    int findKeys[] = {23, 79, 12, 1, 44, 90};
    for (int i = 0; i < 6; i++) {
        PrintSearch(H, findKeys[i]);
        printf("\n");
    }

    // 3. 平均查找长度
    printf("\n=== 3. 平均查找长度 ===\n");
    PrintASL(H);

    // 4. 删除（不需要墓碑，直接摘链即可）
    printf("\n=== 4. 删除 ===\n");
    printf("  删除 19（桶 %d 的链首结点）：%s\n", Hash(H, 19), DeleteKey(H, 19) ? "成功" : "失败");
    printf("  删除 55：%s\n", DeleteKey(H, 55) ? "成功" : "失败");
    printf("  删除 100（表中没有）：%s\n", DeleteKey(H, 100) ? "成功" : "失败");
    PrintTable(H, "删除后的散列表");
    printf("  删除后再查找 23：");
    PrintSearch(H, 23);
    printf("\n");

    DestroyHashTable(H);

    printf("\n结论：拉链法用链表解决冲突，插入为 O(1)，查找与删除约 O(1 + α)；\n");
    printf("      它不像开放定址法那样受装填因子限制（α 可以大于 1），删除也无需墓碑；\n");
    printf("      代价是需要额外的指针空间，实践中还会把过长的桶内链表换成红黑树（如 Java 的 HashMap）。\n");
    return 0;
}
