// 基数排序（最低位优先 LSD）：不比较关键字大小，而是按“位”进行多次“分配 - 收集”
// 每趟按某一位的数字把元素分配到 r 个队列中，再按队列顺序收集回来；
// 一共需要 d 趟（d 为关键字的最大位数），时间复杂度 O(d(n+r))，空间复杂度 O(r + n)
// 属于稳定排序（要求每趟的分配与收集都保持稳定），通常用链式队列实现
// 说明：这里只处理非负整数
#include <stdio.h>
#include <stdlib.h>

// ==================== 链式队列（用于“分配 - 收集”） ====================
typedef struct LiQueueNode {
    int data;                  // 数据域
    struct LiQueueNode *next;  // 指针域
} LiQueueNode;

typedef struct {
    LiQueueNode *front;  // 队头指针（指向头结点）
    LiQueueNode *rear;   // 队尾指针
} LiQueue;

// 初始化队列：创建头结点
void InitQueue(LiQueue &Q) {
    Q.front = Q.rear = (LiQueueNode *)malloc(sizeof(LiQueueNode));
    if (Q.front == NULL) {
        printf("内存分配失败！\n");
        exit(-1);
    }
    Q.front->next = NULL;
}

// 销毁队列
void DestroyQueue(LiQueue &Q) {
    while (Q.front != NULL) {
        LiQueueNode *temp = Q.front;
        Q.front = Q.front->next;
        free(temp);
    }
    Q.rear = NULL;
}

// 判空
bool EmptyQueue(LiQueue Q) {
    return Q.front == Q.rear;
}

// 入队
bool EnQueue(LiQueue &Q, int x) {
    LiQueueNode *newNode = (LiQueueNode *)malloc(sizeof(LiQueueNode));
    if (newNode == NULL) {
        return false;
    }
    newNode->data = x;
    newNode->next = NULL;
    Q.rear->next = newNode;
    Q.rear = newNode;
    return true;
}

// 出队
bool DeQueue(LiQueue &Q, int &x) {
    if (EmptyQueue(Q)) {
        return false;
    }
    LiQueueNode *temp = Q.front->next;
    x = temp->data;
    Q.front->next = temp->next;
    if (Q.rear == temp) {  // 出队的恰好是最后一个元素
        Q.rear = Q.front;
    }
    free(temp);
    return true;
}

// ==================== 基数排序 ====================
// 取 num 在 r 进制下的第 d 位（d 从 1 开始，最低位是第 1 位）
int GetDigit(int num, int d, int r) {
    int base = 1;  // base 最终等于 r^(d-1)
    for (int i = 1; i < d; i++) {
        base *= r;
    }
    return (num / base) % r;
}

// 求数组中最大元素在 r 进制下的位数，即需要多少趟分配-收集
int GetMaxBit(int arr[], int n, int r) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    int bit = 0;
    while (max > 0) {
        max /= r;
        bit++;
    }
    return bit;
}

// 打印数组
void PrintArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 对 n 个元素做 r 进制基数排序（升序）
void RadixSort(int arr[], int n, int r) {
    if (n <= 1) {
        return;
    }
    int m = GetMaxBit(arr, n, r);  // 根据最大元素算出需要多少趟
    LiQueue *queue = (LiQueue *)malloc(r * sizeof(LiQueue));  // r 个队列对应 r 个数字
    if (queue == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < r; i++) {
        InitQueue(queue[i]);
    }

    for (int d = 1; d <= m; d++) {  // 从最低位到最高位，逐位排序（共 m 趟）
        // 1. 分配：按当前位上的数字把元素放入对应的队列
        for (int i = 0; i < n; i++) {
            int digit = GetDigit(arr[i], d, r);
            EnQueue(queue[digit], arr[i]);
        }
        // 2. 收集：按 0 ~ r-1 的顺序依次出队写回数组（这一步保证了稳定性）
        int index = 0;
        for (int i = 0; i < r; i++) {
            while (!EmptyQueue(queue[i])) {
                DeQueue(queue[i], arr[index++]);
            }
        }
        printf("第 %d 趟（按 %d 进制第 %d 位）分配收集后：", d, r, d);
        PrintArray(arr, n);
    }

    for (int i = 0; i < r; i++) {
        DestroyQueue(queue[i]);
    }
    free(queue);
}

int main() {
    // 8 个十进制整数，最大数是 789，共 3 位 → 需要 3 趟
    int arr[] = {256, 123, 45, 789, 32, 678, 9, 505};
    int n = sizeof(arr) / sizeof(arr[0]);
    int r = 10;  // 十进制

    printf("排序前：");
    PrintArray(arr, n);
    RadixSort(arr, n, r);
    printf("排序后：");
    PrintArray(arr, n);

    // 对比：换成二进制（r = 2）依然正确，只是趟数更多
    int brr[] = {256, 123, 45, 789, 32, 678, 9, 505};
    printf("\n用二进制基数排序（r = 2，趟数 = %d）：\n", GetMaxBit(brr, n, 2));
    RadixSort(brr, n, 2);
    printf("排序后：");
    PrintArray(brr, n);
    return 0;
}
