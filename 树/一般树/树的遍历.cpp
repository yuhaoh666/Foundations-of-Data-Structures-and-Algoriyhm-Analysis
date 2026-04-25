#include <stdio.h>
#include <stdlib.h>

// 一般树的遍历操作
typedef struct TreeNode {
    int data; // 结点数据
    struct TreeNode* firstChild; // 指向第一个孩子
    struct TreeNode* nextSibling; // 指向下一个兄弟
} TreeNode, *Tree;

// 初始化树
void initTree(Tree* tree) {
    *tree = NULL; // 初始化树为空
}

// 树的先根遍历
void preOrder(Tree tree) {
    if (tree == NULL) return;
    printf("%d ", tree->data); // 访问根节点
    preOrder(tree->firstChild); // 递归访问第一个孩子
    preOrder(tree->nextSibling); // 递归访问下一个兄弟
}

// 树的后根遍历
void postOrder(Tree tree) {
    if (tree == NULL) return;
    postOrder(tree->firstChild); // 递归访问第一个孩子
    postOrder(tree->nextSibling); // 递归访问下一个兄弟
    printf("%d ", tree->data); // 访问根节点
}

// 树的层次遍历
void levelOrder(Tree tree) {
    if (tree == NULL) return;
    Tree queue[100]; // 用数组模拟队列
    int front = 0, rear = 0;
    queue[rear++] = tree; // 将根节点入队
    while (front < rear) {
        Tree current = queue[front++]; // 出队
        printf("%d ", current->data); // 访问当前节点
        Tree child = current->firstChild;
        while (child != NULL) {
            queue[rear++] = child; // 将孩子节点入队
            child = child->nextSibling; // 访问下一个兄弟
        }
    }
}
       