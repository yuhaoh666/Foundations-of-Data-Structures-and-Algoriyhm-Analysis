// 二叉树的链式存储：每个结点包含数据域、左孩子指针、右孩子指针
// 另外给出三叉链表（多一个双亲指针 parent）的定义，便于向上查找双亲
#include <stdio.h>
#include <stdlib.h>

// 二叉链表
typedef struct BTreeNode {
    int data;                 // 数据域
    struct BTreeNode *left;   // 左孩子指针
    struct BTreeNode *right;  // 右孩子指针
} BTreeNode, *BTree;

// 三叉链表：多一个指向双亲的指针
typedef struct TriNode {
    int data;               // 数据域
    struct TriNode *left;   // 左孩子指针
    struct TriNode *right;  // 右孩子指针
    struct TriNode *parent; // 双亲指针
} TriNode, *TriTree;

// 初始化：必须传引用（或二级指针），否则改的只是形参副本
void InitTree(BTree &root) {
    root = NULL;
}

// 判空
bool IsEmpty(BTree root) {
    return root == NULL;
}

// 创建新结点，失败返回 NULL
BTreeNode *CreateNode(int data) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 给结点 parent 挂上左孩子；若已有左孩子则失败
bool InsertLeft(BTree &parent, int data) {
    if (parent == NULL || parent->left != NULL) {
        return false;
    }
    BTreeNode *node = CreateNode(data);
    if (node == NULL) {
        return false;
    }
    parent->left = node;
    return true;
}

// 给结点 parent 挂上右孩子；若已有右孩子则失败
bool InsertRight(BTree &parent, int data) {
    if (parent == NULL || parent->right != NULL) {
        return false;
    }
    BTreeNode *node = CreateNode(data);
    if (node == NULL) {
        return false;
    }
    parent->right = node;
    return true;
}

// 求树的深度（高度）：空树深度为 0，否则为左右子树深度的较大值加 1
int TreeDepth(BTree root) {
    if (root == NULL) {
        return 0;
    }
    int leftDepth = TreeDepth(root->left);
    int rightDepth = TreeDepth(root->right);
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

// 统计结点总数
int CountNodes(BTree root) {
    if (root == NULL) {
        return 0;
    }
    return CountNodes(root->left) + CountNodes(root->right) + 1;
}

// 统计叶子结点数
int CountLeaves(BTree root) {
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right == NULL) {  // 左右孩子都空即为叶子
        return 1;
    }
    return CountLeaves(root->left) + CountLeaves(root->right);
}

// 销毁二叉树（后序遍历顺序释放）
void DestroyTree(BTree &root) {
    if (root == NULL) {
        return;
    }
    DestroyTree(root->left);
    DestroyTree(root->right);
    free(root);
    root = NULL;  // 置空，避免野指针
}

int main() {
    BTree root;
    InitTree(root);
    printf("初始化后是否为空树：%s\n", IsEmpty(root) ? "是" : "否");

    root = CreateNode(1);            // 根
    InsertLeft(root, 2);             // 1 的左孩子
    InsertRight(root, 3);            // 1 的右孩子
    InsertLeft(root->left, 4);       // 2 的左孩子
    InsertRight(root->left, 5);      // 2 的右孩子
    printf("重复挂左孩子：%s\n", InsertLeft(root->left, 6) ? "成功" : "失败（已有左孩子）");

    printf("树的深度：%d\n", TreeDepth(root));
    printf("结点总数：%d\n", CountNodes(root));
    printf("叶子结点数：%d\n", CountLeaves(root));

    DestroyTree(root);
    printf("销毁后是否为空树：%s\n", IsEmpty(root) ? "是" : "否");
    return 0;
}
