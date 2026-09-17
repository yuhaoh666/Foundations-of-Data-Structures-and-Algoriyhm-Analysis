#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 平衡二叉树（AVL 树）：任何结点的左、右子树高度之差的绝对值都不超过 1
// 平衡因子 BF = 右子树高度 - 左子树高度，所以合法取值只能是 -1、0、1
// 插入或删除后若某结点的 |BF| > 1，就要做旋转来恢复平衡：
//   LL 型（左孩子的左子树过高）-> 对根右旋
//   RR 型（右孩子的右子树过高）-> 对根左旋
//   LR 型（左孩子的右子树过高）-> 先对左孩子左旋，再对根右旋
//   RL 型（右孩子的左子树过高）-> 先对右孩子右旋，再对根左旋
// 含 n 个结点的 AVL 树高度为 O(log n)，因此查找、插入、删除都是 O(log n)
// 本文件在结点中直接维护 height 字段，避免每次都递归求高度

// AVL树结点定义（增加height字段直接维护高度，避免重复递归计算）
typedef struct AVLNode {
    int key;
    int height;  // 结点高度（叶子结点为1）
    struct AVLNode *lchild, *rchild;
} AVLNode, *AVLTree;

// 宏定义：获取最大值
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// 辅助函数：获取结点高度（处理空结点）
static int GetHeight(AVLTree T) {
    return T == NULL ? 0 : T->height;
}

// 辅助函数：更新结点高度
static void UpdateHeight(AVLTree T) {
    if (T == NULL) return;
    T->height = MAX(GetHeight(T->lchild), GetHeight(T->rchild)) + 1;
}

// 辅助函数：计算平衡因子（右子树高度 - 左子树高度）
static int GetBalanceFactor(AVLTree T) {
    return T == NULL ? 0 : GetHeight(T->rchild) - GetHeight(T->lchild);
}

// 右旋操作 (LL型失衡调整)
static void R_Rotate(AVLTree &p) {
    if (p == NULL || p->lchild == NULL) return;

    AVLTree lc = p->lchild;
    p->lchild = lc->rchild;
    lc->rchild = p;

    // 先更新下层结点高度，再更新上层
    UpdateHeight(p);
    UpdateHeight(lc);
    p = lc;
}

// 左旋操作 (RR型失衡调整)
static void L_Rotate(AVLTree &p) {
    if (p == NULL || p->rchild == NULL) return;

    AVLTree rc = p->rchild;
    p->rchild = rc->lchild;
    rc->lchild = p;

    // 先更新下层结点高度，再更新上层
    UpdateHeight(p);
    UpdateHeight(rc);
    p = rc;
}

// LR型失衡调整 (先左旋左孩子，再右旋根)
static void LR_Rotate(AVLTree &p) {
    L_Rotate(p->lchild);
    R_Rotate(p);
}

// RL型失衡调整 (先右旋右孩子，再左旋根)
static void RL_Rotate(AVLTree &p) {
    R_Rotate(p->rchild);
    L_Rotate(p);
}

// 平衡调整函数（核心优化：基于高度直接计算平衡因子，避免重复递归）
static void Balance(AVLTree &T) {
    if (T == NULL) return;

    UpdateHeight(T);  // 先更新当前结点高度
    int balance = GetBalanceFactor(T);

    // 左子树过高 (平衡因子 < -1)
    if (balance < -1) {
        int lc_balance = GetBalanceFactor(T->lchild);
        // LL型：左孩子平衡因子 ≤ 0
        if (lc_balance <= 0) {
            R_Rotate(T);
        }
        // LR型：左孩子平衡因子 > 0
        else {
            LR_Rotate(T);
        }
    }
    // 右子树过高 (平衡因子 > 1)
    else if (balance > 1) {
        int rc_balance = GetBalanceFactor(T->rchild);
        // RR型：右孩子平衡因子 ≥ 0
        if (rc_balance >= 0) {
            L_Rotate(T);
        }
        // RL型：右孩子平衡因子 < 0
        else {
            RL_Rotate(T);
        }
    }
}

// 查找值为 key 的结点（非递归，效率更高）
AVLNode *AVL_Search(AVLTree T, int key) {
    while (T != NULL) {
        if (key == T->key) {
            return T;
        } else if (key < T->key) {
            T = T->lchild;
        } else {
            T = T->rchild;
        }
    }
    return NULL;  // 未找到
}

// 插入核心递归函数（优化：基于height维护，减少递归计算）
static bool AVL_Insert_Rec(AVLTree &T, int key) {
    // 找到插入位置，创建新结点
    if (T == NULL) {
        T = (AVLTree)malloc(sizeof(AVLNode));
        if (T == NULL) {
            fprintf(stderr, "错误：内存分配失败！\n");
            return false;
        }
        T->key = key;
        T->height = 1;  // 叶子结点高度初始化为1
        T->lchild = T->rchild = NULL;
        return true;
    }

    // 关键字已存在，插入失败
    if (key == T->key) {
        fprintf(stderr, "警告：关键字 %d 已存在，插入失败！\n", key);
        return false;
    }

    bool insert_ok = false;
    // 左子树插入
    if (key < T->key) {
        insert_ok = AVL_Insert_Rec(T->lchild, key);
    }
    // 右子树插入
    else {
        insert_ok = AVL_Insert_Rec(T->rchild, key);
    }

    if (!insert_ok) return false;

    // 插入后平衡调整
    Balance(T);
    return true;
}

// 对外接口：插入结点
bool AVL_Insert(AVLTree &T, int key) {
    return AVL_Insert_Rec(T, key);
}

// 辅助函数：找到以T为根的最小结点（用于删除）
static AVLNode *AVL_FindMin(AVLTree T) {
    if (T == NULL) return NULL;
    while (T->lchild != NULL) {
        T = T->lchild;
    }
    return T;
}

// 删除核心递归函数
static bool AVL_Delete_Rec(AVLTree &T, int key) {
    if (T == NULL) {
        fprintf(stderr, "警告：关键字 %d 不存在，删除失败！\n", key);
        return false;  // 未找到待删除结点
    }

    bool delete_ok = true;
    // 找到待删除结点
    if (key == T->key) {
        // 情况1：叶子结点 或 只有一个孩子
        if (T->lchild == NULL || T->rchild == NULL) {
            AVLTree temp = T;
            T = (T->lchild != NULL) ? T->lchild : T->rchild;
            free(temp);
        }
        // 情况2：有两个孩子，找右子树最小结点替代
        else {
            AVLNode *minNode = AVL_FindMin(T->rchild);
            T->key = minNode->key;
            // 删除右子树最小结点
            delete_ok = AVL_Delete_Rec(T->rchild, minNode->key);
        }
    }
    // 左子树删除
    else if (key < T->key) {
        delete_ok = AVL_Delete_Rec(T->lchild, key);
    }
    // 右子树删除
    else {
        delete_ok = AVL_Delete_Rec(T->rchild, key);
    }

    if (!delete_ok || T == NULL) return delete_ok;

    // 删除后平衡调整
    Balance(T);
    return true;
}

// 对外接口：删除结点
bool AVL_Delete(AVLTree &T, int key) {
    return AVL_Delete_Rec(T, key);
}

// 批量创建AVL树
void Creat_AVL(AVLTree &T, int str[], int n) {
    if (str == NULL || n <= 0) {
        T = NULL;
        return;
    }
    T = NULL;
    for (int i = 0; i < n; i++) {
        AVL_Insert(T, str[i]);
    }
}

// 中序遍历（验证有序性，输出平衡因子和高度）
void InOrder(AVLTree T) {
    if (T != NULL) {
        InOrder(T->lchild);
        printf("key:%d | 平衡因子:%d | 高度:%d\n", 
               T->key, GetBalanceFactor(T), T->height);
        InOrder(T->rchild);
    }
}

// 层序遍历（验证平衡结构，按层输出）
void LevelOrder(AVLTree T) {
    if (T == NULL) return;

    AVLTree queue[100];  // 简易队列（假设结点数不超过100）
    int front = 0, rear = 0;
    queue[rear++] = T;

    printf("层序遍历AVL树：");
    while (front < rear) {
        AVLTree curr = queue[front++];
        printf("%d ", curr->key);

        if (curr->lchild != NULL) {
            queue[rear++] = curr->lchild;
        }
        if (curr->rchild != NULL) {
            queue[rear++] = curr->rchild;
        }
    }
    printf("\n");
}

// 销毁AVL树（释放所有内存，避免内存泄漏）
void Destroy_AVL(AVLTree &T) {
    if (T == NULL) return;
    Destroy_AVL(T->lchild);
    Destroy_AVL(T->rchild);
    free(T);
    T = NULL;  // 置空避免野指针
}

// 测试主函数
int main() {
    int arr[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    int n = sizeof(arr) / sizeof(arr[0]);
    AVLTree T = NULL;

    // 1. 创建AVL树
    printf("===== 插入序列创建AVL树 =====\n");
    Creat_AVL(T, arr, n);
    
    // 中序遍历（验证有序性）
    printf("中序遍历（key | 平衡因子 | 高度）：\n");
    InOrder(T);
    printf("\n");

    // 层序遍历（验证平衡结构）
    LevelOrder(T);

    // 2. 查找结点
    printf("\n===== 查找结点 =====\n");
    int searchKey = 11;
    AVLNode *found = AVL_Search(T, searchKey);
    if (found) {
        printf("找到结点：%d（高度：%d，平衡因子：%d）\n", 
               found->key, found->height, GetBalanceFactor(found));
    } else {
        printf("未找到结点：%d\n", searchKey);
    }

    // 3. 删除结点
    printf("\n===== 删除结点 =====\n");
    int delKey = 3;
    if (AVL_Delete(T, delKey)) {
        printf("删除结点 %d 后，中序遍历：\n", delKey);
        InOrder(T);
        printf("\n");
        printf("删除结点 %d 后，层序遍历：", delKey);
        LevelOrder(T);
    } else {
        printf("删除结点 %d 失败！\n", delKey);
    }

    // 4. 销毁树（释放内存）
    Destroy_AVL(T);
    printf("\nAVL树已销毁，内存释放完成\n");

    return 0;
}