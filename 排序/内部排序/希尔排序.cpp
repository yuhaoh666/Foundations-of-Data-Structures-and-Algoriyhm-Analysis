#include <stdio.h>

void ShellSort(int A[], int n) {
    int d, i, j, temp;
    // 步长d初始化为数组长度的一半，逐步折半缩小
    for (d = n / 2; d >= 1; d /= 2) {
        // 对每个步长为d的子序列进行直接插入排序
        for (i = d; i < n; i++) {
            // 暂存当前待插入的元素
            temp = A[i];
            // 向前遍历子序列，找到插入位置
            for (j = i - d; j >= 0 && A[j] > temp; j -= d) {
                // 元素后移，为插入temp腾出位置
                A[j + d] = A[j];
            }
            // 将temp插入到正确位置
            A[j + d] = temp;
        }
    }
} // 不具有稳定性，且适用于顺序表而不适用于链表

// 测试函数
int main() {
    int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("排序前数组：");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    ShellSort(arr, n);
    
    printf("排序后数组：");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}