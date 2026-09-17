// 树的顺序存储——双亲表示法：每个结点记录自己的数据和双亲在数组中的下标
// 特点：找双亲只需 O(1)，找孩子却要遍历整个数组；很适合“找双亲/并查集”这类场景
#include <stdio.h>
#define MaxSize 100

typedef struct {
    int data;    // 结点数据
    int parent;  // 双亲结点的下标，-1 表示该结点没有双亲（即根结点）
} TreeNode;

typedef struct {
    TreeNode nodes[MaxSize];  // 存放结点的数组
    int n;                    // 结点个数
} Tree;

// 初始化：结点个数置 0，并把所有结点的双亲都置为 -1
void InitTree(Tree *tree) {
    tree->n = 0;
    for (int i = 0; i < MaxSize; i++) {
        tree->nodes[i].data = 0;
        tree->nodes[i].parent = -1;
    }
}

// 添加结点：data 为数据，parent 为双亲的下标（-1 表示根结点）
// 返回新结点的下标，失败返回 -1
int AddNode(Tree *tree, int data, int parent) {
    if (tree->n >= MaxSize) {
        printf("树已满，无法添加更多结点。\n");
        return -1;
    }
    if (parent < -1 || parent >= tree->n) {  // 双亲下标必须落在已有结点范围内
        printf("双亲下标 %d 不合法，添加失败。\n", parent);
        return -1;
    }
    int index = tree->n;
    tree->nodes[index].data = data;
    tree->nodes[index].parent = parent;
    tree->n++;
    return index;
}

// 找下标为 p 的结点的双亲下标，-1 表示它是根结点
int FindParent(Tree *tree, int p) {
    if (p < 0 || p >= tree->n) {
        return -1;
    }
    return tree->nodes[p].parent;
}

// 找下标为 p 的结点的所有孩子（输出它们的数据）
void FindChildren(Tree *tree, int p) {
    if (p < 0 || p >= tree->n) {
        printf("下标 %d 不合法\n", p);
        return;
    }
    printf("结点 %d（下标 %d）的孩子：", tree->nodes[p].data, p);
    int count = 0;
    for (int i = 0; i < tree->n; i++) {
        if (tree->nodes[i].parent == p) {  // 双亲下标等于 p 的结点就是 p 的孩子
            printf("%d ", tree->nodes[i].data);
            count++;
        }
    }
    if (count == 0) {
        printf("无");
    }
    printf("\n");
}

// 按数据值查找结点下标，未找到返回 -1
int FindIndex(Tree *tree, int data) {
    for (int i = 0; i < tree->n; i++) {
        if (tree->nodes[i].data == data) {
            return i;
        }
    }
    return -1;
}

// 求树的深度：每个结点都沿双亲指针上溯到根，取最大的路径长度
int TreeDepth(Tree *tree) {
    int maxDepth = 0;
    for (int i = 0; i < tree->n; i++) {
        int depth = 0;
        for (int p = i; p != -1; p = tree->nodes[p].parent) {
            depth++;
        }
        if (depth > maxDepth) {
            maxDepth = depth;
        }
    }
    return maxDepth;
}

// 打印整棵树（下标、数据、双亲）
void PrintTree(Tree *tree) {
    printf("树的顺序存储（双亲表示法）：\n");
    printf("  下标  数据  双亲\n");
    for (int i = 0; i < tree->n; i++) {
        printf("  %-5d %-5d %-5d%s\n", i, tree->nodes[i].data, tree->nodes[i].parent,
               tree->nodes[i].parent == -1 ? "   <-- 根结点" : "");
    }
}

int main() {
    Tree tree;
    InitTree(&tree);

    // 依次添加结点，第二个参数是“双亲的下标”
    AddNode(&tree, 1, -1);  // 下标 0：根结点 1
    AddNode(&tree, 2, 0);   // 下标 1：结点 2，双亲是下标 0（结点 1）
    AddNode(&tree, 3, 0);   // 下标 2：结点 3，双亲是下标 0（结点 1）
    AddNode(&tree, 4, 1);   // 下标 3：结点 4，双亲是下标 1（结点 2）
    AddNode(&tree, 5, 1);   // 下标 4：结点 5，双亲是下标 1（结点 2）
    AddNode(&tree, 6, 9);   // 双亲下标不合法，应被拦截

    PrintTree(&tree);
    printf("树的深度：%d\n", TreeDepth(&tree));

    // 找结点 3 的双亲：先按数据找到下标，再查双亲下标
    int idx = FindIndex(&tree, 3);
    int parent = FindParent(&tree, idx);
    if (parent == -1) {
        printf("结点 3 是根结点，没有双亲\n");
    } else {
        printf("结点 3（下标 %d）的双亲是 结点 %d（下标 %d）\n",
               idx, tree.nodes[parent].data, parent);
    }

    // 找孩子：结点 2 有两个孩子，结点 5 是叶子
    FindChildren(&tree, FindIndex(&tree, 2));
    FindChildren(&tree, FindIndex(&tree, 5));
    return 0;
}
