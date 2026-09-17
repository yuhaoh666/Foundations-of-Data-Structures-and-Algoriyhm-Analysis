// 折半查找（二分查找）：只适用于顺序存储的有序表
//   每次用 mid = (low + high) / 2 处的关键字与目标比较，把查找区间缩小一半
//   时间复杂度 O(log n)，平均查找长度 ASL(成功) ≈ log2(n+1) - 1
//   链表不能用折半查找（无法随机存取中间元素），这是它的主要限制
// 本文件给出迭代与递归两种实现，并按“判定树”精确统计成功、失败的 ASL：
//   判定树是一棵平衡二叉树，每个结点对应一次成功的比较，
//   每个空指针（失败结点）对应一次失败的查找，第 h 层的结点需要比较 h 次
#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int data[MAXSIZE];  // 下标 0 开始存放有序序列
    int length;         // 元素个数
} SSTable;

// 用给定的有序数组初始化顺序表
void InitList(SSTable &L, const int arr[], int n) {
    for (int i = 0; i < n; i++) {
        L.data[i] = arr[i];
    }
    L.length = n;
}

// 打印顺序表
void PrintList(SSTable L) {
    printf("有序表(长度=%d)：", L.length);
    for (int i = 0; i < L.length; i++) {
        printf("%d ", L.data[i]);
    }
    printf("\n");
}

// 迭代版折半查找：找到返回下标，未找到返回 -1
int BinarySearch(SSTable L, int key) {
    int low = 0, high = L.length - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;  // 这样写可以避免 low + high 溢出
        if (L.data[mid] == key) {
            return mid;
        } else if (L.data[mid] > key) {
            high = mid - 1;  // 到左半区间找
        } else {
            low = mid + 1;   // 到右半区间找
        }
    }
    return -1;
}

// 递归版折半查找
int BinarySearchRec(SSTable L, int low, int high, int key) {
    if (low > high) {
        return -1;  // 区间为空，查找失败
    }
    int mid = low + (high - low) / 2;
    if (L.data[mid] == key) {
        return mid;
    }
    if (L.data[mid] > key) {
        return BinarySearchRec(L, low, mid - 1, key);
    }
    return BinarySearchRec(L, mid + 1, high, key);
}

// 带路径的折半查找：把依次比较过的元素记到 path[] 中，便于观察查找过程
// 返回找到的下标或 -1，len 带回比较次数
int BinarySearchWithPath(SSTable L, int key, int path[], int &len) {
    len = 0;
    int low = 0, high = L.length - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        path[len++] = L.data[mid];
        if (L.data[mid] == key) {
            return mid;
        } else if (L.data[mid] > key) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return -1;
}

// 打印查找路径
void PrintPath(const int path[], int len, int key, int pos) {
    if (pos == -1) {
        printf("查找 %-4d 未找到，依次比较了 %d 次：", key, len);
    } else {
        printf("查找 %-4d 找到（下标 %d），依次比较了 %d 次：", key, pos, len);
    }
    for (int i = 0; i < len; i++) {
        printf("%d ", path[i]);
    }
    printf("\n");
}

// 统计判定树各层的成功结点数与失败结点数
// low、high 是当前子树对应的下标区间，depth 是当前根结点的层号（树根为第 1 层）
void CountDecisionTree(SSTable L, int low, int high, int depth, int success[], int failure[],
                       int &maxDepth) {
    if (depth > maxDepth) {
        maxDepth = depth;
    }
    if (low > high) {  // 区间为空：对应一个失败结点（空指针），它落在第 depth 层
        failure[depth]++;
        return;
    }
    int mid = low + (high - low) / 2;
    success[depth]++;  // 结点 mid 在第 depth 层，查找它要比较 depth 次
    CountDecisionTree(L, low, mid - 1, depth + 1, success, failure, maxDepth);
    CountDecisionTree(L, mid + 1, high, depth + 1, success, failure, maxDepth);
}

// 用缩进表示层数打印判定树（同一层先输出左子树（较小的一半），再输出右子树）
void PrintDecisionTree(SSTable L, int low, int high, int depth) {
    if (low > high) {
        return;
    }
    int mid = low + (high - low) / 2;
    for (int i = 1; i < depth; i++) {
        printf("    ");
    }
    printf("%d\n", L.data[mid]);
    PrintDecisionTree(L, low, mid - 1, depth + 1);
    PrintDecisionTree(L, mid + 1, high, depth + 1);
}

// 计算并打印平均查找长度：ASL = Σ(第 h 层结点个数 × h) / 结点个数
void PrintASL(SSTable L) {
    int success[MAXSIZE] = {0}, failure[MAXSIZE] = {0};
    int maxDepth = 0;
    CountDecisionTree(L, 0, L.length - 1, 1, success, failure, maxDepth);

    int sSum = 0, sCnt = 0, fSum = 0, fCnt = 0;
    int treeHeight = 0;
    for (int d = 1; d <= maxDepth + 1; d++) {
        printf("  第 %d 层：成功结点 %d 个，失败结点 %d 个\n", d, success[d], failure[d]);
        sSum += success[d] * d;
        sCnt += success[d];
        fSum += failure[d] * d;
        fCnt += failure[d];
        if (success[d] > 0) {
            treeHeight = d;  // 成功结点所在的最大层号就是判定树的树高
        }
    }
    printf("ASL(成功) = %d/%d ≈ %.3f（查找表中元素平均要比较的次数）\n", sSum, sCnt,
           (double)sSum / sCnt);
    printf("ASL(失败) = %d/%d ≈ %.3f（查找失败平均要比较的次数）\n", fSum, fCnt,
           (double)fSum / fCnt);
    printf("（判定树树高 = %d = ⌈log2(n+1)⌉，即比较次数不会超过这个值；n=11 时教材经典结论："
           "ASL成功 = 3，ASL失败 = 56/12 ≈ 4.67）\n",
           treeHeight);
}

int main() {
    // 教材上常用的例子：11 个元素的有序表
    int arr[] = {5, 13, 19, 21, 37, 56, 64, 75, 80, 88, 92};
    int n = sizeof(arr) / sizeof(arr[0]);
    SSTable L;
    InitList(L, arr, n);
    PrintList(L);

    // 1. 查找过程：迭代实现与递归实现的结果应当完全一致
    printf("\n=== 查找过程（路径上的元素就是依次比较过的元素）===\n");
    int keys[] = {56, 19, 92, 5, 40, 100};
    int path[MAXSIZE], len;
    for (int i = 0; i < 6; i++) {
        int pos = BinarySearchWithPath(L, keys[i], path, len);
        PrintPath(path, len, keys[i], pos);
        int pos2 = BinarySearchRec(L, 0, n - 1, keys[i]);
        printf("                递归实现得到下标 %d，与迭代实现%s\n", pos2,
               pos == pos2 ? "一致" : "不一致");
    }

    // 2. 判定树与平均查找长度
    printf("\n=== 折半查找的判定树（缩进表示层数，先输出较小的一半）===\n");
    PrintDecisionTree(L, 0, n - 1, 1);

    printf("\n=== 平均查找长度 ===\n");
    PrintASL(L);

    printf("\n结论：折半查找每次都能排除一半的区间，比较次数不超过树高，效率远高于顺序查找；\n");
    printf("      代价是表必须采用顺序存储且按关键字有序（插入、删除元素时要维持有序性）。\n");
    return 0;
}
