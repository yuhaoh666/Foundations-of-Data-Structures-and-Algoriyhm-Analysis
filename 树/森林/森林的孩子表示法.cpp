// 孩子表示法表示森林：结点用数组顺序存放，每个结点的孩子用单链表串起来（链表存下标）
// 森林与树的区别：森林中可以有多个根结点（即没有双亲的结点）
#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100

typedef struct CTNode {        // 孩子结点（链表结点）
    int childIndex;            // 孩子在数组中的下标
    struct CTNode *nextChild;  // 指向下一个孩子
} CTNode;

typedef struct {
    int data;            // 结点数据
    int parent;          // 双亲的下标，-1 表示该结点是某棵树的根
    CTNode *firstChild;  // 指向第一个孩子
} CTBox;

typedef struct {
    CTBox nodes[MaxSize];  // 存放结点的数组
    int n;                 // 结点总数
} CForest;

// 初始化森林
void InitForest(CForest *forest) {
    forest->n = 0;
    for (int i = 0; i < MaxSize; i++) {
        forest->nodes[i].data = 0;
        forest->nodes[i].parent = -1;
        forest->nodes[i].firstChild = NULL;
    }
}

// 添加结点：parent 为 -1 表示新开一棵只有该根的树
// 返回新结点的下标，失败返回 -1
int AddNode(CForest *forest, int data, int parent) {
    if (forest->n >= MaxSize) {
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
    forest->nodes[index].firstChild = NULL;
    if (parent != -1) {  // 不是根结点，就把它挂到双亲的孩子链表中
        CTNode *newChild = (CTNode *)malloc(sizeof(CTNode));
        if (newChild == NULL) {
            printf("内存分配失败！\n");
            return -1;
        }
        newChild->childIndex = index;
        newChild->nextChild = forest->nodes[parent].firstChild;
        forest->nodes[parent].firstChild = newChild;
    }
    forest->n++;
    return index;
}

// 找下标为 index 的结点的双亲下标（-1 表示它是根结点）
int FindParent(CForest *forest, int index) {
    if (index < 0 || index >= forest->n) {
        return -1;
    }
    return forest->nodes[index].parent;
}

// 输出下标为 index 的结点的所有孩子
void FindChildren(CForest *forest, int index) {
    if (index < 0 || index >= forest->n) {
        printf("下标 %d 不合法\n", index);
        return;
    }
    printf("结点 %d（下标 %d）的孩子：", forest->nodes[index].data, index);
    if (forest->nodes[index].firstChild == NULL) {
        printf("无");
    }
    // 链表中存的是下标，输出时要再到数组里取出数据
    for (CTNode *child = forest->nodes[index].firstChild; child != NULL;
         child = child->nextChild) {
        printf("%d（下标 %d） ", forest->nodes[child->childIndex].data, child->childIndex);
    }
    printf("\n");
}

// 按数据查找结点下标，未找到返回 -1
int FindIndex(CForest *forest, int data) {
    for (int i = 0; i < forest->n; i++) {
        if (forest->nodes[i].data == data) {
            return i;
        }
    }
    return -1;
}

// 森林中树的棵数 = 双亲为 -1 的结点个数
int CountRoots(CForest *forest) {
    int count = 0;
    for (int i = 0; i < forest->n; i++) {
        if (forest->nodes[i].parent == -1) {
            count++;
        }
    }
    return count;
}

// 打印森林的存储结构
void PrintForest(CForest *forest) {
    printf("孩子表示法存储的森林：\n");
    printf("  下标  数据  双亲  孩子\n");
    for (int i = 0; i < forest->n; i++) {
        printf("  %-5d %-5d %-5d ", i, forest->nodes[i].data, forest->nodes[i].parent);
        if (forest->nodes[i].firstChild == NULL) {
            printf("无");
        }
        for (CTNode *child = forest->nodes[i].firstChild; child != NULL;
             child = child->nextChild) {
            printf("%d ", forest->nodes[child->childIndex].data);
        }
        printf("%s\n", forest->nodes[i].parent == -1 ? "   <-- 某棵树的根结点" : "");
    }
    printf("森林中共有 %d 棵树\n", CountRoots(forest));
}

// 销毁森林：释放所有孩子链表结点
void DestroyForest(CForest *forest) {
    for (int i = 0; i < forest->n; i++) {
        CTNode *child = forest->nodes[i].firstChild;
        while (child != NULL) {
            CTNode *next = child->nextChild;
            free(child);
            child = next;
        }
        forest->nodes[i].firstChild = NULL;
    }
    forest->n = 0;
}

int main() {
    CForest forest;
    InitForest(&forest);

    // 第 1 棵树：1(2(4,5), 3)   第 2 棵树：6(7)
    AddNode(&forest, 1, -1);  // 下标 0：第 1 棵树的根
    AddNode(&forest, 2, 0);   // 下标 1
    AddNode(&forest, 3, 0);   // 下标 2
    AddNode(&forest, 6, -1);  // 下标 3：第 2 棵树的根
    AddNode(&forest, 4, 1);   // 下标 4
    AddNode(&forest, 5, 1);   // 下标 5
    AddNode(&forest, 7, 3);   // 下标 6：双亲是下标 3（结点 6）
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
    FindChildren(&forest, FindIndex(&forest, 2));
    FindChildren(&forest, FindIndex(&forest, 6));
    FindChildren(&forest, FindIndex(&forest, 5));

    DestroyForest(&forest);
    printf("销毁后结点个数：%d，树的棵数：%d\n", forest.n, CountRoots(&forest));
    return 0;
}
