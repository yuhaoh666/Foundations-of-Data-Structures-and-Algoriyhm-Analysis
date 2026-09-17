// 双亲表示法表示森林：森林是若干棵互不相交的树，
// 根结点的 parent 记为 -1，因此“森林”与“树”的存储结构完全相同，
// 区别只在于 parent == -1 的结点有几个（即森林中树的棵数）
#include <stdio.h>
#define MAX_SIZE 100

typedef struct {
    int data;    // 结点数据
    int parent;  // 双亲结点的下标，-1 表示该结点是某棵树的根
} TreeNode;

typedef struct {
    TreeNode nodes[MAX_SIZE];  // 存放结点的数组
    int n;                     // 结点总数
} Forest;

// 初始化森林
void InitForest(Forest *forest) {
    forest->n = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        forest->nodes[i].data = 0;
        forest->nodes[i].parent = -1;
    }
}

// 添加结点：parent 为 -1 表示新开一棵只有该根的树
// 返回新结点的下标，失败返回 -1
int AddNode(Forest *forest, int data, int parent) {
    if (forest->n >= MAX_SIZE) {
        printf("森林已满，无法添加更多结点。\n");
        return -1;
    }
    if (parent < -1 || parent >= forest->n) {  // 双亲下标必须在已有结点范围内
        printf("双亲下标 %d 不合法，添加失败。\n", parent);
        return -1;
    }
    int index = forest->n;
    forest->nodes[index].data = data;
    forest->nodes[index].parent = parent;
    forest->n++;
    return index;
}

// 找下标为 p 的结点的双亲下标，-1 表示它是某棵树的根
int FindParent(Forest *forest, int p) {
    if (p < 0 || p >= forest->n) {
        return -1;
    }
    return forest->nodes[p].parent;
}

// 找下标为 p 的结点的所有孩子
void FindChildren(Forest *forest, int p) {
    if (p < 0 || p >= forest->n) {
        printf("下标 %d 不合法\n", p);
        return;
    }
    printf("结点 %d（下标 %d）的孩子：", forest->nodes[p].data, p);
    int count = 0;
    for (int i = 0; i < forest->n; i++) {
        if (forest->nodes[i].parent == p) {
            printf("%d ", forest->nodes[i].data);
            count++;
        }
    }
    if (count == 0) {
        printf("无");
    }
    printf("\n");
}

// 森林中树的棵数 = 双亲为 -1 的结点个数
int CountRoots(Forest *forest) {
    int count = 0;
    for (int i = 0; i < forest->n; i++) {
        if (forest->nodes[i].parent == -1) {
            count++;
        }
    }
    return count;
}

// 求“以 p 为根的那棵子树”的深度（后序递归：先求各子树深度，再取最大者加 1）
int TreeDepth(Forest *forest, int p) {
    if (p < 0 || p >= forest->n) {
        return 0;
    }
    int maxChildDepth = 0;
    for (int i = 0; i < forest->n; i++) {
        if (forest->nodes[i].parent == p) {
            int d = TreeDepth(forest, i);
            if (d > maxChildDepth) {
                maxChildDepth = d;
            }
        }
    }
    return maxChildDepth + 1;
}

// 打印森林的存储结构，并给出每棵树的根及其深度
void PrintForest(Forest *forest) {
    printf("森林的顺序存储（双亲表示法）：\n");
    printf("  下标  数据  双亲\n");
    for (int i = 0; i < forest->n; i++) {
        printf("  %-5d %-5d %-5d%s\n", i, forest->nodes[i].data, forest->nodes[i].parent,
               forest->nodes[i].parent == -1 ? "   <-- 某棵树的根结点" : "");
    }
    printf("森林中共有 %d 棵树：\n", CountRoots(forest));
    for (int i = 0; i < forest->n; i++) {
        if (forest->nodes[i].parent == -1) {  // parent 为 -1 即为树根
            printf("  以结点 %d（下标 %d）为根的树，深度为 %d\n",
                   forest->nodes[i].data, i, TreeDepth(forest, i));
        }
    }
}

int main() {
    Forest forest;
    InitForest(&forest);

    // 第 1 棵树：1(2(4,5), 3)
    AddNode(&forest, 1, -1);  // 下标 0：第 1 棵树的根
    AddNode(&forest, 2, 0);   // 下标 1
    AddNode(&forest, 3, 0);   // 下标 2
    AddNode(&forest, 4, 1);   // 下标 3
    AddNode(&forest, 5, 1);   // 下标 4
    // 第 2 棵树：6(7)
    AddNode(&forest, 6, -1);  // 下标 5：第 2 棵树的根
    AddNode(&forest, 7, 5);   // 下标 6
    AddNode(&forest, 8, 99);  // 双亲下标不合法，应被拦截

    PrintForest(&forest);

    // 找结点 3（下标 2）的双亲
    int idx = 2;
    int parent = FindParent(&forest, idx);
    if (parent == -1) {
        printf("结点 3 是根结点，没有双亲\n");
    } else {
        printf("结点 3（下标 %d）的双亲是 结点 %d（下标 %d）\n",
               idx, forest.nodes[parent].data, parent);
    }

    // 找孩子
    FindChildren(&forest, 1);  // 结点 2 的孩子：4 5
    FindChildren(&forest, 5);  // 结点 6 的孩子：7
    FindChildren(&forest, 4);  // 结点 5 是叶子
    return 0;
}
