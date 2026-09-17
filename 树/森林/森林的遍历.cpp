// 森林的遍历
//   森林的先序遍历 = 依次先根遍历森林中的每一棵树（等价于其对应二叉树的先序遍历）
//   森林的中序遍历 = 依次后根遍历森林中的每一棵树（等价于其对应二叉树的中序遍历）
#include <stdio.h>
#include <stdlib.h>
#define INIT_CAPACITY 4

typedef struct TreeNode {
    int data;                      // 结点数据
    struct TreeNode *firstChild;   // 第一个孩子
    struct TreeNode *nextSibling;  // 下一个兄弟
} TreeNode, *Tree;

typedef struct {
    Tree *trees;   // 森林中每棵树的根结点数组
    int n;         // 树的数量
    int capacity;  // 数组容量
} Forest;

// ==================== 单棵树的结点操作 ====================
// 创建结点
Tree CreateNode(int data) {
    Tree node = (Tree)malloc(sizeof(TreeNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->data = data;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

// 给 parent 追加一个孩子
bool AddChild(TreeNode *parent, int data) {
    if (parent == NULL) {
        return false;
    }
    Tree node = CreateNode(data);
    if (node == NULL) {
        return false;
    }
    if (parent->firstChild == NULL) {
        parent->firstChild = node;
    } else {
        Tree child = parent->firstChild;
        while (child->nextSibling != NULL) {
            child = child->nextSibling;
        }
        child->nextSibling = node;
    }
    return true;
}

// 按数据查找结点（在一棵树内查找，兄弟链也一并搜索）
Tree FindNode(Tree tree, int data) {
    for (Tree t = tree; t != NULL; t = t->nextSibling) {
        if (t->data == data) {
            return t;
        }
        Tree res = FindNode(t->firstChild, data);
        if (res != NULL) {
            return res;
        }
    }
    return NULL;
}

// 先根遍历一棵树：访问根，再依次先根遍历每棵子树
void PreOrderTree(Tree tree) {
    if (tree == NULL) {
        return;
    }
    printf("%d ", tree->data);
    for (Tree child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PreOrderTree(child);
    }
}

// 后根遍历一棵树：先依次后根遍历每棵子树，最后访问根
void PostOrderTree(Tree tree) {
    if (tree == NULL) {
        return;
    }
    for (Tree child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PostOrderTree(child);
    }
    printf("%d ", tree->data);
}

// 求一棵树的深度
int TreeDepth(Tree tree) {
    if (tree == NULL) {
        return 0;
    }
    int maxChildDepth = 0;
    for (Tree child = tree->firstChild; child != NULL; child = child->nextSibling) {
        int d = TreeDepth(child);
        if (d > maxChildDepth) {
            maxChildDepth = d;
        }
    }
    return maxChildDepth + 1;
}

// 释放一棵树（含它的兄弟链）
void DestroyTree(Tree *tree) {
    if (*tree == NULL) {
        return;
    }
    DestroyTree(&((*tree)->firstChild));
    Tree sibling = (*tree)->nextSibling;
    free(*tree);
    *tree = NULL;
    if (sibling != NULL) {
        DestroyTree(&sibling);
    }
}

// ==================== 森林的操作 ====================
// 初始化森林：动态申请根结点数组
bool InitForest(Forest *forest) {
    forest->trees = (Tree *)malloc(INIT_CAPACITY * sizeof(Tree));
    if (forest->trees == NULL) {
        printf("内存分配失败！\n");
        return false;
    }
    forest->n = 0;
    forest->capacity = INIT_CAPACITY;
    return true;
}

// 把一棵树的根加入森林（容量不足时自动扩容）
bool AddTree(Forest *forest, Tree root) {
    if (root == NULL) {
        return false;
    }
    if (forest->n >= forest->capacity) {
        int newCapacity = forest->capacity * 2;
        Tree *newTrees = (Tree *)malloc(newCapacity * sizeof(Tree));
        if (newTrees == NULL) {
            printf("内存分配失败！\n");
            return false;
        }
        for (int i = 0; i < forest->n; i++) {  // 搬迁原有的树根
            newTrees[i] = forest->trees[i];
        }
        free(forest->trees);
        forest->trees = newTrees;
        forest->capacity = newCapacity;
    }
    forest->trees[forest->n++] = root;
    return true;
}

// 森林的先序遍历
void PreOrderForest(Forest *forest) {
    for (int i = 0; i < forest->n; i++) {
        PreOrderTree(forest->trees[i]);
    }
}

// 森林的中序遍历（即依次后根遍历每棵树）
void PostOrderForest(Forest *forest) {
    for (int i = 0; i < forest->n; i++) {
        PostOrderTree(forest->trees[i]);
    }
}

// 求森林中最高那棵树的深度
int ForestDepth(Forest *forest) {
    int maxDepth = 0;
    for (int i = 0; i < forest->n; i++) {
        int d = TreeDepth(forest->trees[i]);
        if (d > maxDepth) {
            maxDepth = d;
        }
    }
    return maxDepth;
}

// 用括号表示法打印森林
void PrintTree(Tree tree) {
    if (tree == NULL) {
        return;
    }
    printf("%d", tree->data);
    if (tree->firstChild == NULL) {
        return;
    }
    printf("(");
    for (Tree child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PrintTree(child);
        if (child->nextSibling != NULL) {
            printf(",");
        }
    }
    printf(")");
}

void PrintForest(Forest *forest) {
    printf("森林（括号表示法）：");
    for (int i = 0; i < forest->n; i++) {
        PrintTree(forest->trees[i]);
        printf(" ");
    }
    printf("\n");
}

// 销毁森林：先释放每棵树，再释放根结点数组
void DestroyForest(Forest *forest) {
    for (int i = 0; i < forest->n; i++) {
        DestroyTree(&(forest->trees[i]));
    }
    free(forest->trees);
    forest->trees = NULL;
    forest->n = 0;
    forest->capacity = 0;
}

int main() {
    Forest forest;
    InitForest(&forest);

    // 第 1 棵树：A -> B(D, E)、C(F)，用整数 1~6 表示
    Tree t1 = CreateNode(1);
    AddChild(t1, 2);
    AddChild(t1, 3);
    AddChild(FindNode(t1, 2), 4);
    AddChild(FindNode(t1, 2), 5);
    AddChild(FindNode(t1, 3), 6);
    AddTree(&forest, t1);

    // 第 2 棵树：7 -> 8
    Tree t2 = CreateNode(7);
    AddChild(t2, 8);
    AddTree(&forest, t2);

    // 第 3 棵树：9（只有一个结点）
    AddTree(&forest, CreateNode(9));

    printf("森林中共有 %d 棵树，最大深度为 %d\n", forest.n, ForestDepth(&forest));
    PrintForest(&forest);

    printf("森林的先序遍历（依次先根遍历每棵树）：");
    PreOrderForest(&forest);
    printf("\n森林的中序遍历（依次后根遍历每棵树）：");
    PostOrderForest(&forest);
    printf("\n");

    DestroyForest(&forest);
    printf("销毁后森林中树的棵数：%d\n", forest.n);
    return 0;
}
