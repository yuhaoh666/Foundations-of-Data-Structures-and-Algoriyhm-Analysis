#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;
    struct TreeNode *firstChild;   // 左孩子（第一棵子树）
    struct TreeNode *nextSibling;  // 右兄弟（下一棵树）
} TreeNode, *Tree;

typedef struct {
    Tree *trees;   // 森林中每棵树的根节点数组
    int n;         // 树的数量
} Forest;

// 初始化森林
void initForest(Forest *forest) {
    forest->trees = NULL;
    forest->n = 0;
}

// ========== 针对单棵树的遍历（孩子兄弟链表） ==========

// 森林的先序遍历（等价于二叉树的先序：根-左-右）
void preOrder(Tree root) {
    if (root == NULL) return;
    printf("%d ", root->data);
    preOrder(root->firstChild);
    preOrder(root->nextSibling);
}

// 森林的中序遍历（等价于二叉树的中序：左-根-右）
void inOrder(Tree root) {
    if (root == NULL) return;
    inOrder(root->firstChild);
    printf("%d ", root->data);
    inOrder(root->nextSibling);
}

// 森林的后序遍历（等价于二叉树的后序：左-右-根）
void postOrder(Tree root) {
    if (root == NULL) return;
    postOrder(root->firstChild);
    postOrder(root->nextSibling);
    printf("%d ", root->data);
}

// ========== 遍历整个 Forest 结构体 ==========

// 先序遍历森林（依次先序遍历每一棵树）
void preOrderForest(Forest *forest) {
    for (int i = 0; i < forest->n; i++) {
        preOrder(forest->trees[i]);
    }
}

// 中序遍历森林
void inOrderForest(Forest *forest) {
    for (int i = 0; i < forest->n; i++) {
        inOrder(forest->trees[i]);
    }
}

// 后序遍历森林
void postOrderForest(Forest *forest) {
    for (int i = 0; i < forest->n; i++) {
        postOrder(forest->trees[i]);
    }
}