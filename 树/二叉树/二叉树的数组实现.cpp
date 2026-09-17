// 二叉树的顺序存储：用一维数组按“完全二叉树”的层序编号存放结点
// 结点下标为 i（从 0 开始）时：左孩子为 2i+1，右孩子为 2i+2，双亲为 (i-1)/2
// 注意：顺序存储最适合完全二叉树；一般二叉树会浪费大量空间，所以这里用 isEmpty 标记空结点
#include <stdio.h>
#define MAX 100

typedef struct TreeNode {
    int data;      // 结点数据
    bool isEmpty;  // 该位置是否为空结点
} TreeNode;

// 初始化：把所有位置都标记为空
void InitTree(TreeNode tree[]) {
    for (int i = 0; i < MAX; i++) {
        tree[i].data = 0;
        tree[i].isEmpty = true;
    }
}

// 判断下标 i 是否是一个有效的（非空）结点
bool IsEmptyNode(TreeNode tree[], int i) {
    return i < 0 || i >= MAX || tree[i].isEmpty;
}

// 按完全二叉树的编号规则给出左孩子、右孩子、双亲的下标
int LeftChild(int i) { return 2 * i + 1; }
int RightChild(int i) { return 2 * i + 2; }
int Parent(int i) { return (i - 1) / 2; }

// 给下标 i 的位置写入结点值
bool SetNode(TreeNode tree[], int i, int value) {
    if (i < 0 || i >= MAX) {
        return false;
    }
    tree[i].data = value;
    tree[i].isEmpty = false;
    return true;
}

// 打印某个结点的双亲
void PrintParent(TreeNode tree[], int i) {
    if (IsEmptyNode(tree, i)) {
        printf("下标 %d 处没有结点\n", i);
        return;
    }
    if (i == 0) {
        printf("结点 %d 是根结点，没有双亲\n", tree[i].data);
        return;
    }
    int p = Parent(i);
    if (IsEmptyNode(tree, p)) {
        printf("结点 %d 的双亲位置（下标 %d）为空\n", tree[i].data, p);
    } else {
        printf("结点 %d 的双亲是 %d\n", tree[i].data, tree[p].data);
    }
}

// 打印某个结点的左右孩子
void PrintChildren(TreeNode tree[], int i) {
    if (IsEmptyNode(tree, i)) {
        printf("下标 %d 处没有结点\n", i);
        return;
    }
    printf("结点 %d 的左孩子：", tree[i].data);
    int l = LeftChild(i);
    if (IsEmptyNode(tree, l)) {
        printf("无");
    } else {
        printf("%d（下标 %d）", tree[l].data, l);
    }
    printf("，右孩子：");
    int r = RightChild(i);
    if (IsEmptyNode(tree, r)) {
        printf("无");
    } else {
        printf("%d（下标 %d）", tree[r].data, r);
    }
    printf("\n");
}

// 层序遍历：数组本身就是按层序编号存放的，所以按下标顺序输出非空结点即可
void LevelOrder(TreeNode tree[]) {
    printf("层序遍历：");
    for (int i = 0; i < MAX; i++) {
        if (!tree[i].isEmpty) {
            printf("%d ", tree[i].data);
        }
    }
    printf("\n");
}

// 求树的深度：找到下标最大的非空结点，再沿双亲指针一路回到根
int TreeDepth(TreeNode tree[]) {
    int last = -1;
    for (int i = 0; i < MAX; i++) {
        if (!tree[i].isEmpty) {
            last = i;
        }
    }
    if (last == -1) {  // 空树
        return 0;
    }
    int depth = 1;
    while (last > 0) {
        last = Parent(last);
        depth++;
    }
    return depth;
}

int main() {
    TreeNode tree[MAX];
    InitTree(tree);

    // 手工按完全二叉树编号建一棵树：
    //         1(0)
    //        /    \
    //     2(1)    3(2)
    //    /   \
    //  4(3)   5(4)
    SetNode(tree, 0, 1);
    SetNode(tree, 1, 2);
    SetNode(tree, 2, 3);
    SetNode(tree, 3, 4);
    SetNode(tree, 4, 5);

    LevelOrder(tree);
    printf("树的深度：%d\n", TreeDepth(tree));

    PrintParent(tree, 0);  // 根结点
    PrintParent(tree, 4);  // 结点 5 的双亲是 2
    PrintChildren(tree, 1);  // 结点 2 的孩子是 4、5
    PrintChildren(tree, 2);  // 结点 3 没有孩子

    printf("下标 5 处是否为空结点：%s\n", IsEmptyNode(tree, 5) ? "是" : "否");
    return 0;
}
