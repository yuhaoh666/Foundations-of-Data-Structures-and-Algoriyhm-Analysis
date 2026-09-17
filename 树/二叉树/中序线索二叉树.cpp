// 中序线索二叉树：把二叉链表中空闲的左、右指针利用起来存放前驱/后继线索
// ltag / rtag 为 true 时，left / right 指向的不是孩子，而是“前驱 / 后继”线索
// 好处：中序遍历不需要栈，也不需要递归
#include <stdio.h>
#include <stdlib.h>

typedef struct ThreadNode {
    int data;                  // 数据域
    struct ThreadNode *left;   // 左指针（当 ltag 为 true 时是前驱线索）
    struct ThreadNode *right;  // 右指针（当 rtag 为 true 时是后继线索）
    bool ltag, rtag;           // 左、右指针是否为线索
} ThreadNode, *ThreadTree;

// 初始化
void InitTree(ThreadTree *root) {
    *root = NULL;
}

// 判空
bool IsEmpty(ThreadTree root) {
    return root == NULL;
}

// 创建结点：标记位默认为 false，表示指针指向的是孩子
ThreadNode *CreateNode(int data) {
    ThreadNode *node = (ThreadNode *)malloc(sizeof(ThreadNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->data = data;
    node->left = node->right = NULL;
    node->ltag = node->rtag = false;
    return node;
}

// 中序遍历的同时建立线索：pre 始终指向刚刚访问过的结点（即当前结点的前驱）
void InOrderThreading(ThreadTree root, ThreadNode **pre) {
    if (root == NULL) {
        return;
    }
    InOrderThreading(root->left, pre);  // 先递归处理左子树
    if (root->left == NULL) {           // 左孩子为空，让 left 指向前驱
        root->ltag = true;
        root->left = *pre;
    }
    if (*pre != NULL && (*pre)->right == NULL) {  // 前驱的右孩子为空，让它的 right 指向后继（当前结点）
        (*pre)->rtag = true;
        (*pre)->right = root;
    }
    *pre = root;                        // 更新 pre
    InOrderThreading(root->right, pre); // 再递归处理右子树
}

// 建立中序线索二叉树的入口
void CreateInOrderThreadTree(ThreadTree root) {
    ThreadNode *pre = NULL;
    InOrderThreading(root, &pre);
    if (pre != NULL) {  // 中序序列的最后一个结点没有后继，显式置为空线索
        pre->rtag = true;
        pre->right = NULL;
    }
}

// 求中序序列的第一个结点：从 p 出发一路向左，最左下的结点
ThreadNode *FirstNode(ThreadNode *p) {
    while (p != NULL && !p->ltag) {
        p = p->left;
    }
    return p;
}

// 求结点 p 的中序后继
ThreadNode *InOrderSuccessor(ThreadNode *p) {
    if (p->rtag) {                 // 已经是后继线索，直接返回
        return p->right;
    }
    return FirstNode(p->right);    // 否则是右子树中最左下的结点
}

// 求结点 p 的中序前驱
ThreadNode *InOrderPredecessor(ThreadNode *p) {
    if (p->ltag) {                 // 已经是前驱线索
        return p->left;
    }
    ThreadNode *temp = p->left;    // 否则是左子树中最右下的结点
    while (temp != NULL && !temp->rtag) {
        temp = temp->right;
    }
    return temp;
}

// 沿线索中序遍历（不使用栈、不使用递归）
void InOrderTraverse(ThreadTree root) {
    printf("中序遍历（用线索，无需栈）：");
    for (ThreadNode *p = FirstNode(root); p != NULL; p = InOrderSuccessor(p)) {
        printf("%d ", p->data);
    }
    printf("\n");
}

// 借助前驱线索做逆序中序遍历
void InOrderReverse(ThreadTree root) {
    printf("逆序中序遍历：");
    if (root == NULL) {
        printf("空树\n");
        return;
    }
    ThreadNode *p = root;
    while (p->right != NULL && !p->rtag) {  // 找到中序序列的最后一个结点
        p = p->right;
    }
    for (; p != NULL; p = InOrderPredecessor(p)) {
        printf("%d ", p->data);
    }
    printf("\n");
}

// 销毁线索二叉树：只沿着真正的孩子指针递归，不能走线索
void DestroyTree(ThreadTree *root) {
    if (*root == NULL) {
        return;
    }
    if (!(*root)->ltag) {
        DestroyTree(&((*root)->left));
    }
    if (!(*root)->rtag) {
        DestroyTree(&((*root)->right));
    }
    free(*root);
    *root = NULL;
}

int main() {
    ThreadTree root;
    InitTree(&root);

    // 构造如下二叉树（中序序列为 4 2 5 1 3）：
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

    CreateInOrderThreadTree(root);  // 建立中序线索
    InOrderTraverse(root);
    InOrderReverse(root);

    // 测试查找某个结点的前驱与后继
    ThreadNode *p = root->left;  // 结点 2，中序序列中前驱是 4、后继是 5
    ThreadNode *pre = InOrderPredecessor(p);
    ThreadNode *next = InOrderSuccessor(p);
    printf("结点 %d 的中序前驱：", p->data);
    if (pre == NULL) {
        printf("无\n");
    } else {
        printf("%d\n", pre->data);
    }
    printf("结点 %d 的中序后继：%d\n", p->data, next == NULL ? 0 : next->data);
    printf("结点 %d 的左右指针是否为线索：ltag=%s, rtag=%s\n", p->data,
           p->ltag ? "true" : "false", p->rtag ? "true" : "false");

    DestroyTree(&root);
    printf("销毁后是否为空树：%s\n", IsEmpty(root) ? "是" : "否");
    return 0;
}
