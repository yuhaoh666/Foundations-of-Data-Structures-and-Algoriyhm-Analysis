// 败者树：k 路平衡归并中用来快速选出 k 个当前记录中最小者的数据结构
// 它是一棵完全二叉树：叶结点对应 k 个归并段的当前记录，
// 内部结点记录“这一场比赛的败者”，最后的冠军被送到树根之上的 ls[0] 中
// 关键结论：
//   树高为 ⌈log2 k⌉，所以每输出一个记录只需约 ⌈log2 k⌉ 次比较就能重新选出最小者；
//   而“朴素的 k 路归并”每次都要线性比较 k-1 次，k 越大败者树的优势越明显
// 约定（教材经典写法）：
//   b[0..k-1] 是 k 个归并段当前的记录，b[k] 是值为 -∞ 的哨兵记录；
//   ls[1..k-1] 是内部结点（ls[t] 存放结点 t 这场比赛中的败者段号），ls[0] 存放冠军段号
#include <stdio.h>
#define MAXK 20           // 归并路数上限
#define MAXLEN 30         // 每个归并段的记录数上限
#define MAXOUT 200        // 归并输出结果的上限
#define INF 32767         // 无穷大：表示该归并段的记录已经取完
#define NEG_INF (-32767)  // 哨兵记录的“无穷小”值

int cmpCount;  // 比较次数统计（既用于败者树，也用于对照的朴素做法）

// 调整败者树：第 s 个归并段的当前记录发生变化后，从叶结点往上重新比赛
// 每个结点留下败者，胜者（关键字较小者）继续向上，最后把冠军记入 ls[0]
void Adjust(int ls[], const int b[], int k, int s) {
    int t = (s + k) / 2;  // 叶结点 s 对应的父结点
    while (t > 0) {
        cmpCount++;
        if (b[s] > b[ls[t]]) {  // b[s] 更大，说明 s 是这一场的败者
            int temp = s;
            s = ls[t];  // 原来的胜者继续向上
            ls[t] = temp;
        }
        t /= 2;  // 继续向上比赛
    }
    ls[0] = s;  // 循环结束时 s 就是冠军所在的归并段
}

// 建败者树：所有结点的败者先记为哨兵 k（它比谁都“小”，一定赢），再自下而上逐棵调整
void CreateLoserTree(int ls[], int b[], int k) {
    b[k] = NEG_INF;  // 哨兵记录
    for (int i = 0; i < k; i++) {
        ls[i] = k;
    }
    for (int i = k - 1; i >= 0; i--) {
        Adjust(ls, b, k, i);
    }
}

// 用败者树完成 k 路归并：把 data[] 中的 k 个有序归并段合并到 out[]，返回输出的记录个数
int KMergeByLoserTree(const int data[][MAXLEN], const int len[], int k, int out[]) {
    int b[MAXK + 1];  // 各归并段的当前记录，b[k] 为哨兵
    int ls[MAXK];     // 败者树
    int pos[MAXK];    // 每个归并段下一个待取记录的下标

    for (int i = 0; i < k; i++) {  // 先把每个归并段的第一条记录装进 b
        pos[i] = 0;
        b[i] = (pos[i] < len[i]) ? data[i][pos[i]++] : INF;
    }
    cmpCount = 0;
    CreateLoserTree(ls, b, k);  // 建树：这一步可以一次比较出 k 个记录中的最小者

    int cnt = 0;
    while (b[ls[0]] != INF) {  // 冠军记录不是“无穷大”，说明还有记录没输出
        int w = ls[0];         // 冠军所在的归并段
        out[cnt++] = b[w];     // 输出它的当前记录
        b[w] = (pos[w] < len[w]) ? data[w][pos[w]++] : INF;  // 该段递补下一条记录
        Adjust(ls, b, k, w);   // 只需要沿着一条路径重新比赛，共约 ⌈log2 k⌉ 次比较
    }
    return cnt;
}

// 对照实验：朴素做法——每次线性扫描 k 个当前记录选出最小者，需要约 k-1 次比较
int KMergeByScan(const int data[][MAXLEN], const int len[], int k, int out[]) {
    int b[MAXK];
    int pos[MAXK];
    for (int i = 0; i < k; i++) {
        pos[i] = 0;
        b[i] = (pos[i] < len[i]) ? data[i][pos[i]++] : INF;
    }
    cmpCount = 0;
    int cnt = 0;
    while (true) {
        int best = -1;
        for (int i = 0; i < k; i++) {
            if (b[i] == INF) {
                continue;  // 该段已取完
            }
            cmpCount++;
            if (best == -1 || b[i] < b[best]) {
                best = i;
            }
        }
        if (best == -1) {
            break;
        }
        out[cnt++] = b[best];
        b[best] = (pos[best] < len[best]) ? data[best][pos[best]++] : INF;
    }
    return cnt;
}

// 打印数组
void PrintArray(const int a[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

// 演示：只建树不归并，把败者树的内部结构、冠军以及建树所需的比较次数打印出来
void ShowLoserTree(const int data[][MAXLEN], const int len[], int k) {
    int b[MAXK + 1], ls[MAXK];
    for (int i = 0; i < k; i++) {
        b[i] = len[i] > 0 ? data[i][0] : INF;
    }
    cmpCount = 0;
    CreateLoserTree(ls, b, k);

    printf("  各归并段的当前记录：");
    for (int i = 0; i < k; i++) {
        printf("第%d段=%d ", i + 1, b[i]);
    }
    printf("\n  内部结点存放的败者：");
    for (int t = 1; t < k; t++) {
        printf("ls[%d]=第%d段", t, ls[t] + 1);
        if (t < k - 1) {
            printf("，");
        }
    }
    printf("\n  冠军 ls[0] = 第%d段（当前记录 %d），建树一共比较 %d 次\n", ls[0] + 1, b[ls[0]],
           cmpCount);
}

int main() {
    const int k = 5;  // 归并路数
    int data[MAXK][MAXLEN] = {{3, 17, 25, 41},   // 第 1 个归并段
                              {1, 9, 20, 33},    // 第 2 个归并段
                              {5, 13, 30},       // 第 3 个归并段
                              {8, 11, 29, 45},   // 第 4 个归并段
                              {2, 7, 22, 50}};   // 第 5 个归并段
    int len[MAXK] = {4, 4, 3, 4, 4};
    int total = 0;
    for (int i = 0; i < k; i++) {
        total += len[i];
    }

    printf("=== %d 路归并的各个归并段（共 %d 条记录）===\n", k, total);
    for (int i = 0; i < k; i++) {
        printf("  第 %d 个归并段：", i + 1);
        for (int j = 0; j < len[i]; j++) {
            printf("%d ", data[i][j]);
        }
        printf("\n");
    }

    // 1. 先观察败者树的内部结构
    int height = 0, p = 1;
    while (p < k) {  // 计算 ⌈log2 k⌉
        p *= 2;
        height++;
    }
    printf("\n=== 建败者树（树高 ⌈log2 %d⌉ = %d）===\n", k, height);
    ShowLoserTree(data, len, k);

    // 2. 用败者树归并
    int out1[MAXOUT], cnt1;
    cnt1 = KMergeByLoserTree(data, len, k, out1);
    int treeCmp = cmpCount;
    printf("\n=== 用败者树归并 ===\n  归并结果：");
    PrintArray(out1, cnt1);
    printf("  共比较 %d 次，平均每条记录 %.2f 次（理论上限约 ⌈log2 k⌉ = %d 次）\n", treeCmp,
           (double)treeCmp / cnt1, height);

    // 3. 对照：朴素扫描归并
    int out2[MAXOUT], cnt2;
    cnt2 = KMergeByScan(data, len, k, out2);
    int scanCmp = cmpCount;
    printf("\n=== 对照：朴素扫描归并 ===\n  归并结果：");
    PrintArray(out2, cnt2);
    printf("  共比较 %d 次，平均每条记录 %.2f 次（理论约 k-1 = %d 次）\n", scanCmp,
           (double)scanCmp / cnt2, k - 1);

    bool same = (cnt1 == cnt2);
    for (int i = 0; same && i < cnt1; i++) {
        if (out1[i] != out2[i]) {
            same = false;
        }
    }
    printf("  两种做法的结果是否一致：%s；败者树少比较 %d 次\n", same ? "是" : "否",
           scanCmp - treeCmp);

    // 4. k 与每次选择所需比较次数的关系
    printf("\n=== 每输出一条记录需要的比较次数（理论值）===\n");
    for (int kk = 2; kk <= 16; kk *= 2) {
        int h = 0, q = 1;
        while (q < kk) {
            q *= 2;
            h++;
        }
        printf("  k = %-2d：败者树约 %d 次，朴素扫描 %d 次\n", kk, h, kk - 1);
    }

    printf("\n结论：败者树把“从 k 个记录中选最小”的比较次数由 O(k) 降到 O(log2 k)，\n");
    printf("      代价是每输出一条记录要沿着一条路径自底向上调整，需要额外 O(k) 的空间；\n");
    printf("      它是 k 路平衡归并（外部排序）中的经典优化手段。\n");
    return 0;
}
