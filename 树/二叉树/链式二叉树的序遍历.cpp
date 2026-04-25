#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode, *Tree;

void initTree(Tree* root) {
    *root = NULL;
}

bool isEmpty(Tree root) {
    return root == NULL;
}

// 先序遍历
void preOrder(Tree root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// 中序遍历
void inOrder(Tree root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->data);
        inOrder(root->right);
    }
}

// 后序遍历
void postOrder(Tree root) {
    if (root != NULL) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->data);
    }
}

int main() {
    Tree root;
    initTree(&root);
    return 0;
}