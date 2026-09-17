#include <stdio.h>

// 快速排序：基于分治（选取基准 -> 划分 -> 分别递归处理左右两部分）
// 平均时间复杂度 O(n log n)，最坏情况（例如每次划分都极不平衡，如原序列已基本有序）O(n²)
// 空间复杂度 O(log n)（递归工作栈的深度），不稳定
// 平均性能是内部排序中最好的，因此实际中最常用（常配合三数取中、小数组改用插入排序等优化）

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

// 三数取中：选左、中、右中位数为基准，交换到low位置
int Median3(int A[], int low, int high) {
    int mid = low + (high - low) / 2;
    // 调整使 A[low] <= A[mid] <= A[high]
    if (A[low] > A[mid]) {
        int temp = A[low];
        A[low] = A[mid];
        A[mid] = temp;
    }
    if (A[low] > A[high]) {
        int temp = A[low];
        A[low] = A[high];
        A[high] = temp;
    }
    if (A[mid] > A[high]) {
        int temp = A[mid];
        A[mid] = A[high];
        A[high] = temp;
    }
    // 中位数交换到low作为基准
    int temp = A[mid];
    A[mid] = A[low];
    A[low] = temp;
    return A[low];
}

// 优化划分函数
int Partition2(int A[], int low, int high) {
    int pivot = Median3(A, low, high);
    while (low < high) {
        while (low < high && A[high] >= pivot) high--;
        A[low] = A[high];
        while (low < high && A[low] <= pivot) low++;
        A[high] = A[low];
    }
    A[low] = pivot;
    return low;
}

// 快速排序主逻辑
void QuickSort2(int A[], int low, int high) {
    if (low < high) {
        int pivotPos = Partition2(A, low, high);
        QuickSort2(A, low, pivotPos - 1);
        QuickSort2(A, pivotPos + 1, high);
    }
}

// 测试
int main() {
    int arr[] = {5, 2, 9, 1, 5, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("排序前：");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    
    QuickSort(arr, 0, n - 1);
    
    printf("\n排序后：");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);

    QuickSort2(arr, 0, n - 1);

    printf("\n排序后：");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
    
    return 0;
}




