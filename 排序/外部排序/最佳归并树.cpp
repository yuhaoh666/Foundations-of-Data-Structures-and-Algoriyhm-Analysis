// 最佳归并树：对多个初始归并段做 k 路归并时，使读写记录总数最少的归并方案
// 它本质上是一棵 k 叉哈夫曼树：
//   叶子结点 = 各初始归并段（权值为归并段的长度即记录数），权值为 0 的叶子是“虚段”
//   带权路径长度 WPL = Σ(归并段长度 × 该叶结点的层数)，它正好等于归并过程中记录被读写的总次数
// 关键结论：k 叉哈夫曼树要求 (叶子数 - 1) % (k - 1) == 0，
//           不满足时必须先补若干个权值为 0 的虚段，否则最后一次归并不会刚好剩 k 个段
#include <stdio.h>
#define MAXSIZE 100

// 用“孩子兄弟链表 + 双亲”的方式描述 k 叉树，便于计算层数、打印结构
typedef struct {
    int weight;       // 权值（归并段长度；虚段为 0）
    int parent;       // 双亲结点下标
    int firstChild;   // 第一个孩子结点下标
    int nextSibling;  // 下一个兄弟结点下标
} HTNode, KTree[MAXSIZE + 1];  // 下标 0 不用

// 从 1 ~ n 中选出 k 个“没有双亲”且权值最小的结点，下标按权值升序存入 s[]
// 做法：用插入排序的思想维护一个长度为 k 的有序表，只保留最小的 k 个
void SelectK(KTree HT, int n, int k, int s[]) {
    for (int t = 0; t < k; t++) {
        s[t] = 0;  // 0 表示该位置还没有元素
    }
    for (int i = 1; i <= n; i++) {
        if (HT[i].parent != 0) {
            continue;  // 已经被合并过的结点不参与选择
        }
        if (s[k - 1] != 0 && HT[i].weight >= HT[s[k - 1]].weight) {
            continue;  // 比当前第 k 小的还大，直接跳过
        }
        int pos = k - 1;  // 把 i 插入到 s[] 中合适的位置（保持升序，空位在后面）
        while (pos > 0 && (s[pos - 1] == 0 || HT[i].weight < HT[s[pos - 1]].weight)) {
            s[pos] = s[pos - 1];
            pos--;
        }
        s[pos] = i;
    }
}

// 构造 k 叉最佳归并树
// weight[]：各初始归并段的长度；n：归并段个数；k：归并路数
// fakeCount 返回补的虚段个数，leafCount 返回叶结点总数（含虚段）
// 返回树中结点总数 m（也是整棵树的根结点下标），参数非法时返回 0
int CreateBestMergeTree(KTree HT, const int weight[], int n, int k, int &fakeCount, int &leafCount) {
    fakeCount = 0;
    leafCount = n;
    if (n <= 1 || k < 2) {
        printf("参数非法：至少需要 2 个归并段且归并路数 k >= 2\n");
        return 0;
    }
    // 1. 计算需要补充的虚段个数
    int fake = 0;
    while ((n + fake - 1) % (k - 1) != 0) {
        fake++;
    }
    fakeCount = fake;
    leafCount = n + fake;                   // 叶结点总数（含虚段）
    int m = (k * leafCount - 1) / (k - 1);  // 满 k 叉树的结点总数

    // 2. 初始化所有结点
    for (int i = 1; i <= m; i++) {
        HT[i].weight = 0;
        HT[i].parent = 0;
        HT[i].firstChild = 0;
        HT[i].nextSibling = 0;
    }
    for (int i = 1; i <= n; i++) {
        HT[i].weight = weight[i - 1];  // 前 n 个结点是真正的归并段
    }
    for (int i = n + 1; i <= leafCount; i++) {
        HT[i].weight = 0;              // 后面的结点是虚段（长度为 0）
    }

    // 3. 反复合并：每次选出 k 个权值最小的树合并成一棵新树
    int s[MAXSIZE];
    for (int i = leafCount + 1; i <= m; i++) {
        SelectK(HT, i - 1, k, s);  // 在前 i-1 个结点中选 k 个最小的根
        int sum = 0;
        HT[i].firstChild = 0;
        for (int t = 0; t < k; t++) {
            if (s[t] == 0) {  // 万一个数不足 k（正常不会发生），安全退出
                break;
            }
            HT[s[t]].parent = i;   // 被选中的树挂到新结点下
            sum += HT[s[t]].weight;
            if (HT[i].firstChild == 0) {  // 作为第一个孩子
                HT[i].firstChild = s[t];
            } else {                      // 否则接到孩子链的末尾
                int c = HT[i].firstChild;
                while (HT[c].nextSibling != 0) {
                    c = HT[c].nextSibling;
                }
                HT[c].nextSibling = s[t];
            }
        }
        HT[i].weight = sum;  // 新树的权值等于这些子树权值之和
    }
    return m;  // 最后一次合并产生的新结点就是整棵树的根
}

// 计算带权路径长度 WPL：叶结点权值 × 该叶结点的层数（根为第 1 层）
int MergeWPL(KTree HT, int leafCount) {
    int wpl = 0;
    for (int i = 1; i <= leafCount; i++) {
        int depth = 0;
        for (int p = HT[i].parent; p != 0; p = HT[p].parent) {  // 从叶结点一路走到根
            depth++;
        }
        wpl += HT[i].weight * depth;
    }
    return wpl;
}

// 求树的高度（层数）
int TreeHeight(KTree HT, int root) {
    if (root == 0) {
        return 0;
    }
    int maxChildDepth = 0;
    for (int c = HT[root].firstChild; c != 0; c = HT[c].nextSibling) {
        int d = TreeHeight(HT, c);
        if (d > maxChildDepth) {
            maxChildDepth = d;
        }
    }
    return maxChildDepth + 1;
}

// 用括号表示法打印最佳归并树（输出的是权值）
void PrintTree(KTree HT, int root) {
    if (root == 0) {
        return;
    }
    printf("%d", HT[root].weight);
    if (HT[root].firstChild == 0) {
        return;
    }
    printf("(");
    for (int c = HT[root].firstChild; c != 0; c = HT[c].nextSibling) {
        PrintTree(HT, c);
        if (HT[c].nextSibling != 0) {
            printf(",");
        }
    }
    printf(")");
}

// 对一组归并段做测试：构造最佳归并树并输出关键信息
void TestBestMergeTree(const int weight[], int n, int k, const char *title) {
    KTree HT;
    int fakeCount = 0, leafCount = 0;
    printf("=== %s ===\n", title);
    printf("归并段个数 n = %d，归并路数 k = %d\n", n, k);
    printf("(n-1) %% (k-1) = %d，需要补充 %d 个长度为 0 的虚段\n",
           (n - 1) % (k - 1), (n - 1) % (k - 1) == 0 ? 0 : (k - 1) - (n - 1) % (k - 1));

    int m = CreateBestMergeTree(HT, weight, n, k, fakeCount, leafCount);
    if (m == 0) {
        printf("\n");
        return;
    }
    printf("叶结点总数（含虚段）= %d，树中结点总数 = %d\n", leafCount, m);
    printf("最佳归并树（括号表示法，数字为归并段长度）：\n  ");
    PrintTree(HT, m);
    printf("\n");
    printf("归并趟数 = 树高 - 1 = %d\n", TreeHeight(HT, m) - 1);
    printf("读写记录总数（WPL = Σ 叶权 × 层数）= %d\n\n", MergeWPL(HT, leafCount));
}

int main() {
    // 例 1：(n-1) 能被 (k-1) 整除，不需要虚段
    int w1[] = {9, 30, 12, 18, 3, 17, 2, 6, 24};
    TestBestMergeTree(w1, sizeof(w1) / sizeof(w1[0]), 3, "9 个归并段、3 路归并");

    // 例 2：需要补 1 个虚段；(n-1) % (k-1) = 1
    int w2[] = {9, 30, 12, 18, 3, 17, 2, 6};
    TestBestMergeTree(w2, sizeof(w2) / sizeof(w2[0]), 3, "8 个归并段、3 路归并（需补虚段）");

    // 例 3：2 路归并——最佳归并树退化为哈夫曼树，不需要虚段
    int w3[] = {5, 9, 12, 13, 16, 45};
    TestBestMergeTree(w3, sizeof(w3) / sizeof(w3[0]), 2, "6 个归并段、2 路归并（即哈夫曼树）");

    printf("结论：只有把长度最小的归并段放在最底层（先归并），总读写次数才最少；\n");
    printf("      当 (n-1) 不能被 (k-1) 整除时，补上长度为 0 的虚段即可化为标准 k 叉哈夫曼树。\n");
    return 0;
}
