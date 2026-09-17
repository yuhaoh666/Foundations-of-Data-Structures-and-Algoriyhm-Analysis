// 一般树的遍历（孩子兄弟表示法）：先根遍历、后根遍历、层次遍历
// 结论：树的先根遍历序列 == 其对应二叉树的先序序列；
//       树的后根遍历序列 == 其对应二叉树的中序序列
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100

typedef struct TreeNode {
    int data;                      // 结点数据
    struct TreeNode *firstChild;   // 第一个孩子
    struct TreeNode *nextSibling;  // 下一个兄弟
} TreeNode, *Tree;

// 初始化：空树用 NULL 表示
void InitTree(Tree *tree) {
    *tree = NULL;
}

// 判空
bool IsEmpty(Tree tree) {
    return tree == NULL;
}

// 创建结点
TreeNode *CreateNode(int data) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->data = data;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

// 建立根结点（仅空树可用）
bool SetRoot(Tree *tree, int data) {
    if (*tree != NULL) {
        return false;
    }
    *tree = CreateNode(data);
    return *tree != NULL;
}

// 在 parent 的孩子链表末尾追加一个孩子
bool AddChild(TreeNode *parent, int data) {
    if (parent == NULL) {
        return false;
    }
    TreeNode *node = CreateNode(data);
    if (node == NULL) {
        return false;
    }
    if (parent->firstChild == NULL) {
        parent->firstChild = node;
    } else {
        TreeNode *child = parent->firstChild;
        while (child->nextSibling != NULL) {
            child = child->nextSibling;
        }
        child->nextSibling = node;
    }
    return true;
}

// 按数据查找结点
TreeNode *FindNode(Tree tree, int data) {
    if (tree == NULL) {
        return NULL;
    }
    if (tree->data == data) {
        return tree;
    }
    for (TreeNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        TreeNode *res = FindNode(child, data);
        if (res != NULL) {
            return res;
        }
    }
    return NULL;
}

// 先根遍历：先访问根结点，再依次先根遍历每一棵子树
void PreOrder(Tree tree) {
    if (tree == NULL) {
        return;
    }
    printf("%d ", tree->data);  // 访问根结点
    for (Tree child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PreOrder(child);        // 依次递归每一棵子树
    }
}

// 后根遍历：先依次后根遍历每一棵子树，最后访问根结点
void PostOrder(Tree tree) {
    if (tree == NULL) {
        return;
    }
    for (Tree child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PostOrder(child);
    }
    printf("%d ", tree->data);
}

// 层次遍历：借助数组模拟的队列，逐层从左到右访问所有结点
void LevelOrder(Tree tree) {
    if (tree == NULL) {
        return;
    }
    Tree queue[MAXSIZE];  // 队列中保存的是结点指针
    int front = 0, rear = 0;
    queue[rear++] = tree;  // 根结点入队
    while (front < rear) {
        Tree current = queue[front++];  // 出队并访问
        printf("%d ", current->data);
        for (Tree child = current->firstChild; child != NULL; child = child->nextSibling) {
            if (rear >= MAXSIZE) {  // 队列容量保护（正常规模的树不会触发）
                printf("\n(队列已满，层次遍历提前结束)");
                return;
            }
            queue[rear++] = child;  // 所有孩子依次入队
        }
    }
}

// 求树的深度
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

// 销毁树：先释放子树，再释放兄弟链
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

int main() {
    Tree tree;
    InitTree(&tree);

    // 构造如下树：
    //        1
    //      / | \
    //     2  3  4
    //    / \
    //   5   6
    SetRoot(&tree, 1);
    AddChild(FindNode(tree, 1), 2);
    AddChild(FindNode(tree, 1), 3);
    AddChild(FindNode(tree, 1), 4);
    AddChild(FindNode(tree, 2), 5);
    AddChild(FindNode(tree, 2), 6);

    printf("先根遍历：");
    PreOrder(tree);
    printf("\n后根遍历：");
    PostOrder(tree);
    printf("\n层次遍历：");
    LevelOrder(tree);
    printf("\n树的深度：%d\n", TreeDepth(tree));

    DestroyTree(&tree);
    printf("销毁后是否为空树：%s\n", IsEmpty(tree) ? "是" : "否");
    return 0;
}
