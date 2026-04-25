#include <stdio.h>

// 划分函数：将数组分为小于基准和大于基准两部分，返回基准位置
int Partition(int A[], int low, int high) {
    // 选取第一个元素作为基准值
    int pivot = A[low];
    while (low < high) {
        // 从右向左找第一个小于基准的元素
        while (low < high && A[high] >= pivot) high--;
        A[low] = A[high];  // 将该元素移到左侧
        // 从左向右找第一个大于基准的元素
        while (low < high && A[low] <= pivot) low++;
        A[high] = A[low];  // 将该元素移到右侧
    }
    A[low] = pivot;  // 基准元素归位
    return low;     // 返回基准位置
}

// 快速排序递归函数
void QuickSort(int A[], int low, int high) {
    if (low < high) {
        // 划分得到基准位置
        int pivotPos = Partition(A, low, high);
        // 递归排序左子数组
        QuickSort(A, low, pivotPos - 1);
        // 递归排序右子数组
        QuickSort(A, pivotPos + 1, high);
    }
}

// 测试主函数
int main() {
    int arr[] = {5, 2, 9, 1, 5, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("排序前：");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    
    QuickSort(arr, 0, n - 1);
    
    printf("\n排序后：");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    return 0;
}