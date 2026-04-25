#include <stdio.h>
#include <stdlib.h>

typedef struct LiQueueNode {
    int data;  // 数据域
    struct LiQueueNode *next;  // 指针域
} LiQueueNode;

typedef struct {
    LiQueueNode *front;  // 队头指针
    LiQueueNode *rear;  // 队尾指针
} LiQueue;

void InitQueue(LiQueue &Q) {
    Q.front = Q.rear = (LiQueueNode *)malloc(sizeof(LiQueueNode));  // 创建头结点，初始化队头和队尾指针
    if (Q.front == NULL) 
        exit(-1);
    Q.front->next = NULL;  // 头结点的指针域置空
}

void DestroyQueue(LiQueue &Q) {
    while (Q.front != NULL) {
        LiQueueNode *temp = Q.front;  // 临时指针
        Q.front = Q.front->next;  // 移动队头指针
        free(temp);  // 释放节点内存
    }
    Q.rear = NULL;  // 销毁队列，重置队尾指针
}

bool EmptyQueue(LiQueue Q) {
    return Q.front == Q.rear;  // 判断队列是否为空
}

bool EnQueue(LiQueue &Q, int x) {
    LiQueueNode *newNode = (LiQueueNode *)malloc(sizeof(LiQueueNode));  // 创建新节点
    if (newNode == NULL) 
        return false; 
    newNode->data = x;  // 设置数据域
    newNode->next = NULL;  // 设置指针域
    Q.rear->next = newNode;  // 将新节点链接到队尾
    Q.rear = newNode;  // 更新队尾指针
    return true;
}

bool DeQueue(LiQueue &Q, int &x) {
    if (EmptyQueue(Q)) 
        return false;  // 队列空，无法出队
    LiQueueNode *temp = Q.front->next;  // 临时指针，指向队头元素
    x = temp->data;  // 获取队头元素
    Q.front->next = temp->next;  // 更新队头指针
    if (Q.rear == temp) {  // 如果出队的是最后一个元素，更新队尾指针
        Q.rear = Q.front;
    }
    free(temp);  // 释放节点内存
    return true;
}

int main() {
    LiQueue Q;
    InitQueue(Q);
    return 0;
}

