#include <stdio.h>
#include <stdlib.h>

typedef struct BTreeNode {
    int data;
    struct BTreeNode* left;
    struct BTreeNode* right;
} BTreeNode, *BTree;

// 三叉链表定义
typedef struct TriNode {
    int data;
    struct TriNode* left;
    struct TriNode* right;
    struct TriNode* parent;
} TriNode, *TriTree;

void initTree(BTree root) {
    root = NULL;
}

bool isEmpty(BTree root) {
    return root == NULL;
}

int TreeDepth(BTree root) {  // 计算二叉树的深度
    if (root == NULL) {
        return 0;
    }
    int leftDepth = TreeDepth(root->left);
    int rightDepth = TreeDepth(root->right);
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

int main() {
    BTree root;
    initTree(root);
    return 0;
}