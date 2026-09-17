// 置换-选择排序：在内存工作区大小固定为 w 的条件下生成初始归并段
// 它的价值在于：让每个初始归并段的长度大于工作区大小，从而减少归并段个数与归并趟数
// 算法思想（用一个按“(所属段号, 关键字)”排序的小根堆维护工作区）：
//   1. 先把输入文件的前 w 个记录读入工作区，建成小根堆，令当前段号 cur = 1
//   2. 反复取出堆顶记录（它是当前段中关键字最小的记录）输出到当前归并段；
//      再从输入文件读入下一个记录 x，用它顶替刚空出来的堆顶位置：
//        · 若 x >= 刚输出的记录，说明当前段可以继续保持递增，x 留在当前段（段号 cur）
//        · 否则 x 只能留到下一段去（段号 cur + 1）
//      然后向下调整堆
//   3. 当堆顶记录的段号不再是 cur 时，说明当前段已经输出完毕：
//      记下当前段的长度，段号加一，开始生成新的一段
//   4. 输入文件读完且堆空时，所有归并段都生成完毕
// 结论：除最后一段外，每个归并段的长度都不小于 w，平均长度约为 2w，
//       因此初始归并段的个数 m ≈ n/(2w)，而简单分块法得到的是 m = ⌈n/w⌉，几乎差一倍
#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int key;  // 关键字
    int seg;  // 该记录所属的归并段号（从 1 开始）
} Record;

// 堆中元素的大小比较：先比段号，再比关键字
// 这样堆顶一定是“当前段中关键字最小的记录”，段号更大的记录都排在后面
bool Less(Record a, Record b) {
    return a.seg < b.seg || (a.seg == b.seg && a.key < b.key);
}

typedef struct {
    Record heap[MAXSIZE];  // 小根堆（按上面的 Less 定义）
    int size;              // 堆中记录个数
} WorkArea;

// 堆的下沉调整：把下标 i 处的记录向下调整到合适的位置，维持堆序
void SiftDown(WorkArea &W, int i) {
    while (true) {
        int smallest = i;
        int left = 2 * i + 1;   // 左孩子
        int right = 2 * i + 2;  // 右孩子
        if (left < W.size && Less(W.heap[left], W.heap[smallest])) {
            smallest = left;
        }
        if (right < W.size && Less(W.heap[right], W.heap[smallest])) {
            smallest = right;
        }
        if (smallest == i) {  // 已经满足堆序，调整结束
            break;
        }
        Record t = W.heap[i];
        W.heap[i] = W.heap[smallest];
        W.heap[smallest] = t;
        i = smallest;  // 继续沿着被交换的那条路向下调整
    }
}

// 置换-选择排序：input[] 是待处理的记录序列，n 为记录数，w 为内存工作区能容纳的记录数
// runs[] 返回每个初始归并段的长度，返回值为归并段个数 m
int ReplacementSelection(const int input[], int n, int w, int runs[]) {
    WorkArea W;
    W.size = 0;
    int readPos = 0;  // 输入文件中下一个待读记录的下标

    for (int i = 0; i < w && readPos < n; i++) {  // 前 w 个记录先读入工作区
        W.heap[i].key = input[readPos++];
        W.heap[i].seg = 1;  // 最初都属于第 1 段
        W.size++;
    }
    for (int i = W.size / 2 - 1; i >= 0; i--) {  // 从最后一个非叶结点开始建小根堆
        SiftDown(W, i);
    }

    int m = 0;       // 已经生成的归并段个数
    int cur = 1;     // 当前正在输出的段号
    int curLen = 0;  // 当前段的长度
    printf("  工作区大小 w = %d，输出与读入的对应关系：\n", w);

    while (W.size > 0) {
        if (W.heap[0].seg != cur) {  // 堆顶已经属于下一段，当前段输出完毕
            runs[m++] = curLen;
            cur = W.heap[0].seg;
            curLen = 0;
        }
        int lastOut = W.heap[0].key;  // 取出堆顶（当前段中最小者）并输出
        curLen++;
        printf("    输出 %-4d（第 %d 段）", lastOut, cur);

        if (readPos < n) {  // 从输入文件补入一个记录，顶替刚空出的堆顶
            int x = input[readPos++];
            W.heap[0].key = x;
            W.heap[0].seg = (x >= lastOut) ? cur : cur + 1;  // 比刚输出的记录小就留给下一段
            printf("，读入 %-4d → 归入第 %d 段", x, W.heap[0].seg);
        } else {  // 输入文件已读完，堆的规模缩小一个
            W.heap[0] = W.heap[--W.size];
        }
        printf("\n");

        if (W.size > 0) {
            SiftDown(W, 0);  // 调整堆，让堆顶重新成为“当前段最小者”
        }
    }
    runs[m++] = curLen;  // 最后一段
    return m;
}

// 打印归并段的结果，并与“简单分块”的做法对比
void PrintRuns(const int runs[], int m, int n, int w) {
    int sum = 0;
    printf("  共生成 %d 个初始归并段：", m);
    for (int i = 0; i < m; i++) {
        printf("[第%d段 长度=%d] ", i + 1, runs[i]);
        sum += runs[i];
    }
    printf("\n  各段长度之和 = %d，记录总数 n = %d，是否一致：%s\n", sum, n,
           sum == n ? "是" : "否");
    printf("  平均段长 = %.2f（工作区 w = %d，理论上平均约 2w = %d）\n", (double)sum / m, w,
           2 * w);
    printf("  对比“简单分块 + 内部排序”：n = %d、w = %d 时只能得到 ⌈n/w⌉ = %d 个归并段\n", n, w,
           (n + w - 1) / w);
}

int main() {
    // 教材上的例子：9 个记录，而内存工作区只能容纳 3 个记录
    int input[] = {17, 21, 5, 44, 10, 12, 56, 32, 29};
    int n = sizeof(input) / sizeof(input[0]);
    int w = 3;
    int runs[MAXSIZE];

    printf("待排序文件（共 %d 个记录）：", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", input[i]);
    }
    printf("\n\n=== 例 1：置换-选择排序生成初始归并段 ===\n");
    int m = ReplacementSelection(input, n, w, runs);

    printf("\n=== 例 1 结果 ===\n");
    PrintRuns(runs, m, n, w);

    // 再看一个更长的例子，工作区开得更大一些
    int input2[] = {35, 12, 68, 4, 90, 27, 51, 15, 73, 8, 42, 61, 19, 84, 30, 55, 2, 47, 66, 23};
    int n2 = sizeof(input2) / sizeof(input2[0]);
    int w2 = 4;
    int runs2[MAXSIZE];
    printf("\n=== 例 2：20 个记录、工作区 w = %d ===\n", w2);
    int m2 = ReplacementSelection(input2, n2, w2, runs2);
    printf("\n=== 例 2 结果 ===\n");
    PrintRuns(runs2, m2, n2, w2);

    printf("\n结论：置换-选择排序让每个初始归并段的长度都不小于工作区大小 w，平均长度约为 2w；\n");
    printf("      于是归并段个数由 ⌈n/w⌉ 降到约 n/(2w)，归并趟数 S = ⌈log_k m⌉ 也随之减小；\n");
    printf("      代价是输出顺序与输入顺序不一致，必须借助堆来选择“当前段中的最小记录”。\n");
    return 0;
}
