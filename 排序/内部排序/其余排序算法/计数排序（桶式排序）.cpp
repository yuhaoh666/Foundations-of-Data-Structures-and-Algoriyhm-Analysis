// 计数排序（又称桶式排序）：不比较关键字的大小，而是统计每个关键字出现的次数
// 适用条件：关键字是整数且取值范围较小（k 不太大），例如成绩、年龄
// 时间复杂度 O(n+k)，空间复杂度 O(n+k)，是稳定排序：
//   因为最后一步“从后往前”放置元素，相同关键字的相对次序不会改变
#include <stdio.h>
#include <stdlib.h>

// 对 A 做计数排序，结果写入 B；n 为元素个数，k 为关键字取值上限（关键字范围是 0 ~ k-1）
void CountSort(int A[], int B[], int n, int k) {
    int *C = (int *)malloc(k * sizeof(int));  // 辅助数组，动态申请以免数组过大
    if (C == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < k; i++) {
        C[i] = 0;
    }
    for (int i = 0; i < n; i++) {          // Step1：统计每个关键字的出现次数
        if (A[i] < 0 || A[i] >= k) {       // 关键字超出范围时直接报错，避免数组越界
            printf("元素 %d 不在 [0, %d) 范围内，计数排序失败！\n", A[i], k);
            free(C);
            return;
        }
        C[A[i]]++;
    }
    for (int i = 1; i < k; i++) {          // Step2：前缀和，C[i] 变成“不大于 i 的元素个数”
        C[i] = C[i] + C[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {     // Step3：从后往前放置元素，保证排序的稳定性
        C[A[i]] = C[A[i]] - 1;
        B[C[A[i]]] = A[i];                 // 该元素在结果数组中的正确位置
    }
    free(C);
}

// 就地版的计数排序：自动求出最小/最大值，支持负数（用偏移量把关键字映射到下标）
void CountingSortInPlace(int A[], int n) {
    if (n <= 0) {
        return;
    }
    int max = A[0], min = A[0];
    for (int i = 1; i < n; i++) {  // 1. 找最小值和最大值
        if (A[i] > max) {
            max = A[i];
        }
        if (A[i] < min) {
            min = A[i];
        }
    }
    int range = max - min + 1;     // 2. 关键字的取值范围
    int *C = (int *)malloc(range * sizeof(int));
    int *B = (int *)malloc(n * sizeof(int));
    if (C == NULL || B == NULL) {
        printf("内存分配失败！\n");
        free(C);
        free(B);
        return;
    }
    for (int i = 0; i < range; i++) {
        C[i] = 0;
    }
    for (int i = 0; i < n; i++) {          // 3. 统计次数（减去 min 以兼容负数）
        C[A[i] - min]++;
    }
    for (int i = 1; i < range; i++) {      // 4. 前缀和
        C[i] += C[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {     // 5. 从后往前写入临时数组，保持稳定
        int idx = A[i] - min;
        C[idx]--;
        B[C[idx]] = A[i];
    }
    for (int i = 0; i < n; i++) {          // 6. 复制回原数组
        A[i] = B[i];
    }
    free(C);
    free(B);
}

// 打印数组
void PrintArray(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main() {
    int A[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    int n = sizeof(A) / sizeof(A[0]);
    int k = 10;                 // 关键字范围 0 ~ 9
    int B[sizeof(A) / sizeof(A[0])];

    CountSort(A, B, n, k);
    printf("原数组：");
    PrintArray(A, n);
    printf("计数排序（结果存入辅助数组）：");
    PrintArray(B, n);

    // 就地版本：支持负数与任意范围
    int C[] = {-3, 5, -1, 0, 5, -3, 2, 100, -50};
    int m = sizeof(C) / sizeof(C[0]);
    printf("\n就地版本排序前：");
    PrintArray(C, m);
    CountingSortInPlace(C, m);
    printf("就地版本排序后：");
    PrintArray(C, m);

    // 关键字超出范围时的保护
    int bad[] = {1, 2, 20};
    CountSort(bad, B, 3, k);
    return 0;
}
