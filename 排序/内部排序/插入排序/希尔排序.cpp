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
}
// 时间复杂度与增量序列的选取有关：最坏 O(n²)，平均约为 O(n^1.3)；空间复杂度 O(1)
// 不稳定：相同关键字的元素可能被分到不同的子序列中而改变相对次序
// 只适用于顺序存储（需要按下标随机访问），不适用于链式存储

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