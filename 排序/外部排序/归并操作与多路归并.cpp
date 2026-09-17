// 归并（Merge）：把两个或多个有序序列合并成一个有序序列，是外部排序的核心操作
// 外部排序的两个阶段：
//   1. 生成初始归并段：按内存工作区的大小把大文件分成若干段，每段读入内存后用内部排序排好，
//      再写回外存，这样得到若干个有序的“初始归并段”
//   2. 多路归并：对这些归并段反复做 k 路归并，直到只剩下一个归并段为止
// 关键公式：
//   对 m 个初始归并段做 k 路归并，归并趟数 S = ⌈log_k m⌉
//   每一趟都要把全部 n 个记录读出、写入外存各一次，所以总 I/O 次数 = 2n·S
// 结论：增大归并路数 k、或者增大内存工作区以减少归并段个数 m，都能减少 I/O 次数；
//       但 k 越大，每次从 k 个当前记录中选出最小的比较次数就越多（朴素做法要 k-1 次），
//       这正是“败者树”（见 败者树.cpp）要解决的问题
#include <stdio.h>
#define MAXSIZE 100
#define MAXK 10
#define INF 32767  // 无穷大：表示某个归并段的记录已经全部取完

// ==================== 二路归并 ====================
// 把有序表 A[low..mid] 与有序表 A[mid+1..high] 归并成一段有序表
// 先合并到辅助数组 tmp 中，再整体写回 A，这样不会互相覆盖
void Merge(int A[], int low, int mid, int high, int tmp[]) {
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high) {  // 两个子表都还有元素时，取较小的一个
        if (A[i] <= A[j]) {
            tmp[k++] = A[i++];  // 写成 <= 才能在相等时先取左表，保证归并排序稳定
        } else {
            tmp[k++] = A[j++];
        }
    }
    while (i <= mid) {
        tmp[k++] = A[i++];  // 左边剩下的元素直接搬过去
    }
    while (j <= high) {
        tmp[k++] = A[j++];  // 右边剩下的元素直接搬过去
    }
    for (int t = low; t <= high; t++) {
        A[t] = tmp[t];  // 写回原数组
    }
}

// 二路归并的一趟：把数组中每两个相邻的长度为 len 的有序段两两归并
void MergePass(int A[], int n, int len, int tmp[]) {
    int i = 0;
    while (i + 2 * len <= n) {  // 还有完整的两段可以归并
        Merge(A, i, i + len - 1, i + 2 * len - 1, tmp);
        i += 2 * len;
    }
    if (i + len < n) {  // 剩下长度不超过 2*len 的两段，也要归并
        Merge(A, i, i + len - 1, n - 1, tmp);
    }
    // 若只剩一段（i + len >= n），说明本趟它不需要归并，直接留到下一趟
}

// 自底向上的二路归并排序：len = 1, 2, 4, ... 逐步把有序段长度翻倍
// 生成初始归并段时就用它做内存中的内部排序；时间复杂度 O(n·log n)，空间 O(n)
void MergeSort(int A[], int n) {
    int tmp[MAXSIZE];
    for (int len = 1; len < n; len *= 2) {
        MergePass(A, n, len, tmp);
    }
}

// ==================== k 路归并 ====================
// 把 k 个有序归并段合并到 out[] 中（朴素做法：每次在 k 个当前记录中比较出最小者）
// 每输出一个记录要做大约 k-1 次关键字比较，一共 n 个记录，所以比较次数量级为 (k-1)·n
// cmp 带回实际比较次数，便于和败者树的做法对比
void KMerge(const int seg[][MAXSIZE], const int len[], int k, int out[], int &cmp) {
    int pos[MAXK];  // 每个归并段下一个待取记录的下标
    int cur[MAXK];  // 每个归并段当前的记录（取完则为 INF）
    for (int i = 0; i < k; i++) {
        pos[i] = 0;
        cur[i] = (pos[i] < len[i]) ? seg[i][pos[i]++] : INF;
    }

    cmp = 0;
    int cnt = 0;
    while (true) {
        int best = -1;  // 当前最小记录所在的归并段
        for (int i = 0; i < k; i++) {
            if (cur[i] == INF) {
                continue;  // 该段已经取完，不参与比较
            }
            cmp++;
            if (best == -1 || cur[i] < cur[best]) {
                best = i;
            }
        }
        if (best == -1) {
            break;  // 所有段都取完，归并结束
        }
        out[cnt++] = cur[best];  // 输出最小记录
        cur[best] = (pos[best] < len[best]) ? seg[best][pos[best]++] : INF;
    }
}

// 计算 k 路归并的趟数：S = ⌈log_k m⌉
// 每做一趟，m 个归并段就变成 ⌈m/k⌉ 个
int MergePasses(int m, int k) {
    int passes = 0;
    while (m > 1) {
        m = (m + k - 1) / k;
        passes++;
    }
    return passes;
}

// 打印数组
void PrintArray(const int A[], int n, const char *title) {
    printf("%s", title);
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main() {
    // 1. 二路归并：把两个有序段合并成一个有序段
    int A[] = {1, 4, 7, 2, 3, 9};
    int tmp[MAXSIZE];
    printf("=== 二路归并 ===\n归并前：1 4 7 | 2 3 9\n");
    Merge(A, 0, 2, 5, tmp);
    PrintArray(A, 6, "归并后：");

    // 2. 自底向上的二路归并排序：逐趟放大有序段的长度
    //    （生成初始归并段时就是用它做内存中的内部排序）
    int B[] = {49, 38, 65, 97, 76, 13, 27, 50};
    int n = sizeof(B) / sizeof(B[0]);
    printf("\n=== 二路归并排序（自底向上）===\n");
    PrintArray(B, n, "排序前：");
    for (int len = 1; len < n; len *= 2) {
        MergePass(B, n, len, tmp);
        printf("  归并长度为 %d 的相邻两段后：", len);
        for (int i = 0; i < n; i++) {
            printf("%d ", B[i]);
        }
        printf("\n");
    }
    PrintArray(B, n, "排序后：");

    // 3. 3 路归并：把 3 个有序归并段合并成一个
    printf("\n=== 3 路归并 ===\n");
    int seg[3][MAXSIZE] = {{5, 12, 30, 40}, {8, 15, 22}, {9, 10, 25, 33, 45}};
    int lens[3] = {4, 3, 5};
    for (int i = 0; i < 3; i++) {
        printf("  第 %d 个归并段：", i + 1);
        for (int j = 0; j < lens[i]; j++) {
            printf("%d ", seg[i][j]);
        }
        printf("\n");
    }
    int out[MAXSIZE], cmp;
    KMerge(seg, lens, 3, out, cmp);
    PrintArray(out, 12, "  3 路归并结果：");
    printf("  朴素做法共比较 %d 次，输出 12 个记录，平均每个记录比较 %.1f 次（理论约 k-1 = 2 次）\n",
           cmp, (double)cmp / 12);

    // 4. 归并路数 k 对归并趟数与 I/O 次数的影响
    printf("\n=== 归并路数 k 的影响（m = 16 个初始归并段，n = 100000 条记录）===\n");
    int m = 16, total = 100000;
    for (int k = 2; k <= 16; k *= 2) {
        int s = MergePasses(m, k);
        printf("  k = %-2d：归并趟数 S = ⌈log_%d %d⌉ = %d，多路归并阶段的总读写次数 = 2n·S = %d 次\n",
               k, k, m, s, 2 * total * s);
    }

    printf("\n结论：k 越大归并趟数越少（k = 16 时一趟就能归并完），I/O 次数也越少；\n");
    printf("      但每趟从 k 个记录中挑出最小的比较次数会从 k-1 变多，工程上要配合败者树来降低比较次数；\n");
    printf("      还要注意：初始归并段的生成阶段本身也要读写各一遍（2n 次），不能只盯着归并趟数。\n");
    return 0;
}
