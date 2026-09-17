// 二叉排序树（又称二叉查找树 BST）：左子树上所有结点 < 根结点 < 右子树上所有结点
// 重要性质：中序遍历二叉排序树得到的一定是递增有序序列
// 查找、插入、删除的平均时间复杂度都是 O(log n)；
// 但最坏情况（插入序列本身有序时树退化成单支树）会变成 O(n)
#include <stdio.h>
#include <stdlib.h>

typedef struct BSTNode {
    int key;                  // 关键字
    struct BSTNode *lchild, *rchild;  // 左右孩子
} BSTNode, *BSTree;

// 初始化：空树用 NULL 表示
void InitBST(BSTree *T) {
    *T = NULL;
}

// 查找（非递归）：找到返回结点指针，未找到返回 NULL
BSTNode *BST_Search(BSTree T, int key) {
    while (T != NULL && key != T->key) {  // 树空或等于根结点值则结束循环
        if (key < T->key) {
            T = T->lchild;  // 小于根，去左子树找
        } else {
            T = T->rchild;  // 大于根，去右子树找
        }
    }
    return T;
}

// 查找（递归）：找到返回 true 并用 p 带回结点指针
bool BST_SearchRec(BSTree T, int key, BSTNode **p) {
    if (T == NULL) {
        *p = NULL;
        return false;
    }
    if (key == T->key) {
        *p = T;
        return true;
    }
    if (key < T->key) {
        return BST_SearchRec(T->lchild, key, p);
    }
    return BST_SearchRec(T->rchild, key, p);
}

// 插入（递归）：关键字已存在时插入失败（二叉排序树中不允许重复关键字）
bool BST_Insert(BSTree &T, int key) {
    if (T == NULL) {  // 找到了空位置，在此创建新结点
        T = (BSTNode *)malloc(sizeof(BSTNode));
        if (T == NULL) {
            printf("内存分配失败！\n");
            return false;
        }
        T->key = key;
        T->lchild = T->rchild = NULL;
        return true;
    }
    if (key == T->key) {
        return false;  // 已存在相同关键字
    }
    if (key < T->key) {
        return BST_Insert(T->lchild, key);
    }
    return BST_Insert(T->rchild, key);
}

// 建立二叉排序树：把关键字序列依次插入空树
void CreateBST(BSTree &T, const int str[], int n) {
    T = NULL;
    for (int i = 0; i < n; i++) {
        BST_Insert(T, str[i]);
    }
}

// 找最小关键字结点：从根出发一路向左
BSTNode *BST_Min(BSTree T) {
    while (T != NULL && T->lchild != NULL) {
        T = T->lchild;
    }
    return T;
}

// 找最大关键字结点：从根出发一路向右
BSTNode *BST_Max(BSTree T) {
    while (T != NULL && T->rchild != NULL) {
        T = T->rchild;
    }
    return T;
}

// 删除以 p 为根的子树中关键字最小的结点，用 minKey 返回其关键字
void DeleteMin(BSTree &p, int &minKey) {
    if (p->lchild == NULL) {  // p 本身就是最小结点
        minKey = p->key;
        BSTNode *temp = p;
        p = p->rchild;  // 用它的右子树顶替它的位置
        free(temp);
    } else {
        DeleteMin(p->lchild, minKey);  // 继续到左子树中找
    }
}

// 删除关键字为 key 的结点；out 返回被删除的关键字，删除失败时 out 为 -1
bool BST_Delete(BSTree &T, int key, int &out) {
    out = -1;
    if (T == NULL) {
        return false;  // 树空或没有该关键字
    }
    if (key < T->key) {
        return BST_Delete(T->lchild, key, out);  // 到左子树删除
    }
    if (key > T->key) {
        return BST_Delete(T->rchild, key, out);  // 到右子树删除
    }
    // 找到待删除结点 T
    out = T->key;
    if (T->lchild == NULL && T->rchild == NULL) {  // 情况 1：叶子结点，直接删除
        free(T);
        T = NULL;
    } else if (T->lchild == NULL) {                // 情况 2：只有右孩子
        BSTNode *temp = T;
        T = T->rchild;                             // 用右孩子顶替
        free(temp);
    } else if (T->rchild == NULL) {                // 情况 3：只有左孩子
        BSTNode *temp = T;
        T = T->lchild;                             // 用左孩子顶替
        free(temp);
    } else {
        // 情况 4：左右孩子都有 —— 用右子树中的最小结点（中序后继）替代当前结点，
        //         再把这个最小结点删除，这样仍能保证中序序列有序
        int minKey;
        DeleteMin(T->rchild, minKey);
        T->key = minKey;
    }
    return true;
}

// 中序遍历：验证二叉排序树的有序性
void InOrder(BSTree T) {
    if (T != NULL) {
        InOrder(T->lchild);
        printf("%d ", T->key);
        InOrder(T->rchild);
    }
}

// 求树高：树越矮，查找效率越高（理想情况接近 log n）
int TreeDepth(BSTree T) {
    if (T == NULL) {
        return 0;
    }
    int l = TreeDepth(T->lchild);
    int r = TreeDepth(T->rchild);
    return (l > r ? l : r) + 1;
}

// 销毁整棵树（后序释放）
void DestroyBST(BSTree &T) {
    if (T == NULL) {
        return;
    }
    DestroyBST(T->lchild);
    DestroyBST(T->rchild);
    free(T);
    T = NULL;
}

// 执行一次删除并打印结果
void PrintDeleteResult(BSTree &T, int key) {
    int out;
    bool ok = BST_Delete(T, key, out);
    if (ok) {
        printf("删除 %d 成功，中序遍历：", out);
    } else {
        printf("删除 %d 失败（关键字不存在），中序遍历：", key);
    }
    InOrder(T);
    printf("\n");
}

int main() {
    int arr[] = {5, 3, 7, 2, 4, 6, 8};
    int n = sizeof(arr) / sizeof(arr[0]);
    BSTree T;

    CreateBST(T, arr, n);  // 按序列 {5,3,7,2,4,6,8} 建树，这是一棵比较平衡的树
    printf("构建后中序遍历（应为递增序列）：");
    InOrder(T);
    printf("\n树高：%d，最小关键字：%d，最大关键字：%d\n", TreeDepth(T), BST_Min(T)->key,
           BST_Max(T)->key);

    // 查找
    int keys[] = {4, 9};
    for (int i = 0; i < 2; i++) {
        BSTNode *p = BST_Search(T, keys[i]);
        printf("查找 %d：%s", keys[i], p != NULL ? "找到" : "未找到");
        BSTNode *q = NULL;
        printf("（递归查找结果：%s）\n", BST_SearchRec(T, keys[i], &q) ? "找到" : "未找到");
    }

    // 删除的三种情况
    PrintDeleteResult(T, 2);  // 情况 1：删除叶子结点
    PrintDeleteResult(T, 3);  // 情况 2/3：删除只有一个孩子的结点
    PrintDeleteResult(T, 5);  // 情况 4：删除有两个孩子的根结点
    PrintDeleteResult(T, 9);  // 关键字不存在

    printf("最终中序遍历：");
    InOrder(T);
    printf("\n最终树高：%d\n", TreeDepth(T));

    DestroyBST(T);
    printf("销毁后树是否为空：%s\n", T == NULL ? "是" : "否");
    return 0;
}
