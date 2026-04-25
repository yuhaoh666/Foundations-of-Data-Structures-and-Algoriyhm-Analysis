// 顺序队列的实现

#include <stdio.h>
#define Maxsize 100

typedef struct {
    int data[Maxsize];  // 存储空间
    int front;  // 队头指针
    int rear;  // 队尾指针
} SqQueue;

void InitQueue(SqQueue &Q) {
    Q.rear = Q.front = 0;  
}

void DestroyQueue(SqQueue &Q) {
    Q.front = 0;  // 销毁队列，重置队头指针
    Q.rear = 0;  // 销毁队列，重置队尾指针
}

bool EmptyQueue(SqQueue Q) {
    return Q.front == Q.rear;  // 判断队列是否为空
}

bool FullQueue(SqQueue Q) {
    return (Q.rear + 1) % Maxsize == Q.front;  // 判断队列是否已满，事实上牺牲一个存储空间来区分队列满和队列空的情况，如果要利用所有存储空间，可以使用一个计数器size来记录队列中的元素个数(此时要重写DestroyQueue)，或者使用一个标志位tag来区分队列满和队列空的情况。
}

bool EnQueue(SqQueue &Q, int x) {
    if (FullQueue(Q)) {  // 判断队列是否已满
        return false;  // 队列满，无法入队
    }
    Q.data[Q.rear] = x;  // 将元素入队
    Q.rear = (Q.rear + 1) % Maxsize;  // 更新队尾指针，循环使用数组空间
    return true;
}

bool DeQueue(SqQueue &Q, int &x) {
    if (EmptyQueue(Q)) {  // 判断队列是否为空
        return false;  // 队列空，无法出队
    }
    x = Q.data[Q.front];  // 将队头元素出队
    Q.front = (Q.front + 1) % Maxsize;  // 更新队头指针，循环使用数组空间
    return true;
}

bool GetHead(SqQueue Q, int &x) {
    if (EmptyQueue(Q)) {  // 判断队列是否为空
        return false;  // 队列空，无法获取队头元素
    }
    x = Q.data[Q.front];  // 获取队头元素
    return true;
}

int QueueLength(SqQueue Q) {
    return (Q.rear - Q.front + Maxsize) % Maxsize;  // 计算队列长度
}

int main() {
    SqQueue Q;
    InitQueue(Q);
    return 0;
}
