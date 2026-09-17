// 链式二叉树的先序、中序、后序遍历（递归实现）以及中序的非递归实现
// 三种递归遍历的时间复杂度都是 O(n)；递归工作栈的深度即树的深度
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100

typedef struct TreeNode {
    int data;                 // 数据域
    struct TreeNode *left;    // 左孩子指针
    struct TreeNode *right;   // 右孩子指针
} TreeNode, *Tree;

// 初始化（传入指针的地址，才能真正把根指针置空）
void InitTree(Tree *root) {
    *root = NULL;
}

// 判空
bool IsEmpty(Tree root) {
    return root == NULL;
}

// 创建新结点
Tree CreateNode(int data) {
    Tree node = (Tree)malloc(sizeof(TreeNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 先序遍历：根 -> 左 -> 右
void PreOrder(Tree root) {
    if (root != NULL) {
        printf("%d ", root->data);  // 先访问根
        PreOrder(root->left);       // 再递归遍历左子树
        PreOrder(root->right);      // 最后递归遍历右子树
    }
}

// 中序遍历：左 -> 根 -> 右
void InOrder(Tree root) {
    if (root != NULL) {
        InOrder(root->left);
        printf("%d ", root->data);
        InOrder(root->right);
    }
}

// 后序遍历：左 -> 右 -> 根
void PostOrder(Tree root) {
    if (root != NULL) {
        PostOrder(root->left);
        PostOrder(root->right);
        printf("%d ", root->data);
    }
}

// 中序遍历的非递归实现：用栈保存沿途经过的结点，出栈时才访问（左 -> 根 -> 右）
void InOrderNonRecursive(Tree root) {
    Tree stack[MAXSIZE];  // 用数组模拟栈
    int top = -1;
    Tree p = root;
    while (p != NULL || top != -1) {
        if (p != NULL) {   // 一路向左，把经过的结点压栈
            stack[++top] = p;
            p = p->left;
        } else {           // 左子树走完，弹出栈顶访问，再转向其右子树
            p = stack[top--];
            printf("%d ", p->data);
            p = p->right;
        }
    }
}

// 销毁二叉树（后序顺序释放所有结点）
void DestroyTree(Tree *root) {
    if (*root == NULL) {
        return;
    }
    DestroyTree(&((*root)->left));
    DestroyTree(&((*root)->right));
    free(*root);
    *root = NULL;
}

int main() {
    Tree root;
    InitTree(&root);
    printf("初始化后是否为空树：%s\n", IsEmpty(root) ? "是" : "否");

    // 构造如下二叉树：
    //         1
    //        / \
    //       2   3
    //      / \
    //     4   5
    root = CreateNode(1);
    root->left = CreateNode(2);
    root->right = CreateNode(3);
    root->left->left = CreateNode(4);
    root->left->right = CreateNode(5);

    printf("先序遍历：");
    PreOrder(root);
    printf("\n中序遍历（递归）：");
    InOrder(root);
    printf("\n中序遍历（非递归）：");
    InOrderNonRecursive(root);
    printf("\n后序遍历：");
    PostOrder(root);
    printf("\n");

    DestroyTree(&root);
    printf("销毁后是否为空树：%s\n", IsEmpty(root) ? "是" : "否");
    return 0;
}
