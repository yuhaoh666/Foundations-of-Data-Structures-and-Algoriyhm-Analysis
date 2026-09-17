// 树的“顺序存储 + 链式存储”结合——孩子表示法
// 结点本身用数组顺序存放，每个结点的孩子则用一条单链表串起来（链表中存的是孩子在数组中的下标）
// 特点：找孩子很方便；找双亲要么遍历整条孩子链表，要么像这里一样额外存一个双亲下标（兼顾两者）
#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100

typedef struct CTNode {        // 孩子结点（链表结点）
    int childIndex;            // 孩子在数组中的下标
    struct CTNode *nextChild;  // 指向下一个孩子
} CTNode;

typedef struct {
    int data;            // 结点数据
    int parent;          // 双亲结点在数组中的下标，-1 表示根结点
    CTNode *firstChild;  // 指向第一个孩子的指针
} CTBox;

typedef struct {
    CTBox nodes[MaxSize];  // 存放结点的数组
    int n;                 // 结点个数
    int root;              // 根结点的下标，-1 表示空树
} CTree;

// 初始化
void InitTree(CTree *tree) {
    tree->n = 0;
    tree->root = -1;
    for (int i = 0; i < MaxSize; i++) {
        tree->nodes[i].data = 0;
        tree->nodes[i].parent = -1;
        tree->nodes[i].firstChild = NULL;
    }
}

// 添加结点：data 为数据，parent 为双亲的下标（-1 表示作为根结点）
// 返回新结点的下标，失败返回 -1
int AddNode(CTree *tree, int data, int parent) {
    if (tree->n >= MaxSize) {
        printf("树已满，无法添加更多结点。\n");
        return -1;
    }
    if (parent < -1 || parent >= tree->n) {  // 双亲下标必须在已有结点范围内
        printf("双亲下标 %d 不合法，添加失败。\n", parent);
        return -1;
    }
    int index = tree->n;
    tree->nodes[index].data = data;
    tree->nodes[index].parent = parent;
    tree->nodes[index].firstChild = NULL;
    if (parent == -1) {
        tree->root = index;  // 记录根结点
    } else {
        // 新建一个孩子结点，头插到父结点的孩子链表中
        CTNode *newChild = (CTNode *)malloc(sizeof(CTNode));
        if (newChild == NULL) {
            printf("内存分配失败！\n");
            return -1;
        }
        newChild->childIndex = index;
        newChild->nextChild = tree->nodes[parent].firstChild;
        tree->nodes[parent].firstChild = newChild;
    }
    tree->n++;
    return index;
}

// 找下标为 index 的结点的双亲下标（利用双亲域，O(1)）
// 若结构中不设双亲域，则只能遍历所有结点的孩子链表来找，时间复杂度 O(n)
int FindParent(CTree *tree, int index) {
    if (index < 0 || index >= tree->n) {
        return -1;
    }
    return tree->nodes[index].parent;
}

// 输出下标为 index 的结点的所有孩子
void FindChildren(CTree *tree, int index) {
    if (index < 0 || index >= tree->n) {
        printf("下标 %d 不合法\n", index);
        return;
    }
    printf("结点 %d（下标 %d）的孩子：", tree->nodes[index].data, index);
    if (tree->nodes[index].firstChild == NULL) {
        printf("无");
    }
    for (CTNode *child = tree->nodes[index].firstChild; child != NULL;
         child = child->nextChild) {
        // 注意：链表中存的是下标，要输出数据得再到数组中查一次
        printf("%d（下标 %d） ", tree->nodes[child->childIndex].data, child->childIndex);
    }
    printf("\n");
}

// 按数据查找结点下标
int FindIndex(CTree *tree, int data) {
    for (int i = 0; i < tree->n; i++) {
        if (tree->nodes[i].data == data) {
            return i;
        }
    }
    return -1;
}

// 打印整棵树的结构信息
void PrintTree(CTree *tree) {
    printf("孩子表示法存储的树（根结点下标：%d）：\n", tree->root);
    for (int i = 0; i < tree->n; i++) {
        printf("  下标 %-3d 数据 %-3d 双亲 %-3d 孩子：", i, tree->nodes[i].data,
               tree->nodes[i].parent);
        if (tree->nodes[i].firstChild == NULL) {
            printf("无");
        }
        for (CTNode *child = tree->nodes[i].firstChild; child != NULL;
             child = child->nextChild) {
            printf("%d ", tree->nodes[child->childIndex].data);
        }
        printf("\n");
    }
}

// 销毁树：释放所有孩子链表结点
void DestroyTree(CTree *tree) {
    for (int i = 0; i < tree->n; i++) {
        CTNode *child = tree->nodes[i].firstChild;
        while (child != NULL) {
            CTNode *next = child->nextChild;
            free(child);
            child = next;
        }
        tree->nodes[i].firstChild = NULL;
    }
    tree->n = 0;
    tree->root = -1;
}

int main() {
    CTree tree;
    InitTree(&tree);

    // 依次添加结点，第二个参数是双亲的下标
    AddNode(&tree, 1, -1);  // 下标 0：根结点 1
    AddNode(&tree, 2, 0);   // 下标 1：结点 2，双亲是下标 0
    AddNode(&tree, 3, 0);   // 下标 2：结点 3，双亲是下标 0
    AddNode(&tree, 4, 1);   // 下标 3：结点 4，双亲是下标 1
    AddNode(&tree, 5, 1);   // 下标 4：结点 5，双亲是下标 1
    AddNode(&tree, 6, 9);   // 双亲下标不合法，应被拦截

    PrintTree(&tree);

    // 找结点 3 的双亲
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

    DestroyTree(&tree);
    printf("销毁后结点个数：%d\n", tree.n);
    return 0;
}
