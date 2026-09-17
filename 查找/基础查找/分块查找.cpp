// 分块查找（又叫索引顺序查找）：介于顺序查找与折半查找之间的一种折中方案
//   要求：把表分成若干块，块内元素可以无序，但块与块之间必须“分块有序”
//         （即第 i 块中所有关键字都小于第 i+1 块中的任何一个关键字）
//   另外要建一张索引表，每一项记录一块的最大关键字和该块的起始下标
// 查找过程：
//   1. 先在索引表中确定目标可能所在的块（索引表有序，既可以顺序查找，也可以折半查找）
//   2. 再在该块内顺序查找
// 平均查找长度 ASL = ASL(索引表) + ASL(块内)：
//   设块长为 s、块数为 b = n/s，索引表用顺序查找时 ASL = (b+1)/2 + (s+1)/2，
//   当 s = √n 时取最小值 ASL ≈ √n + 1，明显优于顺序查找的 (n+1)/2
#include <stdio.h>
#define MAXSIZE 100  // 数据元素个数上限
#define MAXBLOCK 20  // 块数上限

typedef struct {
    int data[MAXSIZE];  // 按块存放的元素（块内无序，块间有序）
    int length;         // 元素个数
} BlockList;

typedef struct {
    int maxKey;  // 该块中的最大关键字（各块之间递增）
    int start;   // 该块第一个元素在 data[] 中的下标
    int length;  // 该块的元素个数
} IndexItem;

typedef struct {
    IndexItem item[MAXBLOCK];  // 索引表（有序表）
    int blockNum;              // 块数
} IndexTable;

// 建立顺序表：这里直接给出“块内无序、块间有序”的数据，每 3 个元素为一块
//   第 1 块 {12, 8, 22}  第 2 块 {25, 30, 33}  第 3 块 {36, 38, 42}
//   第 4 块 {45, 50, 55} 第 5 块 {58, 62, 66}
void InitList(BlockList &L) {
    int data[] = {12, 8,  22, 25, 30, 33, 36, 38,
                  42, 45, 50, 55, 58, 62, 66};
    L.length = sizeof(data) / sizeof(data[0]);
    for (int i = 0; i < L.length; i++) {
        L.data[i] = data[i];
    }
}

// 按固定块长建立索引表：每块的 maxKey 取块内最大值，start 记录该块的首下标
void BuildIndex(BlockList L, IndexTable &idx, int blockSize) {
    idx.blockNum = 0;
    for (int start = 0; start < L.length; start += blockSize) {
        int len = (start + blockSize <= L.length) ? blockSize : L.length - start;
        int maxKey = L.data[start];
        for (int i = start + 1; i < start + len; i++) {  // 求本块的最大关键字
            if (L.data[i] > maxKey) {
                maxKey = L.data[i];
            }
        }
        idx.item[idx.blockNum].maxKey = maxKey;
        idx.item[idx.blockNum].start = start;
        idx.item[idx.blockNum].length = len;
        idx.blockNum++;
    }
}

// 打印顺序表（按块分行，便于观察“块内无序、块间有序”）
void PrintList(BlockList L, int blockSize) {
    printf("顺序表（共 %d 个元素，块长 %d）：\n", L.length, blockSize);
    for (int start = 0; start < L.length; start += blockSize) {
        int len = (start + blockSize <= L.length) ? blockSize : L.length - start;
        printf("  第 %d 块（下标 %2d ~ %2d）：", start / blockSize + 1, start,
               start + len - 1);
        for (int i = start; i < start + len; i++) {
            printf("%d ", L.data[i]);
        }
        printf("\n");
    }
}

// 打印索引表
void PrintIndex(IndexTable idx) {
    printf("索引表（共 %d 块）：", idx.blockNum);
    for (int i = 0; i < idx.blockNum; i++) {
        printf("[第%d块 最大关键字=%d 起始下标=%d 长度=%d] ", i + 1, idx.item[i].maxKey,
               idx.item[i].start, idx.item[i].length);
    }
    printf("\n");
}

// 分块查找（索引表用顺序查找）：返回元素下标，未找到返回 -1
// indexCmp 带回索引表的比较次数，blockCmp 带回块内的比较次数
int BlockSearchSeq(BlockList L, IndexTable idx, int key, int &indexCmp, int &blockCmp) {
    indexCmp = blockCmp = 0;
    int b = -1;  // 目标可能所在的块号
    for (int i = 0; i < idx.blockNum; i++) {
        indexCmp++;
        if (key <= idx.item[i].maxKey) {  // 第一个“最大关键字 ≥ key”的块
            b = i;
            break;
        }
    }
    if (b == -1) {
        return -1;  // key 比所有块的最大关键字都大，直接判定失败
    }
    for (int i = idx.item[b].start; i < idx.item[b].start + idx.item[b].length; i++) {
        blockCmp++;
        if (L.data[i] == key) {
            return i;
        }
    }
    return -1;  // 块已定位，但块内没有该关键字
}

// 分块查找（索引表用折半查找）：索引表本身有序，所以也可以折半，
// 找“第一个 maxKey ≥ key”的块，写法就是折半查找的常见变形
int BlockSearchBinary(BlockList L, IndexTable idx, int key, int &indexCmp, int &blockCmp) {
    indexCmp = blockCmp = 0;
    int low = 0, high = idx.blockNum - 1, b = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        indexCmp++;
        if (idx.item[mid].maxKey >= key) {
            b = mid;  // 记下来，继续往左找“更靠前”的可行块
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    if (b == -1) {
        return -1;
    }
    for (int i = idx.item[b].start; i < idx.item[b].start + idx.item[b].length; i++) {
        blockCmp++;
        if (L.data[i] == key) {
            return i;
        }
    }
    return -1;
}

// 执行一次分块查找并打印比较次数：useBinary 为 true 表示索引表采用折半查找
void PrintSearchResult(BlockList L, IndexTable idx, int key, bool useBinary) {
    int indexCmp, blockCmp, pos;
    if (useBinary) {
        pos = BlockSearchBinary(L, idx, key, indexCmp, blockCmp);
    } else {
        pos = BlockSearchSeq(L, idx, key, indexCmp, blockCmp);
    }
    printf("  %s 查找 %-3d：", useBinary ? "索引表折半" : "索引表顺序", key);
    if (pos == -1) {
        printf("未找到");
    } else {
        printf("找到（下标 %d）", pos);
    }
    printf("，索引表比较 %d 次 + 块内比较 %d 次 = 共 %d 次\n", indexCmp, blockCmp,
           indexCmp + blockCmp);
}

// 求 ⌈log2(x)⌉（x ≥ 1），用于估算索引表折半查找的最大比较次数
int CeilLog2(int x) {
    int k = 0, p = 1;
    while (p < x) {
        p *= 2;
        k++;
    }
    return k;
}

int main() {
    BlockList L;
    IndexTable idx;
    int blockSize = 3;  // 块长
    InitList(L);
    BuildIndex(L, idx, blockSize);
    PrintList(L, blockSize);
    PrintIndex(idx);

    // 1. 查找过程：既查存在的关键字，也查不存在的关键字
    printf("\n=== 查找过程 ===\n");
    int keys[] = {30, 42, 66, 12, 20, 70, 35};
    for (int i = 0; i < 7; i++) {
        PrintSearchResult(L, idx, keys[i], false);
        PrintSearchResult(L, idx, keys[i], true);
    }

    // 2. 平均查找长度：对表中每个元素各查一次，统计平均比较次数
    printf("\n=== 平均查找长度（对表中每个元素各查一次）===\n");
    double sumSeq = 0, sumBin = 0;
    for (int i = 0; i < L.length; i++) {
        int ic, bc;
        BlockSearchSeq(L, idx, L.data[i], ic, bc);
        sumSeq += ic + bc;
        BlockSearchBinary(L, idx, L.data[i], ic, bc);
        sumBin += ic + bc;
    }
    printf("  索引表用顺序查找：ASL(成功) = %.2f（理论值 (b+1)/2 + (s+1)/2 = %.2f）\n",
           sumSeq / L.length, (idx.blockNum + 1) / 2.0 + (blockSize + 1) / 2.0);
    printf("  索引表用折半查找：ASL(成功) = %.2f（理论值 ⌈log2(b+1)⌉ + (s+1)/2 = %.2f）\n",
           sumBin / L.length, CeilLog2(idx.blockNum + 1) + (blockSize + 1) / 2.0);

    printf("\n结论：n = %d、块长 s = %d（理论上 s 取 √n ≈ 4 时 ASL 最小）；\n", L.length,
           blockSize);
    printf("      分块查找的 ASL ≈ √n + 1，远好于顺序查找，又不像折半查找那样要求整个表有序；\n");
    printf("      代价是需要额外维护索引表，并且块内无序时插入删除仍要搬动元素。\n");
    return 0;
}
