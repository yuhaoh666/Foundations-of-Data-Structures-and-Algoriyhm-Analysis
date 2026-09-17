// 折半查找的变形：当表中有重复关键字时，怎样定位“第一个/最后一个”等于 x 的位置
//   1. LowerBound：第一个 ≥ x 的下标（所有元素都小于 x 时返回 n）
//   2. UpperBound：第一个 > x 的下标（所有元素都不大于 x 时返回 n）
//   3. 第一个等于 x 的位置 = LowerBound 的结果（再判断该位置上的值是否真的等于 x）
//   4. 最后一个等于 x 的位置 = UpperBound 的结果减 1（同样要判断）
//   5. x 出现的次数 = UpperBound - LowerBound
// 这几个函数正是 C++ STL 中 lower_bound / upper_bound 的原理，也是二分查找最容易写错的地方
// 写好它们的关键是“区间不变式”：始终保证答案落在当前区间内，
// 循环结束时区间收缩为一个点，那个点就是答案；这样既不会死循环，也不会漏解
#include <stdio.h>
#define MAXSIZE 100

// 打印数组
void PrintArray(const int a[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

// 1. LowerBound：第一个 ≥ x 的下标
// 采用半开区间 [low, high)，保证“答案一定在 [low, high] 中”
int LowerBound(const int a[], int n, int x) {
    int low = 0, high = n;  // 区间为空时答案就是 n（表示“不存在”）
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (a[mid] >= x) {
            high = mid;  // mid 自身可能就是答案，不能丢掉
        } else {
            low = mid + 1;  // a[mid] < x，答案一定在 mid 右边
        }
    }
    return low;
}

// 2. UpperBound：第一个 > x 的下标
int UpperBound(const int a[], int n, int x) {
    int low = 0, high = n;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (a[mid] > x) {
            high = mid;
        } else {
            low = mid + 1;  // a[mid] <= x，答案只能在 mid 右边
        }
    }
    return low;
}

// 3. 第一个等于 x 的下标，不存在返回 -1
int FirstEqual(const int a[], int n, int x) {
    int p = LowerBound(a, n, x);
    return (p < n && a[p] == x) ? p : -1;
}

// 4. 最后一个等于 x 的下标，不存在返回 -1
int LastEqual(const int a[], int n, int x) {
    int p = UpperBound(a, n, x) - 1;
    return (p >= 0 && a[p] == x) ? p : -1;
}

// 5. 统计 x 在有序数组中出现的次数
int CountEqual(const int a[], int n, int x) {
    return UpperBound(a, n, x) - LowerBound(a, n, x);
}

// 6. 循环有序（旋转）数组中的查找：形如 {4,5,6,7,0,1,2}，它是某个有序数组左移若干位得到的
// 思路：mid 把区间分成两半，其中至少有一半是有序的，先判断目标是否落在有序的那一半里
int SearchRotated(const int a[], int n, int x) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (a[mid] == x) {
            return mid;
        }
        if (a[low] <= a[mid]) {                    // 左半段 [low, mid] 有序
            if (x >= a[low] && x < a[mid]) {
                high = mid - 1;                    // 目标在左半段
            } else {
                low = mid + 1;                     // 否则只能去右半段找
            }
        } else {                                   // 右半段 [mid, high] 有序
            if (x > a[mid] && x <= a[high]) {
                low = mid + 1;                     // 目标在右半段
            } else {
                high = mid - 1;
            }
        }
    }
    return -1;
}

// 7. 用二分法求整数 x 的平方根（只取整数部分），说明二分不仅能用于查找
int MySqrt(int x) {
    if (x < 2) {
        return x;
    }
    int low = 1, high = x / 2, ans = 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if ((long long)mid * mid <= x) {  // 用 long long 防止 mid*mid 溢出
            ans = mid;                    // mid 满足条件，先记下来再去右半边试更大的
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

// 打印某个 x 的上下界信息
void PrintBound(const int a[], int n, int x) {
    int lb = LowerBound(a, n, x);
    int ub = UpperBound(a, n, x);
    printf("  x = %-3d：LowerBound = %-3d", x, lb);
    if (lb == n) {
        printf("（不存在 ≥ x 的元素）");
    } else {
        printf("（其值为 %d）", a[lb]);
    }
    printf("，UpperBound = %-3d", ub);
    if (ub == n) {
        printf("（不存在 > x 的元素）");
    } else {
        printf("（其值为 %d）", a[ub]);
    }
    printf("，x 共出现 %d 次\n", CountEqual(a, n, x));
}

int main() {
    // 1. 含有重复元素的有序数组，演示上下界与“第一个/最后一个”位置
    int a[] = {1, 2, 2, 2, 3, 5, 5, 7, 9, 9, 9};
    int n = sizeof(a) / sizeof(a[0]);
    printf("有序数组（含重复元素）：");
    PrintArray(a, n);

    printf("\n=== 上下界查询 ===\n");
    int keys[] = {2, 5, 9, 4, 0, 10};
    for (int i = 0; i < 6; i++) {
        PrintBound(a, n, keys[i]);
    }

    printf("\n=== 第一个与最后一个等于 x 的位置 ===\n");
    for (int i = 0; i < 6; i++) {
        printf("  x = %-3d：第一个等于 x 的下标 = %-3d，最后一个等于 x 的下标 = %-3d\n",
               keys[i], FirstEqual(a, n, keys[i]), LastEqual(a, n, keys[i]));
    }

    // 2. 循环有序（旋转）数组中的查找
    int r[] = {4, 5, 6, 7, 0, 1, 2};
    int rn = sizeof(r) / sizeof(r[0]);
    printf("\n=== 循环有序（旋转）数组中的查找 ===\n数组：");
    PrintArray(r, rn);
    int rk[] = {0, 4, 2, 3, 8};
    for (int i = 0; i < 5; i++) {
        int pos = SearchRotated(r, rn, rk[i]);
        printf("  查找 %-3d：%s", rk[i], pos == -1 ? "未找到" : "找到");
        if (pos != -1) {
            printf("（下标 %d）", pos);
        }
        printf("\n");
    }

    // 3. 二分的另一种用途：求整数平方根
    printf("\n=== 用二分法求平方根（整数部分）===\n");
    int nums[] = {0, 1, 8, 16, 17, 2147395599};
    for (int i = 0; i < 6; i++) {
        printf("  ⌊√%-10d⌋ = %d\n", nums[i], MySqrt(nums[i]));
    }

    printf("\n结论：二分查找的变形很多，只要保持“答案始终在区间内”这一不变式就不会写错；\n");
    printf("      LowerBound / UpperBound 是处理“有重复元素”的通用工具，\n");
    printf("      它们的比较次数同样是 O(log n)，而且可以直接算出区间长度、出现次数。\n");
    return 0;
}
