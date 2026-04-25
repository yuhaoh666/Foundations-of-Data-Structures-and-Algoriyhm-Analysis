#include <stdio.h>
#include <stdlib.h>

typedef struct BTreeNode {
    int data;
    struct BTreeNode* left;
    struct BTreeNode* right;
} BTreeNode, *BTree;

void initTree(BTree* root) {
    *root = NULL;
}

bool isEmpty(BTree root) {
    return root == NULL;
}

typedef struct LiQueueNode {
    BTreeNode* data;
    struct LiQueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} LiQueue;

void initQueue(LiQueue* q) {
    q->front = q->rear = (QueueNode*)malloc(sizeof(QueueNode));
    q->front->next = NULL;
}

bool isQueueEmpty(LiQueue* q) {
    return q->front == q->rear;
}

bool enqueue(LiQueue* q, BTreeNode* node) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) return false;
    newNode->data = node;
    newNode->next = NULL;
    q->rear->next = newNode;
    q->rear = newNode;
    return true;
}

BTreeNode* dequeue(LiQueue* q) {
    if (isQueueEmpty(q)) return NULL;
    QueueNode* temp = q->front->next;
    BTreeNode* node = temp->data;
    q->front->next = temp->next;
    if (q->rear == temp) {
        q->rear = q->front;
    }
    free(temp);
    return node;
}

void levelOrder(BTree root) {
    if (root == NULL) return;
    LiQueue q;
    initQueue(&q);
    enqueue(&q, root);
    while (!isQueueEmpty(&q)) {
        BTreeNode* current = dequeue(&q);
        printf("%d ", current->data);
        if (current->left) {
            enqueue(&q, current->left);
        }
        if (current->right) {
            enqueue(&q, current->right);
        }
    }
}

int main() {
    BTree root;
    initTree(&root);
    return 0;
}