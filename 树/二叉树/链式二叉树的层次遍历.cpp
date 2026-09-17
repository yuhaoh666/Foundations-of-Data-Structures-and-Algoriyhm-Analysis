// 链式二叉树的层次遍历：借助队列逐层从左到右访问所有结点
// 每个结点只入队、出队一次，时间复杂度 O(n)
#include <stdio.h>
#include <stdlib.h>

typedef struct BTreeNode {
    int data;                 // 数据域
    struct BTreeNode *left;   // 左孩子指针
    struct BTreeNode *right;  // 右孩子指针
} BTreeNode, *BTree;

// 初始化
void InitTree(BTree *root) {
    *root = NULL;
}

// 判空
bool IsEmpty(BTree root) {
    return root == NULL;
}

// 创建新结点
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

// ==================== 辅助链队列（结点队列） ====================
typedef struct QueueNode {
    BTreeNode *data;         // 数据域存放二叉树结点的指针
    struct QueueNode *next;  // 指针域
} QueueNode;

typedef struct {
    QueueNode *front;  // 队头指针（指向头结点）
    QueueNode *rear;   // 队尾指针
} LiQueue;

// 初始化队列：创建头结点
bool InitQueue(LiQueue *q) {
    q->front = q->rear = (QueueNode *)malloc(sizeof(QueueNode));
    if (q->front == NULL) {
        return false;
    }
    q->front->next = NULL;
    return true;
}

// 判空
bool IsQueueEmpty(LiQueue *q) {
    return q->front == q->rear;
}

// 入队
bool EnQueue(LiQueue *q, BTreeNode *node) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        return false;
    }
    newNode->data = node;
    newNode->next = NULL;
    q->rear->next = newNode;
    q->rear = newNode;
    return true;
}

// 出队：成功返回结点指针，队空返回 NULL
BTreeNode *DeQueue(LiQueue *q) {
    if (IsQueueEmpty(q)) {
        return NULL;
    }
    QueueNode *temp = q->front->next;
    BTreeNode *node = temp->data;
    q->front->next = temp->next;
    if (q->rear == temp) {  // 出队的是最后一个结点，修正队尾指针
        q->rear = q->front;
    }
    free(temp);
    return node;
}

// 销毁队列
void DestroyQueue(LiQueue *q) {
    while (q->front != NULL) {
        QueueNode *temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
}

// ==================== 层次遍历 ====================
// 逐层输出每个结点，并在每层结束时换行，便于观察树的结构
void LevelOrder(BTree root) {
    if (root == NULL) {
        printf("空树，无法遍历\n");
        return;
    }
    LiQueue q;
    if (!InitQueue(&q)) {
        printf("队列初始化失败！\n");
        return;
    }
    EnQueue(&q, root);
    printf("层次遍历：\n");
    while (!IsQueueEmpty(&q)) {
        int levelCount = 0;  // 当前层的结点个数
        for (QueueNode *p = q.front->next; p != NULL; p = p->next) {
            levelCount++;
        }
        for (int i = 0; i < levelCount; i++) {  // 一次处理完一整层
            BTreeNode *current = DeQueue(&q);
            printf("%d ", current->data);
            if (current->left != NULL) {
                EnQueue(&q, current->left);   // 左孩子入队
            }
            if (current->right != NULL) {
                EnQueue(&q, current->right);  // 右孩子入队
            }
        }
        printf("\n");  // 一层处理完换行
    }
    DestroyQueue(&q);
}

// 销毁二叉树
void DestroyTree(BTree *root) {
    if (*root == NULL) {
        return;
    }
    DestroyTree(&((*root)->left));
    DestroyTree(&((*root)->right));
    free(*root);
    *root = NULL;
}

int main() {
    BTree root;
    InitTree(&root);

    // 构造如下二叉树：
    //         1
    //        / \
    //       2   3
    //      / \   \
    //     4   5   6
    root = CreateNode(1);
    root->left = CreateNode(2);
    root->right = CreateNode(3);
    root->left->left = CreateNode(4);
    root->left->right = CreateNode(5);
    root->right->right = CreateNode(6);

    LevelOrder(root);

    DestroyTree(&root);
    printf("销毁后是否为空树：%s\n", IsEmpty(root) ? "是" : "否");
    return 0;
}
