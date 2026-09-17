// B+ 树：B 树的变体，同样要求所有叶结点在同一层，但有两点重要区别
//   1. 有 n 棵子树的结点就有 n 个关键字（B 树是 n 棵子树对应 n-1 个关键字）；
//      非叶结点中的关键字只起“索引”作用，真正的记录（关键字）全部放在叶结点里
//   2. 所有叶结点按关键字大小顺序用指针串成一条单链表，
//      因此 B+ 树既能像 B 树那样“随机查找”，又能顺着叶子链表做“顺序查找 / 范围查询”
// 本文件实现 4 阶 B+ 树：约定每个结点最多 M = 4 个关键字（最多 4 棵子树），
//   除根结点外每个结点至少有 ⌈M/2⌉ = 2 个关键字；非叶结点的 key[i] 是第 i 棵子树中的最大关键字
// 应用：数据库索引、文件系统（如 MySQL 的 InnoDB、NTFS），
//       因为它的叶结点链表特别适合“范围查询”和“全表顺序扫描”
#include <stdio.h>
#include <stdlib.h>
#define M 4                      // B+ 树的阶
#define MAXKEY M                 // 每个结点最多 M 个关键字（同时也是最多 M 棵子树）
#define MINKEY ((M + 1) / 2)     // 除根外每个结点最少 ⌈M/2⌉ 个关键字
#define MAXCAP (M + 2)           // 数组容量：留出分裂时的临时空间
#define MAXNODE 200              // 打印时队列的容量

// 约定：结点中 key[1..keynum] 递增，child[i-1] 中所有关键字都 <= key[i]，
//       并且 key[i] 恰好等于 child[i-1] 这棵子树中的最大关键字（i = 1..keynum）
typedef struct BPNode {
    bool isLeaf;                   // 是否是叶结点
    int key[MAXCAP];               // 关键字：非叶结点存各子树的最大关键字，叶结点存记录关键字
    struct BPNode *child[MAXCAP];  // 子树指针（仅非叶结点使用）
    struct BPNode *next;           // 叶子链表指针（仅叶结点使用）
    int keynum;                    // 关键字个数
    struct BPNode *parent;         // 双亲指针
} BPNode, *BPTree;

// 新建一个结点
BPNode *CreateNode(bool isLeaf) {
    BPNode *p = (BPNode *)malloc(sizeof(BPNode));
    if (p == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    p->isLeaf = isLeaf;
    p->keynum = 0;
    p->next = NULL;
    p->parent = NULL;
    for (int i = 0; i < MAXCAP; i++) {
        p->child[i] = NULL;
    }
    return p;
}

// 找出 p 是双亲的第几棵子树（返回的是子树下标，从 0 开始）
int IndexOfChild(BPNode *parent, BPNode *p) {
    for (int i = 0; i < parent->keynum; i++) {
        if (parent->child[i] == p) {
            return i;
        }
    }
    return -1;
}

// 沿索引一路走到 key 可能落在的叶结点；若 key 比整棵树的最大关键字还大则返回 NULL
BPNode *FindLeaf(BPTree T, int key) {
    BPNode *p = T;
    while (p != NULL && !p->isLeaf) {
        int j = 1;
        while (j <= p->keynum && key > p->key[j]) {  // 第一个“最大关键字 >= key”的子树
            j++;
        }
        if (j > p->keynum) {
            return NULL;  // 比这棵子树里所有关键字都大，一定不在树中
        }
        p = p->child[j - 1];
    }
    return p;
}

// 插入时用的定位：即使 key 比所有关键字都大，也走到最后一棵子树对应的叶结点
BPNode *FindInsertLeaf(BPTree T, int key) {
    BPNode *p = T;
    while (p != NULL && !p->isLeaf) {
        int j = 1;
        while (j <= p->keynum && key > p->key[j]) {
            j++;
        }
        if (j > p->keynum) {
            j = p->keynum;  // 走最后一棵子树
        }
        p = p->child[j - 1];
    }
    return p;
}

// 随机查找（从根一路走到叶结点）：found 返回是否找到，nodeVisited 返回访问过的结点个数
bool BPSearch(BPTree T, int key, int &nodeVisited) {
    nodeVisited = 0;
    BPNode *p = T;
    while (p != NULL && !p->isLeaf) {
        nodeVisited++;
        int j = 1;
        while (j <= p->keynum && key > p->key[j]) {  // 定位到可能包含 key 的那棵子树
            j++;
        }
        if (j > p->keynum) {
            return false;  // 比这棵子树里的最大关键字还大，一定不存在
        }
        p = p->child[j - 1];
    }
    if (p == NULL) {
        return false;
    }
    nodeVisited++;
    for (int i = 1; i <= p->keynum; i++) {  // 在叶结点内部顺序查找
        if (p->key[i] == key) {
            return true;
        }
        if (p->key[i] > key) {
            return false;
        }
    }
    return false;
}

// 顺序查找（沿叶子链表）：从最左边的叶结点开始逐个比较，返回是否找到
// 顺着这条链表还可以方便地做范围查询，这正是 B+ 树相对 B 树的优势
bool BPSeqSearch(BPTree T, int key, int &visited) {
    visited = 0;
    if (T == NULL) {
        return false;
    }
    BPNode *p = T;
    while (!p->isLeaf) {
        p = p->child[0];  // 一直往左走到最小的叶结点
    }
    for (; p != NULL; p = p->next) {
        for (int i = 1; i <= p->keynum; i++) {
            visited++;
            if (p->key[i] == key) {
                return true;
            }
            if (p->key[i] > key) {
                return false;  // 叶子链表递增，后面的只会更大
            }
        }
    }
    return false;
}

// 从结点 p 开始沿双亲链向上修正索引：
// 非叶结点中存放的是“子树的最大关键字”，叶结点的最大值变了就要一路改上去
void FixIndexUp(BPNode *p) {
    while (p->parent != NULL) {
        BPNode *parent = p->parent;
        int idx = IndexOfChild(parent, p);
        int newMax = p->key[p->keynum];
        if (parent->key[idx + 1] == newMax) {
            return;  // 上层的索引值没有变化，再往上都不用改了
        }
        parent->key[idx + 1] = newMax;
        p = parent;  // 本结点的最大关键字变了，继续向上修正
    }
}

// 插入关键字：先插入到叶结点，再修正索引，最后自底向上处理分裂
void BPInsert(BPTree &T, int key) {
    if (T == NULL) {  // 空树：造一个叶结点当根
        T = CreateNode(true);
        T->key[1] = key;
        T->keynum = 1;
        return;
    }

    // 1. 找到关键字应该插入的叶结点，并在叶结点内保持有序地插入
    BPNode *p = FindInsertLeaf(T, key);
    int i = p->keynum;
    while (i >= 1 && key < p->key[i]) {
        p->key[i + 1] = p->key[i];
        i--;
    }
    p->key[i + 1] = key;
    p->keynum++;

    // 2. 叶结点中的最大关键字可能变大，沿双亲链向上修正索引
    FixIndexUp(p);

    // 3. 自底向上处理上溢（关键字个数超过上限就要分裂）
    while (p->keynum > MAXKEY) {
        BPNode *parent = p->parent;
        if (parent == NULL) {  // p 原来是根：造一个新根，B+ 树长高一层
            parent = CreateNode(false);
            parent->parent = NULL;
            parent->child[0] = p;
            parent->key[1] = p->key[p->keynum];
            parent->keynum = 1;
            p->parent = parent;
            T = parent;
        }
        int idx = IndexOfChild(parent, p);   // p 是双亲的第几棵子树
        int leftCnt = (MAXKEY + 1) / 2;      // 4 阶时把 5 个关键字分成左边 2 个、右边 3 个
        BPNode *q = CreateNode(p->isLeaf);   // 分裂出来的右半部分
        int j = 0;
        for (int t = leftCnt + 1; t <= p->keynum; t++) {  // 关键字右半部分搬给 q
            q->key[++j] = p->key[t];
        }
        if (!p->isLeaf) {  // 内部结点还要把子树一起搬过去
            for (int t = 0; t < j; t++) {
                q->child[t] = p->child[leftCnt + t];
                q->child[t]->parent = q;
            }
        } else {  // 叶结点要接好叶子链表，这是“顺序查找”的前提
            q->next = p->next;
            p->next = q;
        }
        q->keynum = j;
        q->parent = parent;
        p->keynum = leftCnt;  // p 只保留左半部分

        // 在双亲中 p 的索引项后面插入 q 的索引项，同时更新 p 的最大关键字
        for (int t = parent->keynum; t >= idx + 2; t--) {
            parent->key[t + 1] = parent->key[t];
            parent->child[t] = parent->child[t - 1];
        }
        parent->key[idx + 1] = p->key[p->keynum];
        parent->key[idx + 2] = q->key[q->keynum];
        parent->child[idx + 1] = q;
        parent->keynum++;
        p = parent;  // 双亲也可能上溢，继续向上处理
    }
}

// 处理下溢：结点 p 的关键字个数少于下限时
//   1. 兄弟还有富余 → 向兄弟“借”一个（把兄弟边上的关键字挪过来，并修改双亲的索引）
//   2. 兄弟也都只有下限个 → 与兄弟合并（合并后关键字个数 = (MINKEY-1) + MINKEY < MAXKEY）
// 合并会让双亲少一个孩子，双亲也可能下溢，所以要循环向上处理
void FixUnderflow(BPTree &T, BPNode *p) {
    while (p != T && p->keynum < MINKEY) {
        BPNode *parent = p->parent;
        int idx = IndexOfChild(parent, p);
        BPNode *left = (idx > 0) ? parent->child[idx - 1] : NULL;
        BPNode *right = (idx + 1 < parent->keynum) ? parent->child[idx + 1] : NULL;

        if (left != NULL && left->keynum > MINKEY) {  // 情况 1：向左兄弟借
            for (int i = p->keynum; i >= 1; i--) {  // 关键字整体后移，腾出最前面一个位置
                p->key[i + 1] = p->key[i];
            }
            p->key[1] = left->key[left->keynum];  // 左兄弟的最大关键字挪到 p 的最前面
            if (!p->isLeaf) {                     // 内部结点还要把对应的子树挪过来
                for (int i = p->keynum; i >= 1; i--) {
                    p->child[i] = p->child[i - 1];
                }
                p->child[0] = left->child[left->keynum - 1];
                p->child[0]->parent = p;
            }
            left->keynum--;
            p->keynum++;
            FixIndexUp(left);  // 左兄弟的最大关键字变小了，索引要跟着改
            FixIndexUp(p);
            return;
        }

        if (right != NULL && right->keynum > MINKEY) {  // 情况 2：向右兄弟借
            p->key[p->keynum + 1] = right->key[1];      // 右兄弟的最小关键字挪到 p 的最后
            if (!p->isLeaf) {
                p->child[p->keynum] = right->child[0];
                p->child[p->keynum]->parent = p;
                for (int i = 1; i < right->keynum; i++) {
                    right->child[i - 1] = right->child[i];
                }
            }
            for (int i = 1; i < right->keynum; i++) {
                right->key[i] = right->key[i + 1];
            }
            right->keynum--;
            p->keynum++;
            FixIndexUp(p);
            return;
        }

        // 情况 3：两个兄弟都没有富余，只能合并（优先并到左兄弟里）
        BPNode *kept = (left != NULL) ? left : p;      // 保留下来、接收内容的结点
        BPNode *removed = (left != NULL) ? p : right;  // 被搬空、之后释放的结点
        int base = kept->keynum;
        for (int i = 1; i <= removed->keynum; i++) {  // removed 的关键字接到 kept 后面
            kept->key[base + i] = removed->key[i];
            if (!kept->isLeaf) {  // 内部结点还要接上子树
                kept->child[base + i - 1] = removed->child[i - 1];
                kept->child[base + i - 1]->parent = kept;
            }
        }
        kept->keynum = base + removed->keynum;
        if (kept->isLeaf) {
            kept->next = removed->next;  // 维护叶子链表
        }

        // 从双亲中删掉 removed 这个孩子（连同与它对应的索引项）
        int ridx = (left != NULL) ? idx : idx + 1;
        // 合并之后 kept 吸收了 removed 的全部内容，它的最大关键字就是 removed 原来的最大值
        // （child[i] 对应的索引项是 key[i+1]，所以这里正好是 key[ridx] ← key[ridx+1]）
        parent->key[ridx] = parent->key[ridx + 1];
        for (int i = ridx; i < parent->keynum - 1; i++) {
            parent->key[i + 1] = parent->key[i + 2];
            parent->child[i] = parent->child[i + 1];
        }
        parent->keynum--;
        free(removed);
        FixIndexUp(parent);  // 双亲的索引可能变化
        p = parent;          // 双亲少了一个孩子，可能也要继续调整
    }

    // 根结点的特判
    if (T != NULL && T->isLeaf && T->keynum == 0) {  // 根叶结点已空 → 变成空树
        free(T);
        T = NULL;
    } else if (T != NULL && !T->isLeaf && T->keynum == 1) {  // 分支根只剩一棵子树 → 树高降一层
        BPNode *old = T;
        T = T->child[0];
        T->parent = NULL;
        free(old);
    }
}

// 删除关键字：先把它从叶结点中摘掉，再修正索引，最后处理可能的下溢
bool BPDelete(BPTree &T, int key) {
    if (T == NULL) {
        return false;
    }
    BPNode *p = FindLeaf(T, key);
    if (p == NULL) {
        return false;  // 比整棵树的最大关键字还大
    }
    int pos = 0;
    for (int i = 1; i <= p->keynum; i++) {  // 在叶结点中找到这个关键字
        if (p->key[i] == key) {
            pos = i;
            break;
        }
    }
    if (pos == 0) {
        return false;  // 叶结点里没有，说明树中不存在
    }
    for (int i = pos; i < p->keynum; i++) {  // 摘掉它
        p->key[i] = p->key[i + 1];
    }
    p->keynum--;

    FixIndexUp(p);       // 叶结点的最大关键字可能变小，先修正上层索引
    FixUnderflow(T, p);  // 关键字个数可能少于下限，向上修复
    return true;
}

// ==================== 打印与校验 ====================
// 收集叶结点中的全部关键字（B+ 树的记录只在叶子里，所以这就是全部记录，且天然有序）
void CollectKeys(BPTree T, int arr[], int &n) {
    if (T == NULL) {
        return;
    }
    BPNode *p = T;
    while (!p->isLeaf) {
        p = p->child[0];  // 先走到最左边的叶结点
    }
    for (; p != NULL; p = p->next) {  // 再顺着叶子链表收集
        for (int i = 1; i <= p->keynum; i++) {
            arr[n++] = p->key[i];
        }
    }
}

// 打印叶子链表：B+ 树的“顺序访问”走的就是这条链
void PrintLeafChain(BPTree T) {
    if (T == NULL) {
        printf("  （空树）\n");
        return;
    }
    BPNode *p = T;
    while (!p->isLeaf) {
        p = p->child[0];
    }
    printf("  叶子链表（顺序访问）：");
    for (; p != NULL; p = p->next) {
        printf("[");
        for (int i = 1; i <= p->keynum; i++) {
            printf("%d", p->key[i]);
            if (i < p->keynum) {
                printf(" ");
            }
        }
        printf("]");
        if (p->next != NULL) {
            printf(" -> ");
        }
    }
    printf("\n");
}

// 层序打印整棵树：叶结点用 (叶) 标注，内部结点显示它保存的索引关键字
void PrintTree(BPTree T) {
    if (T == NULL) {
        printf("  （空树）\n");
        return;
    }
    BPTree queue[MAXNODE];
    int front = 0, rear = 0, levelEnd = 1, level = 1;
    queue[rear++] = T;
    printf("  第 %d 层：", level);
    while (front < rear) {
        BPNode *p = queue[front++];
        printf("[");
        for (int i = 1; i <= p->keynum; i++) {
            printf("%d", p->key[i]);
            if (i < p->keynum) {
                printf(" ");
            }
        }
        printf("]%s ", p->isLeaf ? "(叶)" : "");
        if (!p->isLeaf) {
            for (int i = 0; i < p->keynum; i++) {
                if (p->child[i] != NULL) {
                    queue[rear++] = p->child[i];
                }
            }
        }
        if (front == levelEnd && front < rear) {
            level++;
            levelEnd = rear;
            printf("\n  第 %d 层：", level);
        }
    }
    printf("\n");
}

// 统计结点总数与叶结点个数
void CountNodes(BPTree T, int &nodeCount, int &leafCount) {
    if (T == NULL) {
        return;
    }
    nodeCount++;
    if (T->isLeaf) {
        leafCount++;
        return;
    }
    for (int i = 0; i < T->keynum; i++) {
        CountNodes(T->child[i], nodeCount, leafCount);
    }
}

// 递归检查：关键字个数是否越界、结点内关键字是否严格递增、
//           非叶结点的 key[i] 是否恰好等于第 i 棵子树的最大关键字、所有叶结点是否同层
bool CheckNode(BPNode *p, int depth, int &leafDepth, bool isRoot) {
    bool ok = true;
    if (p->keynum > MAXKEY) {
        printf("    × 结点关键字个数 %d 超过上限 %d\n", p->keynum, MAXKEY);
        ok = false;
    }
    if (!isRoot && p->keynum < MINKEY) {
        printf("    × 非根结点的关键字个数 %d 少于下限 %d\n", p->keynum, MINKEY);
        ok = false;
    }
    for (int i = 1; i <= p->keynum; i++) {
        if (i > 1 && p->key[i] <= p->key[i - 1]) {
            printf("    × 结点内的关键字不是严格递增的\n");
            ok = false;
        }
    }
    if (p->isLeaf) {
        if (leafDepth == 0) {
            leafDepth = depth;
        } else if (leafDepth != depth) {
            printf("    × 叶结点不在同一层（第 %d 层与第 %d 层）\n", leafDepth, depth);
            ok = false;
        }
        return ok;
    }
    for (int i = 0; i < p->keynum; i++) {
        if (p->child[i] == NULL) {
            printf("    × 非叶结点缺少第 %d 棵子树\n", i);
            ok = false;
            continue;
        }
        if (p->child[i]->parent != p) {
            printf("    × 第 %d 棵子树的双亲指针错误\n", i);
            ok = false;
        }
        int childMax = p->child[i]->key[p->child[i]->keynum];
        if (childMax != p->key[i + 1]) {  // 索引必须等于子树的最大关键字
            printf("    × 索引 key[%d] = %d 与子树的最大关键字 %d 不一致\n", i + 1, p->key[i + 1],
                   childMax);
            ok = false;
        }
        if (!CheckNode(p->child[i], depth + 1, leafDepth, false)) {
            ok = false;
        }
    }
    return ok;
}

// 校验整棵 B+ 树，并打印规模信息
bool VerifyBPTree(BPTree T) {
    int nodeCount = 0, leafCount = 0, leafDepth = 0, keyCount = 0;
    CountNodes(T, nodeCount, leafCount);
    bool ok = true;
    if (T == NULL) {
        printf("    （空树）\n");
    } else {
        if (T->parent != NULL) {
            printf("    × 根结点的双亲指针不为空\n");
            ok = false;
        }
        if (T->keynum < 1) {
            printf("    × 根结点没有关键字\n");
            ok = false;
        }
        if (!CheckNode(T, 1, leafDepth, true)) {
            ok = false;
        }
        int last = -1000000;  // 顺着叶子链表检查：覆盖所有记录且严格递增
        BPNode *p = T;
        while (!p->isLeaf) {
            p = p->child[0];
        }
        for (; p != NULL; p = p->next) {
            for (int i = 1; i <= p->keynum; i++) {
                if (p->key[i] <= last) {
                    printf("    × 叶子链表中的关键字不是严格递增的\n");
                    ok = false;
                }
                last = p->key[i];
                keyCount++;
            }
        }
    }
    printf("    结点数 = %d（叶结点 %d 个），记录总数 = %d，叶结点层号 = %d，检查结果：%s\n",
           nodeCount, leafCount, keyCount, leafDepth, ok ? "满足 4 阶 B+ 树的性质" : "存在问题");
    return ok;
}

// 范围查询：借助叶子链表输出 [low, high] 之间的所有关键字（B+ 树的拿手好戏）
void RangeQuery(BPTree T, int low, int high) {
    printf("  范围查询 [%d, %d]：", low, high);
    if (T == NULL) {
        printf("（空树）\n");
        return;
    }
    BPNode *p = T;
    while (!p->isLeaf) {
        p = p->child[0];
    }
    bool found = false;
    for (; p != NULL; p = p->next) {
        for (int i = 1; i <= p->keynum; i++) {
            if (p->key[i] > high) {  // 叶子链表递增，一旦超过右界就可以停止
                if (!found) {
                    printf("（没有满足条件的记录）");
                }
                printf("\n");
                return;
            }
            if (p->key[i] >= low) {
                printf("%d ", p->key[i]);
                found = true;
            }
        }
    }
    if (!found) {
        printf("（没有满足条件的记录）");
    }
    printf("\n");
}

// ==================== 参考用的有序表（压力测试时与 B+ 树对照） ====================
// 销毁整棵树（后序释放），根指针置空
void DestroyTree(BPTree &T) {
    if (T == NULL) {
        return;
    }
    if (!T->isLeaf) {
        for (int i = 0; i < T->keynum; i++) {
            DestroyTree(T->child[i]);
        }
    }
    free(T);
    T = NULL;
}

// 插入一个关键字（重复则不插入），返回是否真的插入了
bool RefInsert(int ref[], int &n, int key) {
    for (int i = 0; i < n; i++) {
        if (ref[i] == key) {
            return false;
        }
    }
    int i = n - 1;
    while (i >= 0 && ref[i] > key) {
        ref[i + 1] = ref[i];
        i--;
    }
    ref[i + 1] = key;
    n++;
    return true;
}

// 删除一个关键字，返回是否真的删除了
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

// 比较 B+ 树的记录集合与期望数组是否完全一致
bool SameAsExpected(BPTree T, const int expect[], int expectCount) {
    int keys[400], n = 0;
    CollectKeys(T, keys, n);
    if (n != expectCount) {
        printf("    记录个数不一致：实际 %d 条，期望 %d 条\n", n, expectCount);
        return false;
    }
    for (int i = 0; i < n; i++) {
        if (keys[i] != expect[i]) {
            printf("    第 %d 条记录不一致：实际 %d，期望 %d\n", i + 1, keys[i], expect[i]);
            return false;
        }
    }
    return true;
}

int main() {
    BPTree T = NULL;
    int keys[] = {22, 5, 11, 36, 8, 14, 30, 40, 3, 7, 9, 12, 17, 20, 25, 28, 33, 38, 41, 45};
    int n = sizeof(keys) / sizeof(keys[0]);

    printf("=== 1. 依次插入 %d 个关键字（每次插入后都检查性质）===\n", n);
    printf("（%d 阶 B+ 树：每个结点最多 %d 个关键字，除根外最少 %d 个关键字）\n\n", M, MAXKEY,
           MINKEY);
    for (int i = 0; i < n; i++) {
        BPInsert(T, keys[i]);
        printf("  插入 %-3d 后：", keys[i]);
        PrintLeafChain(T);
        VerifyBPTree(T);
    }

    printf("\n=== 2. 插入完成后的树结构 ===\n");
    PrintTree(T);
    PrintLeafChain(T);

    printf("\n=== 3. 两种查找方式的对比 ===\n");
    int findKeys[] = {25, 45, 22, 10, 50, 3};
    for (int i = 0; i < 6; i++) {
        int visitByIndex, visitedByChain;
        bool ok1 = BPSearch(T, findKeys[i], visitByIndex);
        bool ok2 = BPSeqSearch(T, findKeys[i], visitedByChain);
        printf("  查找 %-3d：随机查找 %s（从上到下访问 %d 个结点）；顺序查找 %s（比较 %d 个关键字）\n",
               findKeys[i], ok1 ? "找到" : "未找到", visitByIndex, ok2 ? "找到" : "未找到",
               visitedByChain);
    }
    printf("  说明：随机查找的次数等于树高，与关键字位置无关；\n");
    printf("        顺序查找沿叶子链表走，越靠后的记录比较次数越多，但它天生适合范围查询与全表扫描。\n");

    printf("\n=== 4. 范围查询（B+ 树的特长）===\n");
    RangeQuery(T, 11, 28);
    RangeQuery(T, 43, 45);
    RangeQuery(T, 100, 200);

    printf("\n=== 5. 删除（每次删除后都检查性质）===\n");
    int delKeys[] = {22, 5, 36, 11, 8};
    for (int i = 0; i < 5; i++) {
        bool ok = BPDelete(T, delKeys[i]);
        printf("  删除 %-3d：%s\n", delKeys[i], ok ? "成功" : "失败（关键字不存在）");
        PrintLeafChain(T);
        VerifyBPTree(T);
    }
    printf("  再删除一个不存在的关键字 99：%s\n",
           BPDelete(T, 99) ? "成功" : "失败（关键字不存在）");

    printf("\n=== 6. 删除后的树结构 ===\n");
    PrintTree(T);
    int expect[] = {3, 7, 9, 12, 14, 17, 20, 25, 28, 30, 33, 38, 40, 41, 45};
    int expectCount = sizeof(expect) / sizeof(expect[0]);
    printf("  与期望的剩余记录集合（删掉 22、5、36、11、8 之后）比对：%s\n",
           SameAsExpected(T, expect, expectCount) ? "完全一致" : "不一致");

    // 7. 随机压力测试：反复插入与删除，每一步都与参考有序表比对，并检查 B+ 树性质
    printf("\n=== 7. 随机压力测试：2000 步插入/删除 ===\n");
    printf("  先把前面那棵树销毁，从空树重新开始。\n");
    DestroyTree(T);
    int ref[600], refCount = 0, nodeCount = 0, leafCount = 0;
    unsigned seed = 987654321u;
    bool allOK = true;
    for (int step = 0; step < 2000; step++) {
        seed = seed * 1103515245u + 12345u;  // 线性同余法产生伪随机数
        int key = (int)((seed >> 16) % 300);
        bool doInsert = ((seed >> 9) & 1) == 0;
        bool okTree, okRef;
        if (doInsert) {
            BPInsert(T, key);
            okTree = true;  // B+ 树允许插入重复关键字（这里由参考表负责去重判断）
            okRef = RefInsert(ref, refCount, key);
            if (!okRef) {   // 关键字已存在时，删掉刚插入的重复记录，保持两边一致
                BPDelete(T, key);
                okTree = false;
            }
        } else {
            okTree = BPDelete(T, key);
            okRef = RefDelete(ref, refCount, key);
        }
        if (okTree != okRef) {
            printf("  × 第 %d 步：对 %d 执行%s时结果不一致\n", step + 1, key,
                   doInsert ? "插入" : "删除");
            allOK = false;
            break;
        }
        if (!SameAsExpected(T, ref, refCount)) {
            printf("  × 第 %d 步：树中的记录集合与参考有序表不一致\n", step + 1);
            allOK = false;
            break;
        }
        int leafDepth = 0;
        if (T != NULL &&
            (!CheckNode(T, 1, leafDepth, true) || T->parent != NULL)) {
            printf("  × 第 %d 步：B+ 树的性质被破坏\n", step + 1);
            allOK = false;
            break;
        }
    }
    if (allOK) {
        printf("  2000 步操作全部通过校验：\n");
        printf("    · 每一步的记录集合都与参考有序表完全一致；\n");
        printf("    · 每一步都满足关键字有序、结点关键字个数不越界、索引值等于子树最大关键字、\n");
        printf("      所有叶结点在同一层、叶子链表递增等性质。\n");
    }
    CountNodes(T, nodeCount, leafCount);
    printf("  测试结束时树中有 %d 条记录、%d 个结点（叶结点 %d 个）\n", refCount, nodeCount,
           leafCount);

    printf("\n结论：B+ 树把记录全部放在叶结点并用链表串起来，于是同时具备两种能力：\n");
    printf("      随机查找 O(log n)（沿索引从根走到叶），顺序查找与范围查询只需顺着叶子链表扫；\n");
    printf("      代价是索引结点会重复出现叶结点的关键字，占用一点额外空间，\n");
    printf("      但正是这个“冗余 + 链表”的设计，让它成为数据库索引与文件系统的不二之选。\n");
    return 0;
}
