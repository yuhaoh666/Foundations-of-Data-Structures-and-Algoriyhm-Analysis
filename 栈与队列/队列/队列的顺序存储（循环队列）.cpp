// 循环队列的顺序实现：用取模运算把数组“首尾相接”，避免假溢出
// 约定：牺牲一个存储单元来区分队空与队满
//   队空：front == rear；队满：(rear + 1) % Maxsize == front
#include <stdio.h>
#define Maxsize 100

typedef struct {
    int data[Maxsize];  // 存放队列元素
    int front;          // 队头指针，指向队头元素
    int rear;           // 队尾指针，指向队尾元素的下一个位置
} SqQueue;

// 初始化：两个指针都指向 0
void InitQueue(SqQueue &Q) {
    Q.rear = Q.front = 0;
}

// 销毁队列：顺序存储由系统回收，逻辑置空即可
void DestroyQueue(SqQueue &Q) {
    Q.front = 0;
    Q.rear = 0;
}

// 判空
bool EmptyQueue(SqQueue Q) {
    return Q.front == Q.rear;
}

// 判满：队尾指针再进一格就会撞上队头
// 说明：若想利用全部存储空间，可额外用 size 计数或 tag 标志来区分队空/队满（此时要改写判满与求长）
bool FullQueue(SqQueue Q) {
    return (Q.rear + 1) % Maxsize == Q.front;
}

// 入队：元素放到队尾，队尾指针后移一格（取模实现循环）
bool EnQueue(SqQueue &Q, int x) {
    if (FullQueue(Q)) {
        return false;  // 队满
    }
    Q.data[Q.rear] = x;
    Q.rear = (Q.rear + 1) % Maxsize;
    return true;
}

// 出队：取出队头元素，队头指针后移一格（取模实现循环）
bool DeQueue(SqQueue &Q, int &x) {
    if (EmptyQueue(Q)) {
        return false;  // 队空
    }
    x = Q.data[Q.front];
    Q.front = (Q.front + 1) % Maxsize;
    return true;
}

// 读取队头元素（不出队）
bool GetHead(SqQueue Q, int &x) {
    if (EmptyQueue(Q)) {
        return false;
    }
    x = Q.data[Q.front];
    return true;
}

// 求队列长度：rear 可能“绕”到 front 前面，所以要加 Maxsize 后再取模
int QueueLength(SqQueue Q) {
    return (Q.rear - Q.front + Maxsize) % Maxsize;
}

// 打印队列（从队头到队尾）
void PrintQueue(SqQueue Q) {
    printf("队列(长度=%d)：队头 -> ", QueueLength(Q));
    for (int i = Q.front; i != Q.rear; i = (i + 1) % Maxsize) {
        printf("%d ", Q.data[i]);
    }
    printf("<- 队尾\n");
}

int main() {
    SqQueue Q;
    int x;
    InitQueue(Q);
    printf("初始化后是否为空：%s\n", EmptyQueue(Q) ? "是" : "否");

    for (int i = 1; i <= 5; i++) {  // 依次入队 10 20 30 40 50
        EnQueue(Q, i * 10);
    }
    PrintQueue(Q);

    if (GetHead(Q, x)) {
        printf("队头元素：%d\n", x);
    }
    if (DeQueue(Q, x)) {
        printf("出队元素：%d\n", x);
    }
    EnQueue(Q, 60);  // 出队后再入队，验证循环利用数组空间
    PrintQueue(Q);

    while (!EmptyQueue(Q)) {  // 全部出队
        DeQueue(Q, x);
    }
    printf("全部出队后是否为空：%s，空队列出队返回：%s\n",
           EmptyQueue(Q) ? "是" : "否", DeQueue(Q, x) ? "成功" : "失败");

    DestroyQueue(Q);
    return 0;
}
