// 大根堆的插入与删除
// 堆是用数组存储的完全二叉树：下标 i 的父结点为 (i-1)/2，左孩子为 2i+1，右孩子为 2i+2
//（这里统一采用下标从 0 开始的写法；若按教材下标从 1 开始，则父结点为 i/2、孩子为 2i 与 2i+1）
// 插入：新元素放到数组末尾，然后“上浮”到合适位置，O(log n)
// 删除：用末尾元素顶替被删元素，然后从该位置“下沉”调整，O(log n)
#include <stdio.h>
#define MAXSIZE 100

// 向下调整（堆化）：把下标 i 处的元素向下沉到合适位置，维持大根堆性质
void Heapify(int A[], int len, int i) {
    while (true) {
        int largest = i;
        int left = 2 * i + 1;   // 左孩子
        int right = 2 * i + 2;  // 右孩子
        if (left < len && A[left] > A[largest]) {
            largest = left;
        }
        if (right < len && A[right] > A[largest]) {
            largest = right;
        }
        if (largest == i) {  // 父结点已经不小于两个孩子，调整结束
            break;
        }
        int temp = A[i];
        A[i] = A[largest];
        A[largest] = temp;
        i = largest;  // 继续沿着被交换的那条路向下调整
    }
}

// 建立大根堆：从最后一个非叶结点开始自底向上调整，时间复杂度 O(n)
void BuildMaxHeap(int A[], int len) {
    for (int i = len / 2 - 1; i >= 0; i--) {
        Heapify(A, len, i);
    }
}

// 插入：把 key 插入大根堆（len 为当前元素个数），返回插入后的元素个数
int HeapInsert(int A[], int len, int key) {
    if (len >= MAXSIZE) {
        printf("堆已满，插入失败！\n");
        return len;
    }
    int i = len;    // 新元素先放到数组末尾
    A[i] = key;
    while (i > 0 && A[(i - 1) / 2] < A[i]) {  // 比父结点大就与父结点交换（上浮）
        int temp = A[i];
        A[i] = A[(i - 1) / 2];
        A[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
    return len + 1;
}

// 删除：删除下标为 index 的元素，用末尾元素顶替并向下调整
// len 传引用，删除成功后元素个数减一；返回被删除的关键字，出错返回 -1
int HeapDelete(int A[], int &len, int index) {
    if (len <= 0 || index < 0 || index >= len) {  // 先做合法性检查，再访问数组
        printf("删除位置 %d 不合法！\n", index);
        return -1;
    }
    int deleted = A[index];     // 记录被删除的关键字
    A[index] = A[len - 1];      // 用末尾元素填补空缺
    len--;                      // 元素个数减一
    Heapify(A, len, index);     // 从 index 处向下调整
    return deleted;
}

// 打印堆（按数组顺序，即完全二叉树的层序）
void PrintHeap(int A[], int len) {
    printf("堆(元素个数=%d)：", len);
    for (int i = 0; i < len; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main() {
    int heap[MAXSIZE];
    int len = 0;

    // 1. 依次插入若干元素，观察每次插入后的堆
    int keys[] = {3, 1, 5, 4, 2, 8, 7};
    for (int i = 0; i < 7; i++) {
        len = HeapInsert(heap, len, keys[i]);
        printf("插入 %d 后，", keys[i]);
        PrintHeap(heap, len);
    }

    // 2. 删除中间某个元素
    int deleted = HeapDelete(heap, len, 2);
    printf("删除下标 2 的元素 %d 后，", deleted);
    PrintHeap(heap, len);

    // 3. 删除堆顶元素（最大值）
    deleted = HeapDelete(heap, len, 0);
    printf("删除堆顶元素 %d 后，", deleted);
    PrintHeap(heap, len);

    // 4. 非法删除会被拦截
    printf("删除非法下标的返回值为 %d\n", HeapDelete(heap, len, 99));

    // 5. 对比：也可以用建堆的方式直接让一个数组变成大根堆（O(n)）
    int B[] = {3, 1, 5, 4, 2, 8, 7};
    BuildMaxHeap(B, 7);
    printf("对数组 {3,1,5,4,2,8,7} 直接建大根堆：");
    PrintHeap(B, 7);
    return 0;
}
