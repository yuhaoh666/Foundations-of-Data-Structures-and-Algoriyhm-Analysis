#include <stdio.h>
#include <stdlib.h>

// 归并排序（2 路归并）：先递归地把序列一分为二，再把两个有序子序列合并成一个
// 时间复杂度 O(n log n)（最好、最坏、平均都相同）；空间复杂度 O(n)（需要辅助数组）
// 是稳定排序：合并时先取左边子序列的元素，相同关键字的相对次序不会改变
// 与快速排序相比性能更稳定，但需要额外的 O(n) 空间

// 全局辅助数组（归并排序专用）
int *B;

// 合并：将A[low..mid]和A[mid+1..high]合并为有序序列
void Merge(int A[], int low, int mid, int high) {
    // 把A的待合并区间复制到辅助数组B
    for (int i = low; i <= high; i++) 
        B[i] = A[i];
    // i：左序列起始；j：右序列起始；k：原数组填充位置
    int i = low, j = mid + 1, k = low;
    // 归并两个有序子序列
    while (i <= mid && j <= high) {
        if (B[i] <= B[j]) 
            A[k++] = B[i++];
        else 
            A[k++] = B[j++];
        
    }
    // 复制左序列剩余元素
    while (i <= mid) {
        A[k++] = B[i++];
    }
    // 复制右序列剩余元素
    while (j <= high) {
        A[k++] = B[j++];
    }
}

// 归并排序（递归分治）
void MergeSort(int A[], int low, int high) {
    if (low < high) {
        // 取中间点，分治
        int mid = (low + high) / 2;
        // 递归排序左半
        MergeSort(A, low, mid);
        // 递归排序右半
        MergeSort(A, mid + 1, high);
        // 合并两个有序子数组
        Merge(A, low, mid, high);
    }
}

// 测试主函数
int main() {
    // 待排序数组
    int A[] = {5, 2, 7, 3, 9, 1, 6, 4, 8};
    // 数组长度
    int n = sizeof(A) / sizeof(A[0]);

    // 动态分配辅助数组（修复原代码n未定义的错误）
    B = (int *)malloc(n * sizeof(int));

    // 归并排序
    MergeSort(A, 0, n - 1);

    // 输出排序结果
    printf("排序后：");
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    // 释放动态内存
    free(B);
    return 0;
}