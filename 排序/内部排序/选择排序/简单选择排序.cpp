// 选择排序：每一趟在待排序元素中选取关键字最小（或最大）的元素加入有序子序列
// 简单选择排序的做法：每趟从未排序区间中选出最小元素，与未排序区间的第一个元素交换
// 时间复杂度 O(n²)（比较次数固定为 n(n-1)/2，与初始序列无关），空间复杂度 O(1)
// 不稳定：交换元素时可能把相同关键字的相对次序打乱，而且不支持链式存储的高效实现
#include <stdio.h>

// 简单选择排序（升序）
void SelectSort(int A[], int n) {
    for (int i = 0; i < n - 1; i++) {  // 共进行 n-1 趟，每趟确定一个元素的位置
        int minIndex = i;              // 先假设未排序区间的第一个元素最小
        for (int j = i + 1; j < n; j++) {
            if (A[j] < A[minIndex]) {
                minIndex = j;          // 记录更小元素的下标
            }
        }
        if (minIndex != i) {  // 找到了更小的元素才交换，避免无意义的移动
            int temp = A[i];
            A[i] = A[minIndex];
            A[minIndex] = temp;
        }
        // 此时 A[0..i] 已有序
    }
}

// 辅助函数：打印数组
void PrintArray(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main() {
    int A[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n = sizeof(A) / sizeof(A[0]);

    printf("排序前：");
    PrintArray(A, n);

    SelectSort(A, n);

    printf("排序后：");
    PrintArray(A, n);
    return 0;
}
