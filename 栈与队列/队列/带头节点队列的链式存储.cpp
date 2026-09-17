// 链队列（带头结点）：队头指针指向头结点，队尾指针指向最后一个数据结点
// 入队只操作队尾、出队只操作队头，时间复杂度都是 O(1)
#include <stdio.h>
#include <stdlib.h>

typedef struct LiQueueNode {
    int data;                  // 数据域
    struct LiQueueNode *next;  // 指针域
} LiQueueNode;

typedef struct {
    LiQueueNode *front;  // 队头指针（指向头结点）
    LiQueueNode *rear;   // 队尾指针（指向队尾元素）
} LiQueue;

// 初始化：创建头结点，front 与 rear 都指向它
void InitQueue(LiQueue &Q) {
    Q.front = Q.rear = (LiQueueNode *)malloc(sizeof(LiQueueNode));
    if (Q.front == NULL) {
        printf("内存分配失败！\n");
        exit(-1);
    }
    Q.front->next = NULL;
}

// 销毁队列：从队头到队尾逐个释放结点
void DestroyQueue(LiQueue &Q) {
    while (Q.front != NULL) {
        LiQueueNode *temp = Q.front;
        Q.front = Q.front->next;
        free(temp);
    }
    Q.rear = NULL;
}

// 判空：队尾指针追上队头指针（都指向头结点）
bool EmptyQueue(LiQueue Q) {
    return Q.front == Q.rear;
}

// 求队长：从首元结点数到队尾
int QueueLength(LiQueue Q) {
    int len = 0;
    for (LiQueueNode *p = Q.front->next; p != NULL; p = p->next) {
        len++;
    }
    return len;
}

// 入队：新结点接到队尾之后，并更新队尾指针
bool EnQueue(LiQueue &Q, int x) {
    LiQueueNode *newNode = (LiQueueNode *)malloc(sizeof(LiQueueNode));
    if (newNode == NULL) {
        return false;
    }
    newNode->data = x;
    newNode->next = NULL;
    Q.rear->next = newNode;  // 原队尾结点的 next 指向新结点
    Q.rear = newNode;        // 更新队尾指针
    return true;
}

// 出队：摘下头结点之后的第一个数据结点
bool DeQueue(LiQueue &Q, int &x) {
    if (EmptyQueue(Q)) {
        return false;  // 队空，无法出队
    }
    LiQueueNode *temp = Q.front->next;  // temp 指向队头元素
    x = temp->data;
    Q.front->next = temp->next;         // 从队头摘链
    if (Q.rear == temp) {               // 出队的恰好是最后一个元素，需修正队尾指针
        Q.rear = Q.front;
    }
    free(temp);
    return true;
}

// 读取队头元素（不出队）
bool GetHead(LiQueue Q, int &x) {
    if (EmptyQueue(Q)) {
        return false;
    }
    x = Q.front->next->data;
    return true;
}

// 打印队列（从队头到队尾）
void PrintQueue(LiQueue Q) {
    printf("链队列(长度=%d)：队头 -> ", QueueLength(Q));
    for (LiQueueNode *p = Q.front->next; p != NULL; p = p->next) {
        printf("%d ", p->data);
    }
    printf("<- 队尾\n");
}

int main() {
    LiQueue Q;
    int x;
    InitQueue(Q);
    printf("初始化后是否为空：%s\n", EmptyQueue(Q) ? "是" : "否");

    for (int i = 1; i <= 4; i++) {  // 依次入队 10 20 30 40
        EnQueue(Q, i * 10);
    }
    PrintQueue(Q);

    if (GetHead(Q, x)) {
        printf("队头元素：%d\n", x);
    }
    while (!EmptyQueue(Q)) {  // 全部出队，验证队尾指针的修正
        DeQueue(Q, x);
        printf("出队元素：%d\n", x);
    }
    EnQueue(Q, 99);  // 空队列入队，验证队尾指针已恢复正确
    PrintQueue(Q);
    printf("此时队列是否为空：%s\n", EmptyQueue(Q) ? "是" : "否");

    DestroyQueue(Q);
    return 0;
}
