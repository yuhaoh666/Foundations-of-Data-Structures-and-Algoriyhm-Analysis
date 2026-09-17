// 红黑树：一种自平衡的二叉排序树，每个结点带颜色（红/黑），必须满足五条性质：
//   1. 每个结点是红色或黑色
//   2. 根结点是黑色
//   3. 每个叶结点（即空指针 NULL，记作 NIL）是黑色
//   4. 红结点的两个孩子都必须是黑色（即不存在连续的两个红结点）
//   5. 从任一结点出发到它所有后代叶结点的路径上，黑结点数目相同（黑高相同）
// 由性质 4、5 可以推出：最长路径不超过最短路径的 2 倍，树高 ≤ 2·log2(n+1)，
// 因此查找、插入、删除的时间复杂度都是 O(log n)
// 插入或删除破坏性质时，用“变色 + 旋转”来修复：
//   插入修复：新结点染红，若双亲也是红，按叔结点的颜色分三种情况处理
//   删除修复：被移走的是黑结点时会产生“双重黑”，按兄弟结点的颜色及其孩子颜色分四种情况处理
// 与 AVL 树相比：红黑树的平衡条件更宽松、旋转次数更少，插入删除的综合效率更高，
// 因此被广泛应用在 STL 的 map/set、Linux 内核等场合
#include <stdio.h>
#include <stdlib.h>
#define RED 1
#define BLACK 0
#define NINF (-1000000)  // 校验时使用的关键字下界
#define PINF 1000000     // 校验时使用的关键字上界

typedef struct RBNode {
    int key;                                // 关键字
    int color;                              // 颜色：RED 或 BLACK
    struct RBNode *left, *right, *parent;   // 左孩子、右孩子、双亲
} RBNode, *RBTree;

RBNode *nil;  // 全局哨兵结点：所有空指针都指向它，这样边界情况不必单独判断

// 前置声明：插入修复与删除修复函数在下面定义，但会被更早的函数调用
void RBInsertFixup(RBTree &T, RBNode *z);
void RBDeleteFixup(RBTree &T, RBNode *x);

// 初始化哨兵：它是黑色的，左右孩子和双亲都指向自己
void InitNil() {
    nil = (RBNode *)malloc(sizeof(RBNode));
    if (nil == NULL) {
        printf("内存分配失败！\n");
        exit(-1);
    }
    nil->key = 0;
    nil->color = BLACK;
    nil->left = nil->right = nil->parent = nil;
}

// 新建一个红结点（关键字 key，左右孩子都是哨兵）
RBNode *CreateNode(int key) {
    RBNode *node = (RBNode *)malloc(sizeof(RBNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->key = key;
    node->color = RED;  // 新结点一律染红，这样不会破坏性质 5，只可能破坏性质 4
    node->left = node->right = nil;
    node->parent = nil;
    return node;
}

// 左旋：以 x 为支点，把 x 的右孩子 y 提上来，x 变成 y 的左孩子
// 旋转不改变中序遍历的次序，只在必要时调整结构
void LeftRotate(RBTree &T, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;              // y 的左子树改挂到 x 的右边
    if (y->left != nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;           // y 接替 x 原来的位置
    if (x->parent == nil) {
        T = y;                       // x 原来是根
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;                     // x 成为 y 的左孩子
    x->parent = y;
}

// 右旋：左旋的对称操作
void RightRotate(RBTree &T, RBNode *x) {
    RBNode *y = x->left;
    x->left = y->right;
    if (y->right != nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nil) {
        T = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

// 中序后继（用于删除有两个孩子的结点）
RBNode *RBMin(RBTree T) {
    while (T->left != nil) {
        T = T->left;
    }
    return T;
}

// 查找：找到返回结点指针，未找到返回哨兵 nil
RBNode *RBSearch(RBTree T, int key) {
    while (T != nil && T->key != key) {
        T = (key < T->key) ? T->left : T->right;
    }
    return T;
}

// 插入：按二叉排序树的方式找到空位置，新结点染红再接上，然后修复可能被破坏的性质 4
bool RBInsert(RBTree &T, int key) {
    RBNode *y = nil;  // y 记录插入位置的父结点
    RBNode *x = T;
    while (x != nil) {
        y = x;
        if (key == x->key) {
            return false;  // 关键字已存在，不允许重复
        }
        x = (key < x->key) ? x->left : x->right;
    }

    RBNode *z = CreateNode(key);
    if (z == NULL) {
        return false;
    }
    z->parent = y;
    if (y == nil) {
        T = z;  // 原来是空树，z 成为根
    } else if (key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    RBInsertFixup(T, z);  // 修复被破坏的红黑性质
    return true;
}

// 插入修复：z 是红结点，若它的双亲也是红，就破坏了性质 4，分三种情况处理
//   情况 1：叔结点是红 → 双亲与叔结点染黑、祖父染红，把问题往上移两层
//   情况 2：叔结点是黑且 z 是“内侧”孩子 → 旋转双亲，转成情况 3
//   情况 3：叔结点是黑且 z 是“外侧”孩子 → 变色 + 旋转祖父，修复完成
void RBInsertFixup(RBTree &T, RBNode *z) {
    while (z->parent->color == RED) {  // 双亲为红才需要处理（根的父结点是黑色哨兵，故不会越界）
        if (z->parent == z->parent->parent->left) {  // 双亲是祖父的左孩子
            RBNode *uncle = z->parent->parent->right;  // 叔结点
            if (uncle->color == RED) {
                z->parent->color = BLACK;          // 情况 1：变色即可，问题继续向上
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {       // 情况 2：先左旋双亲
                    z = z->parent;
                    LeftRotate(T, z);
                }
                z->parent->color = BLACK;          // 情况 3：变色 + 右旋祖父
                z->parent->parent->color = RED;
                RightRotate(T, z->parent->parent);
            }
        } else {  // 对称：双亲是祖父的右孩子
            RBNode *uncle = z->parent->parent->left;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RightRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(T, z->parent->parent);
            }
        }
    }
    T->color = BLACK;  // 性质 2：根结点永远是黑色
}

// 用结点 v 顶替结点 u 的位置（只调整父子关系，不释放任何结点）
void Transplant(RBTree &T, RBNode *u, RBNode *v) {
    if (u->parent == nil) {
        T = v;  // u 是根结点
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;  // 即使 v 是哨兵 nil 也要记录它的双亲，删除修复时会用到
}

// 删除关键字为 key 的结点：返回是否删除成功
// 思路：先像二叉排序树那样把结点摘掉（两个孩子都在时用中序后继顶替），
//       如果被移走的是黑结点，则黑高被破坏，需要调用修复函数
bool RBDelete(RBTree &T, int key) {
    RBNode *z = RBSearch(T, key);
    if (z == nil) {
        return false;  // 关键字不存在
    }

    RBNode *y = z;         // y 是“真正被移走的结点”
    int yColor = y->color; // 记住它的颜色，黑色才需要修复
    RBNode *x;             // x 是顶替 y 位置的结点（可能是哨兵）

    if (z->left == nil) {          // 情况 1：没有左孩子，用右孩子顶替
        x = z->right;
        Transplant(T, z, z->right);
    } else if (z->right == nil) {  // 情况 2：没有右孩子，用左孩子顶替
        x = z->left;
        Transplant(T, z, z->left);
    } else {                       // 情况 3：两个孩子都在，用中序后继 y 顶替 z
        y = RBMin(z->right);
        yColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;         // y 就是 z 的右孩子，x 的双亲要指向 y
        } else {
            Transplant(T, y, y->right);  // 先把 y 从原位置摘下来
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant(T, z, y);       // y 顶替 z 的位置
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;       // 继承 z 的颜色，保证 x 一侧的黑结点数不变
    }

    free(z);
    if (yColor == BLACK) {         // 移走的是黑结点 → 黑高被破坏，必须修复
        RBDeleteFixup(T, x);
    }
    return true;
}

// 删除修复：x 顶替了一个黑结点，相当于它“多背了一层黑色”（双重黑），
// 修复的目标就是把这层多余的黑色化解掉，下面以 x 是双亲的左孩子为例（右孩子情形完全对称）
void RBDeleteFixup(RBTree &T, RBNode *x) {
    while (x != T && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;  // w 是兄弟结点
            if (w == nil) {                // 兄弟子树为空：无法借黑色，只能把多余的黑色往上推
                x = x->parent;
                continue;
            }
            if (w->color == RED) {  // 情况 1：兄弟是红 → 转成情况 2/3/4，此时新兄弟一定是黑
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;  // 情况 2：兄弟的两个孩子都黑 → 兄弟染红，多余的黑色上移到双亲
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {  // 情况 3：兄弟的右孩子黑、左孩子红
                    w->left->color = BLACK;      //        调整兄弟，转成情况 4
                    w->color = RED;
                    RightRotate(T, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;  // 情况 4：兄弟的右孩子是红 → 变色 + 左旋双亲，结束
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(T, x->parent);
                x = T;  // 多余的黑色已经消化掉，循环可以结束
            }
        } else {
            RBNode *w = x->parent->left;  // 对称：x 是双亲的右孩子
            if (w == nil) {
                x = x->parent;
                continue;
            }
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                RightRotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(T, x->parent);
                x = T;
            }
        }
    }
    x->color = BLACK;  // 把多余的黑色去掉（x 也可能是哨兵，此时它的颜色本来就是黑）
}

// ==================== 打印与校验 ====================
// 中序遍历：红黑树本质上仍是二叉排序树，中序序列一定是递增的
void InOrder(RBTree T) {
    if (T != nil) {
        InOrder(T->left);
        printf("%d ", T->key);
        InOrder(T->right);
    }
}

// 层序遍历打印（带颜色），红结点显示为“红”，黑结点显示为“黑”
void PrintLevelOrder(RBTree T) {
    if (T == nil) {
        printf("  （空树）\n");
        return;
    }
    RBTree queue[256];
    int front = 0, rear = 0, levelEnd = 1, level = 1;
    queue[rear++] = T;
    printf("  第 %d 层：", level);
    while (front < rear) {
        RBNode *cur = queue[front++];
        printf("%d%s ", cur->key, cur->color == RED ? "红" : "黑");
        if (cur->left != nil) {
            queue[rear++] = cur->left;
        }
        if (cur->right != nil) {
            queue[rear++] = cur->right;
        }
        if (front == levelEnd && front < rear) {  // 上一层输出完，接着输出下一层
            level++;
            levelEnd = rear;
            printf("\n  第 %d 层：", level);
        }
    }
    printf("\n");
}

bool gOK;  // 校验过程中记录是否有问题

// 递归检查以 T 为根的子树，返回该子树的黑高（T 是哨兵时黑高为 0）
// low、high 是关键字必须落入的开区间，用来验证二叉排序树的有序性
int CheckSub(RBTree T, int low, int high) {
    if (T == nil) {
        return 0;
    }
    if (T->color != RED && T->color != BLACK) {
        printf("    × 结点 %d 的颜色非法（违反性质 1）\n", T->key);
        gOK = false;
    }
    if (T->key <= low || T->key >= high) {
        printf("    × 结点 %d 不在应有的关键字区间内（破坏二叉排序树性质）\n", T->key);
        gOK = false;
    }
    if (T->color == RED && (T->left->color == RED || T->right->color == RED)) {
        printf("    × 结点 %d 是红色，但它的孩子也是红色（违反性质 4）\n", T->key);
        gOK = false;
    }
    if (T->left != nil && T->left->parent != T) {
        printf("    × 结点 %d 的左孩子双亲指针错误\n", T->key);
        gOK = false;
    }
    if (T->right != nil && T->right->parent != T) {
        printf("    × 结点 %d 的右孩子双亲指针错误\n", T->key);
        gOK = false;
    }
    int lh = CheckSub(T->left, low, T->key);
    int rh = CheckSub(T->right, T->key, high);
    if (lh != rh) {
        printf("    × 结点 %d 的左右子树黑高不同（%d 与 %d，违反性质 5）\n", T->key, lh, rh);
        gOK = false;
    }
    return lh + (T->color == BLACK ? 1 : 0);
}

// 验证红黑树是否满足所有性质，同时打印整棵树的黑高
bool VerifyRB(RBTree T) {
    gOK = true;
    if (nil->color != BLACK) {
        printf("    × 哨兵（叶结点）不是黑色（违反性质 3）\n");
        gOK = false;
    }
    if (T != nil) {
        if (T->color != BLACK) {
            printf("    × 根结点不是黑色（违反性质 2）\n");
            gOK = false;
        }
        if (T->parent != nil) {
            printf("    × 根结点的双亲不是哨兵\n");
            gOK = false;
        }
    }
    int bh = CheckSub(T, NINF, PINF);
    printf("    黑高 = %d，检查结果：%s\n", bh, gOK ? "满足红黑树的全部性质" : "存在问题");
    return gOK;
}

// 中序收集所有关键字（用于和参考集合比对）
void CollectKeys(RBTree T, int arr[], int &n) {
    if (T == nil) {
        return;
    }
    CollectKeys(T->left, arr, n);
    arr[n++] = T->key;
    CollectKeys(T->right, arr, n);
}

// 销毁整棵树（后序释放），根指针复位为哨兵
void DestroyTree(RBTree &T) {
    if (T == nil) {
        return;
    }
    DestroyTree(T->left);
    DestroyTree(T->right);
    free(T);
    T = nil;
}

// ==================== 参考用的有序表（压力测试时与红黑树对照） ====================
// 往有序表中插入一个关键字（重复则不插入），返回是否真的插入了
bool RefInsert(int ref[], int &n, int key) {
    for (int i = 0; i < n; i++) {
        if (ref[i] == key) {
            return false;  // 已存在
        }
    }
    int i = n - 1;
    while (i >= 0 && ref[i] > key) {  // 从后往前挪，始终保持有序
        ref[i + 1] = ref[i];
        i--;
    }
    ref[i + 1] = key;
    n++;
    return true;
}

// 从有序表中删除一个关键字，返回是否真的删除了
bool RefDelete(int ref[], int &n, int key) {
    for (int i = 0; i < n; i++) {
        if (ref[i] == key) {
            for (int j = i; j < n - 1; j++) {
                ref[j] = ref[j + 1];
            }
            n--;
            return true;
        }
    }
    return false;
}

int main() {
    InitNil();
    RBTree T = nil;  // 根指针指向哨兵，表示空树

    // 1. 依次插入若干关键字，每插入一个都检查一次红黑性质
    int keys[] = {10, 20, 30, 15, 25, 5, 35, 28, 40, 50, 45, 22, 12, 8, 3};
    int n = sizeof(keys) / sizeof(keys[0]);
    printf("=== 1. 依次插入 %d 个关键字（每次插入后都检查性质）===\n", n);
    for (int i = 0; i < n; i++) {
        RBInsert(T, keys[i]);
        printf("  插入 %-3d 后中序：", keys[i]);
        InOrder(T);
        printf("\n");
        VerifyRB(T);
    }

    // 2. 打印最终的树结构
    printf("\n=== 2. 最终的树结构（层序遍历，带颜色）===\n");
    PrintLevelOrder(T);
    printf("  中序遍历（应为递增序列）：");
    InOrder(T);
    printf("\n");

    // 3. 查找
    printf("\n=== 3. 查找 ===\n");
    int findKeys[] = {28, 45, 7, 100};
    for (int i = 0; i < 4; i++) {
        RBNode *p = RBSearch(T, findKeys[i]);
        printf("  查找 %-4d：%s\n", findKeys[i], p == nil ? "未找到" : "找到");
    }

    // 4. 删除：分别覆盖“叶子”“只有一个孩子”“有两个孩子”以及关键字不存在的情形
    printf("\n=== 4. 删除（每删除一个都检查性质）===\n");
    int delKeys[] = {3, 50, 20, 10, 45, 7};
    for (int i = 0; i < 6; i++) {
        bool ok = RBDelete(T, delKeys[i]);
        printf("  删除 %-3d：%s，中序：", delKeys[i], ok ? "成功" : "失败（关键字不存在）");
        InOrder(T);
        printf("\n");
        VerifyRB(T);
    }
    printf("  删除后的树结构：\n");
    PrintLevelOrder(T);

    // 5. 随机压力测试：反复插入与删除，每一步都与外部的参考有序表比对，并检查红黑性质
    printf("\n=== 5. 随机压力测试：1000 步插入/删除 ===\n");
    DestroyTree(T);  // 从空树重新开始
    int ref[600], refCount = 0;
    unsigned seed = 12345u;
    bool allOK = true;
    for (int step = 0; step < 1000; step++) {
        seed = seed * 1103515245u + 12345u;  // 线性同余法产生伪随机数
        int key = (int)((seed >> 16) % 500);
        bool doInsert = ((seed >> 8) & 1) == 0;  // 一半概率插入，一半概率删除
        bool okTree, okRef;
        if (doInsert) {
            okTree = RBInsert(T, key);
            okRef = RefInsert(ref, refCount, key);
        } else {
            okTree = RBDelete(T, key);
            okRef = RefDelete(ref, refCount, key);
        }
        if (okTree != okRef) {  // 操作的成败应当与参考有序表完全一致
            printf("  × 第 %d 步：对 %d 执行%s时结果不一致\n", step + 1, key,
                   doInsert ? "插入" : "删除");
            allOK = false;
            break;
        }

        int treeKeys[600], treeCount = 0;  // 把树中的关键字按中序取出，与参考表逐个比对
        CollectKeys(T, treeKeys, treeCount);
        if (treeCount != refCount) {
            printf("  × 第 %d 步：关键字个数不一致（树中 %d 个，参考 %d 个）\n", step + 1,
                   treeCount, refCount);
            allOK = false;
            break;
        }
        for (int i = 0; i < treeCount; i++) {
            if (treeKeys[i] != ref[i]) {
                printf("  × 第 %d 步：第 %d 个关键字不一致（树中 %d，参考 %d）\n", step + 1, i + 1,
                       treeKeys[i], ref[i]);
                allOK = false;
                break;
            }
        }
        if (!allOK) {
            break;
        }

        gOK = true;
        CheckSub(T, NINF, PINF);  // 检查五条性质与双亲指针
        if (!gOK || (T != nil && (T->color != BLACK || T->parent != nil))) {
            printf("  × 第 %d 步：红黑性质被破坏\n", step + 1);
            allOK = false;
            break;
        }
    }
    if (allOK) {
        printf("  1000 步操作全部通过校验：\n");
        printf("    · 每一步的关键字集合都与参考有序表完全一致（说明插入、删除都正确）；\n");
        printf("    · 每一步都满足二叉排序树的有序性；\n");
        printf("    · 每一步都满足根为黑、红结点的孩子为黑、各路径黑高相同等性质。\n");
    }
    printf("  测试结束时树中有 %d 个关键字，整棵树的黑高 = %d\n", refCount,
           CheckSub(T, NINF, PINF));

    DestroyTree(T);
    free(nil);
    printf("\n结论：红黑树用“颜色”这一较弱的不平衡条件换来了更少的结构调整，\n");
    printf("      树高不超过 2·log2(n+1)，查找、插入、删除都能保证 O(log n)；\n");
    printf("      代价是维护性质的代码比 AVL 树复杂，插入要分三种情况、删除要分四种情况。\n");
    return 0;
}
