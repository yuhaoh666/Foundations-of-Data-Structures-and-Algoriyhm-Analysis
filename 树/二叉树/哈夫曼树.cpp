// 哈夫曼树（最优二叉树）：带权路径长度 WPL 最小的二叉树
// 构造思路：每次从森林中选出权值最小的两棵树合并，重复 n-1 次
// 采用顺序存储：数组下标 0 不用，1 ~ n 为叶子结点，n+1 ~ 2n-1 为内部结点
#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int weight;               // 权值
    int parent, left, right;  // 双亲、左孩子、右孩子在数组中的下标
} HTNode, HuffmanTree[MAXSIZE + 1];

// 从 1 ~ n 中选出两个双亲为 0（尚未合并）且权值最小的结点，下标存入 s1、s2
void Select(HuffmanTree HT, int n, int &s1, int &s2) {
    s1 = s2 = 0;
    for (int i = 1; i <= n; i++) {
        if (HT[i].parent != 0) {  // 已经被合并过的结点不再参与选择
            continue;
        }
        if (s1 == 0 || HT[i].weight < HT[s1].weight) {
            s2 = s1;  // 原来的最小者退居次小
            s1 = i;
        } else if (s2 == 0 || HT[i].weight < HT[s2].weight) {
            s2 = i;
        }
    }
}

// 构造哈夫曼树：weight[0..n-1] 存放 n 个叶子结点的权值
void CreateHuffmanTree(HuffmanTree HT, const int weight[], int n) {
    if (n <= 1) {
        return;
    }
    int m = 2 * n - 1;               // 哈夫曼树共有 2n-1 个结点
    for (int i = 1; i <= m; i++) {   // 初始化所有结点
        HT[i].weight = 0;
        HT[i].parent = 0;
        HT[i].left = 0;
        HT[i].right = 0;
    }
    for (int i = 1; i <= n; i++) {   // 前 n 个位置放叶子结点
        HT[i].weight = weight[i - 1];
    }
    for (int i = n + 1; i <= m; i++) {  // 共合并 n-1 次
        int s1, s2;
        Select(HT, i - 1, s1, s2);      // 在前 i-1 个结点中找两棵权值最小的树
        HT[s1].parent = HT[s2].parent = i;  // 它们的双亲都指向新结点 i
        HT[i].left = s1;
        HT[i].right = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;  // 新结点权值为两者之和
    }
}

// 计算带权路径长度 WPL = Σ(叶子权值 × 该叶子的层数)
int HuffmanWPL(HuffmanTree HT, int n) {
    int wpl = 0;
    for (int i = 1; i <= n; i++) {
        int depth = 0;
        for (int p = HT[i].parent; p != 0; p = HT[p].parent) {  // 从叶子沿双亲走到根
            depth++;
        }
        wpl += HT[i].weight * depth;
    }
    return wpl;
}

// 求每个叶子结点的哈夫曼编码（从叶子往根逆向求，再正序输出）
void HuffmanCodes(HuffmanTree HT, int n) {
    char code[MAXSIZE];
    printf("各叶子结点的哈夫曼编码（左分支记 0，右分支记 1）：\n");
    for (int i = 1; i <= n; i++) {
        int start = n;   // 编码从 code[n-1] 开始向前存放
        int c = i;
        for (int p = HT[c].parent; p != 0; p = HT[p].parent) {
            code[--start] = (HT[p].left == c) ? '0' : '1';
            c = p;
        }
        printf("  叶子权值 %-4d 编码 ", HT[i].weight);
        for (int j = start; j < n; j++) {
            putchar(code[j]);
        }
        printf("\n");
    }
}

// 打印哈夫曼树的顺序存储结构（下标、权值、双亲、左右孩子）
void PrintTree(HuffmanTree HT, int n) {
    int m = 2 * n - 1;
    printf("哈夫曼树的顺序存储结构：\n");
    printf("  下标  权值  双亲  左孩子  右孩子\n");
    for (int i = 1; i <= m; i++) {
        printf("  %-5d %-5d %-5d %-7d %-7d%s\n", i, HT[i].weight, HT[i].parent,
               HT[i].left, HT[i].right, i == n ? "   <-- 叶子结点到此为止" : "");
    }
}

int main() {
    int weight[] = {5, 9, 12, 13, 16, 45};
    int n = sizeof(weight) / sizeof(weight[0]);
    HuffmanTree HT;

    CreateHuffmanTree(HT, weight, n);
    printf("哈夫曼树构造完成，共 %d 个结点（%d 个叶子）\n", 2 * n - 1, n);

    PrintTree(HT, n);
    printf("带权路径长度 WPL = %d\n", HuffmanWPL(HT, n));
    HuffmanCodes(HT, n);
    return 0;
}
