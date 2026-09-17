#include <stdio.h>

// 将以 k 为根的子树调整为大根堆
// 说明：本文件采用下标从 0 开始的写法（父结点为 (i-1)/2，左右孩子为 2i+1、2i+2）；
//      若按教材从下标 1 开始存储，则父结点为 i/2，左右孩子为 2i、2i+1
void HeapAdjust(int A[], int k, int len) {
    // 暂存根节点
    int temp = A[k];
    // 沿key较大的子节点向下筛选(i为左孩子)
    for (int i = 2 * k + 1; i < len; i = 2 * i + 1) {
        // 取左右孩子中较大的下标
        if (i + 1 < len && A[i] < A[i + 1]) {
            i++;
        }
        // 根节点大于等于较大孩子,无需调整
        if (temp >= A[i]) {
            break;
        }
        // 将较大孩子上移
        A[k] = A[i];
        // 修改k值,继续向下筛选
        k = i;
    }
    // 插入正确位置
    A[k] = temp;
}

// 建立大根堆
void BuildMaxHeap(int A[], int len) {
    // 从最后一个非叶子节点开始,倒序调整
    for (int i = len / 2 - 1; i >= 0; i--) {
        HeapAdjust(A, i, len);
    }
}

// 堆排序核心函数
void HeapSort(int A[], int len) {
    // 构建大根堆
    BuildMaxHeap(A, len);
    // 交换堆顶与末尾元素,重新调整堆
    for (int i = len - 1; i > 0; i--) {
        // 交换堆顶和当前末尾元素
        int temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        // 调整剩余元素为大根堆
        HeapAdjust(A, 0, i);
    }
}
// 时间复杂度：建堆 O(n) + (n-1) 次调整 × O(log n) = O(n log n)（最好、最坏、平均相同）
// 空间复杂度 O(1)：只在原数组上交换，属于原地排序
// 不稳定：堆顶与末尾元素交换时可能改变相同关键字的相对次序
// 适合从海量数据中选出前 k 个最大/最小值（只需建一个大小为 k 的堆）

// 打印数组
void PrintArray(int A[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

// 主函数测试
int main() {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int len = sizeof(arr) / sizeof(arr[0]);
    
    printf("排序前：");
    PrintArray(arr, len);
    
    HeapSort(arr, len);
    
    printf("排序后：");
    PrintArray(arr, len);
    
    return 0;
}