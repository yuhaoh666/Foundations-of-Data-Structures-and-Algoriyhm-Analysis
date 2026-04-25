#include <iostream>
#include <vector>
using namespace std;

// 基础插入排序
// 空间复杂度 S(n) = O(1)，时间复杂度 T(n) = O(n²)（最坏/平均），最好 O(n)
void InsertSort(int A[], int n) {
    int i, j, temp;
    // 从第2个元素开始（下标1），逐个向前插入
    for (i = 1; i < n; i++) {
        // 若当前元素比前一个小，需要插入到前面有序区间
        if (A[i] < A[i-1]) {
            temp = A[i]; // 暂存当前元素
            // 向前遍历有序区间，比temp大的元素后移
            for (j = i-1; A[j] > temp && j >= 0; j--) {
                A[j+1] = A[j];
            }
            A[j+1] = temp; // 插入到正确位置
        }
    }
}

// 带哨兵的插入排序（减少j>=0的边界判断）
// 注意：数组A[0]作为哨兵位，有效元素从A[1]到A[n]，因此数组长度需至少为n+1
void InsertSort2(int A[], int n) {
    int i, j;
    // 从第2个有效元素开始（下标2）
    for (i = 2; i <= n; i++) {
        if (A[i] < A[i-1]) {
            A[0] = A[i]; // 哨兵位暂存当前元素
            // 无需判断j>=0，因为A[0] = temp，A[j] > A[0] 最终会终止
            for (j = i-1; A[j] > A[0]; j--) {
                A[j+1] = A[j];
            }
            A[j+1] = A[0]; // 插入到正确位置
        }
    }
}

// 折半插入排序（优化查找插入位置的过程）
// 空间复杂度 O(1)，时间复杂度 比较次数O(nlogn)、移动次数O(n²)，整体仍为O(n²)
void BinaryInsertSort(int A[], int n) {
    int i, j, low, high, mid, temp;
    // 从第2个元素开始插入
    for (i = 1; i < n; i++) {
        temp = A[i]; // 暂存待插入元素
        low = 0;     // 有序区间左边界
        high = i-1;  // 有序区间右边界

        // 二分查找：找到temp的插入位置
        while (low <= high) {
            mid = (low + high) / 2; // 中间位置
            if (A[mid] > temp) high = mid - 1; // 插入位置在左半区间
            else low = mid + 1;  // 插入位置在右半区间
        }
        // 移动元素：将high+1到i-1的元素后移一位
        for (j = i-1; j >= high+1; j--) {
            A[j+1] = A[j];
        }
        A[high+1] = temp; // 插入到正确位置
    }
}

// 辅助函数：打印数组
void PrintArray(int A[], int n) {
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    cout << endl;
}

// 测试代码
int main() {
    // 测试基础插入排序
    int arr1[] = {5, 2, 9, 3, 7, 6, 1, 8, 4};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    cout << "原数组：";
    PrintArray(arr1, n1);
    
    InsertSort(arr1, n1);
    cout << "基础插入排序后：";
    PrintArray(arr1, n1);

    // 测试带哨兵的插入排序（注意数组首元素为哨兵位，有效元素从下标1开始）
    int arr2[] = {0, 5, 2, 9, 3, 7, 6, 1, 8, 4}; // A[0]是哨兵
    int n2 = sizeof(arr2) / sizeof(arr2[0]) - 1; // 有效元素个数为9
    InsertSort2(arr2, n2);
    cout << "带哨兵插入排序后：";
    PrintArray(arr2+1, n2); // 跳过哨兵位，打印有效元素

    // 测试折半插入排序
    int arr3[] = {5, 2, 9, 3, 7, 6, 1, 8, 4};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    BinaryInsertSort(arr3, n3);
    cout << "折半插入排序后：";
    PrintArray(arr3, n3);

    return 0;
}