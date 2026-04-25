#include <stdio.h>

// 冒泡排序实现：升序排列
void BubbleSort(int A[], int n) {
    // 外层循环控制排序的趟数，最多需要n-1趟
    for (int i = 0; i < n - 1; i++) {
        // 标记当前趟是否发生交换，初始为false（未交换）
        int swapped = 0;
        // 内层循环进行相邻元素比较交换，每趟结束后最大元素已"冒泡"到末尾
        // 优化：每趟后末尾i个元素已有序，无需再比较
        for (int j = 0; j < n - 1 - i; j++) {
            // 前一个元素大于后一个，交换位置（升序）
            if (A[j] > A[j + 1]) {
                int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
                swapped = 1; // 标记发生了交换
            }
        }
        // 如果当前趟没有发生任何交换，说明数组已有序，提前退出
        if (!swapped) {
            break;
        }
    }
} // 空间复杂度O(1), 时间复杂度O(n**2)，最好时间复杂度O(n)，可以用于链表，链表一般是从前往后冒

// 辅助函数：打印数组
void PrintArray(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

// 测试用例
int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("排序前的数组：");
    PrintArray(arr, n);
    
    BubbleSort(arr, n);
    
    printf("排序后的数组：");
    PrintArray(arr, n);
    
    return 0;
}