// B 树（多路平衡查找树）：m 阶 B 树满足以下性质
//   1. 每个结点最多有 m 棵子树，即最多 m-1 个关键字
//   2. 除根结点外，每个结点至少有 ⌈m/2⌉ 棵子树，即至少 ⌈m/2⌉-1 个关键字
//   3. 若根结点不是叶子结点，则它至少有 2 棵子树
//   4. 所有叶子结点（失败结点）都在同一层，即树是完全平衡的
// 特点：树高为 O(log_⌈m/2⌉ n)，而访问一个结点恰好相当于一次磁盘读取，
//       因此 B 树很适合磁盘等外存上的动态查找（数据库索引、文件系统）
// 本文件实现 5 阶 B 树的查找、插入（分裂）、删除（借位 / 合并），并自动校验 B 树性质
#include <stdio.h>
#include <stdlib.h>
#define M 5                       // B 树的阶
#define MAXKEY (M - 1)            // 每个结点最多 M-1 = 4 个关键字
#define MINKEY ((M + 1) / 2 - 1)  // 除根外每个结点最少 ⌈M/2⌉-1 = 2 个关键字
#define MAXNODE 100               // 打印时队列的容量

// 约定：结点中 key[1..keynum] 是递增有序的关键字，
//       ptr[j-1] 中的关键字都小于 key[j]，ptr[j] 中的关键字都大于 key[j]
typedef struct BTNode {
    int key[M + 1];             // 关键字，下标 0 不使用
    struct BTNode *ptr[M + 1];  // 子树指针，有效范围是 ptr[0] ~ ptr[keynum]
    int keynum;                 // 当前关键字个数
    struct BTNode *parent;      // 双亲指针（分裂与删除调整时要向上回溯）
} BTNode, *BTree;

// 新建一个空结点
BTNode *CreateNode() {
    BTNode *p = (BTNode *)malloc(sizeof(BTNode));
    if (p == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    p->keynum = 0;
    p->parent = NULL;
    for (int i = 0; i <= M; i++) {
        p->ptr[i] = NULL;
    }
    return p;
}

// 判断结点是否叶子（B 树不存储失败结点，所以子树指针为空即为叶子）
bool IsLeaf(BTNode *p) {
    return p->ptr[0] == NULL;
}

// 查找关键字 key：找到时返回所在结点并用 pos 带回它在结点中的下标；
// 未找到时返回 NULL
BTNode *BTSearch(BTree T, int key, int &pos) {
    BTNode *p = T;
    pos = 0;
    while (p != NULL) {
        int i = 1;
        while (i <= p->keynum && key > p->key[i]) {  // 结点内关键字很少，顺序查找即可
            i++;
        }
        if (i <= p->keynum && key == p->key[i]) {
            pos = i;
            return p;  // 命中
        }
        p = p->ptr[i - 1];  // 否则进入第 i-1 棵子树继续找
    }
    return NULL;
}

// 从根一路走到关键字 key 应该落在的叶结点（供插入使用）
BTNode *FindLeaf(BTree T, int key) {
    BTNode *p = T;
    while (!IsLeaf(p)) {
        int i = 1;
        while (i <= p->keynum && key > p->key[i]) {
            i++;
        }
        p = p->ptr[i - 1];
    }
    return p;
}

// 在结点 p 中插入关键字 key 与子树 right：
// pos 表示 p 中比 key 小的关键字个数，插入后 key 落在 p->key[pos+1] 处，right 挂在 p->ptr[pos+1]
// 插入叶结点时 right 传 NULL
void InsertKeyPtr(BTNode *p, int key, BTNode *right, int pos) {
    for (int i = p->keynum; i > pos; i--) {  // 把 key[pos+1..] 与 ptr[pos+1..] 整体后移一位
        p->key[i + 1] = p->key[i];
        p->ptr[i + 1] = p->ptr[i];
    }
    p->key[pos + 1] = key;
    p->ptr[pos + 1] = right;
    if (right != NULL) {
        right->parent = p;
    }
    p->keynum++;
}

// 插入关键字：先插入到叶结点，再自底向上处理“上溢”（关键字个数超过上限就要分裂）
bool BTInsert(BTree &T, int key) {
    if (T == NULL) {  // 空树，直接建根
        T = CreateNode();
        T->key[1] = key;
        T->keynum = 1;
        return true;
    }
    int pos;
    if (BTSearch(T, key, pos) != NULL) {
        return false;  // 关键字已存在，B 树中不允许重复
    }

    BTNode *p = FindLeaf(T, key);
    int i = 0;  // 统计叶结点中小于 key 的关键字个数，确定插入位置
    while (i < p->keynum && key > p->key[i + 1]) {
        i++;
    }
    InsertKeyPtr(p, key, NULL, i);

    while (p->keynum > MAXKEY) {  // 上溢，需要分裂
        BTNode *q = CreateNode();
        int leftCnt = (MAXKEY + 1) / 2;  // 5 阶时把 5 个关键字分成左边 2 个、右边 3 个
        int upKey = p->key[leftCnt + 1]; // 中间的关键字要提升到双亲作为分隔
        int j = 0;
        for (int t = leftCnt + 2; t <= p->keynum; t++) {  // 右半部分搬到新结点 q
            q->key[++j] = p->key[t];
            q->ptr[j] = p->ptr[t];
            if (q->ptr[j] != NULL) {
                q->ptr[j]->parent = q;
            }
        }
        q->ptr[0] = p->ptr[leftCnt + 1];
        if (q->ptr[0] != NULL) {
            q->ptr[0]->parent = q;
        }
        q->keynum = j;
        q->parent = p->parent;
        p->keynum = leftCnt;  // p 只保留左半部分

        if (p->parent == NULL) {  // p 原来是根：造一个新根，树长高一层
            BTNode *root = CreateNode();
            root->key[1] = upKey;
            root->keynum = 1;
            root->ptr[0] = p;
            root->ptr[1] = q;
            p->parent = root;
            q->parent = root;
            T = root;
            break;  // 新根只有 1 个关键字，不会再上溢
        }
        BTNode *pa = p->parent;
        int idx = 0;
        while (pa->ptr[idx] != p) {  // 找到 p 是双亲的第几棵子树
            idx++;
        }
        InsertKeyPtr(pa, upKey, q, idx);  // 把 upKey 和 q 插入双亲
        p = pa;                           // 双亲也可能上溢，继续向上处理
    }
    return true;
}

// 找出 p 是双亲 parent 的第几棵子树（分裂与删除时都要用到）
int IndexOfChild(BTNode *parent, BTNode *p) {
    for (int i = 0; i <= parent->keynum; i++) {
        if (parent->ptr[i] == p) {
            return i;
        }
    }
    return -1;
}

// 处理下溢：结点 p 的关键字个数少于下限时
//   1. 若某个兄弟的关键字个数还大于下限，就向它“借”一个（相当于父子结点间做旋转）
//   2. 若兄弟也都只有下限个关键字，就和兄弟合并
//      （合并后关键字个数为 (MINKEY-1) + 1 + MINKEY = MAXKEY，正好不会上溢）
// 合并会让双亲少一个关键字，所以双亲也可能下溢，需要继续向上处理
void FixUnderflow(BTree &T, BTNode *p) {
    while (p != T && p->keynum < MINKEY) {
        BTNode *pa = p->parent;
        int idx = IndexOfChild(pa, p);
        BTNode *left = (idx > 0) ? pa->ptr[idx - 1] : NULL;
        BTNode *right = (idx + 1 <= pa->keynum) ? pa->ptr[idx + 1] : NULL;

        if (left != NULL && left->keynum > MINKEY) {  // 情况 1：向左兄弟借
            // 双亲中的分隔关键字下移到 p 的最前面，左兄弟的最大关键字上移到双亲
            if (IsLeaf(p)) {  // 叶结点只需搬关键字
                for (int i = p->keynum; i >= 1; i--) {
                    p->key[i + 1] = p->key[i];
                }
            } else {  // 内部结点还要把左兄弟的最后一棵子树挪到 p 的最前面
                for (int i = p->keynum; i >= 1; i--) {
                    p->key[i + 1] = p->key[i];
                    p->ptr[i + 1] = p->ptr[i];
                }
                p->ptr[1] = p->ptr[0];
                p->ptr[0] = left->ptr[left->keynum];
                p->ptr[0]->parent = p;
            }
            p->key[1] = pa->key[idx];
            pa->key[idx] = left->key[left->keynum];
            left->keynum--;
            p->keynum++;
            return;  // 借完以后 p 不再下溢，兄弟也仍不少于下限，调整结束
        }

        if (right != NULL && right->keynum > MINKEY) {  // 情况 2：向右兄弟借
            // 双亲中的分隔关键字下移到 p 的最后面，右兄弟的最小关键字上移到双亲
            if (IsLeaf(p)) {
                p->key[p->keynum + 1] = pa->key[idx + 1];
                pa->key[idx + 1] = right->key[1];
                for (int i = 1; i < right->keynum; i++) {
                    right->key[i] = right->key[i + 1];
                }
            } else {
                p->key[p->keynum + 1] = pa->key[idx + 1];
                p->ptr[p->keynum + 1] = right->ptr[0];  // 右兄弟的第一棵子树挪到 p 的最后
                p->ptr[p->keynum + 1]->parent = p;
                pa->key[idx + 1] = right->key[1];
                for (int i = 1; i < right->keynum; i++) {
                    right->key[i] = right->key[i + 1];
                    right->ptr[i - 1] = right->ptr[i];
                }
                right->ptr[right->keynum - 1] = right->ptr[right->keynum];
            }
            right->keynum--;
            p->keynum++;
            return;
        }

        // 情况 3：两个兄弟都只有下限个关键字，只能合并（优先与左兄弟合并）
        BTNode *kept = (left != NULL) ? left : p;      // 保留下来、吸收对方的结点
        BTNode *removed = (left != NULL) ? p : right;  // 内容被搬走、之后释放的结点
        int sepIdx = (left != NULL) ? idx : idx + 1;   // 双亲中要下移的分隔关键字下标
        int base = kept->keynum;
        kept->key[base + 1] = pa->key[sepIdx];  // 双亲的分隔关键字下移到中间
        kept->ptr[base + 1] = removed->ptr[0];
        if (kept->ptr[base + 1] != NULL) {
            kept->ptr[base + 1]->parent = kept;
        }
        for (int i = 1; i <= removed->keynum; i++) {  // removed 剩下的内容全部接到后面
            kept->key[base + 1 + i] = removed->key[i];
            kept->ptr[base + 1 + i] = removed->ptr[i];
            if (kept->ptr[base + 1 + i] != NULL) {
                kept->ptr[base + 1 + i]->parent = kept;
            }
        }
        kept->keynum = base + 1 + removed->keynum;

        for (int i = sepIdx; i < pa->keynum; i++) {  // 从双亲里删掉该关键字和 removed 子树
            pa->key[i] = pa->key[i + 1];
            pa->ptr[i] = pa->ptr[i + 1];
        }
        pa->keynum--;
        free(removed);
        p = pa;  // 双亲少了一个关键字，可能也要继续调整
    }

    // 根结点的特判
    if (T != NULL && T->keynum == 0) {
        if (IsLeaf(T)) {  // 根是叶子且关键字已删光 → 变成空树
            free(T);
            T = NULL;
        } else {  // 根是内部结点却只剩一棵子树 → 树高减一，用唯一的子树当新根
            BTNode *old = T;
            T = T->ptr[0];
            T->parent = NULL;
            free(old);
        }
    }
}

// 删除关键字：先把它摘掉（在内部结点时用前驱顶替，再到叶结点里删），最后修复可能的下溢
bool BTDelete(BTree &T, int key) {
    if (T == NULL) {
        return false;
    }
    int pos;
    BTNode *p = BTSearch(T, key, pos);
    if (p == NULL) {
        return false;  // 关键字不存在
    }

    if (!IsLeaf(p)) {
        // 关键字在内部结点：用它的前驱（左子树中最大的关键字）顶替，
        // 再把这个前驱从叶结点中删掉，这样不会破坏 B 树的结构
        BTNode *q = p->ptr[pos - 1];
        while (!IsLeaf(q)) {
            q = q->ptr[q->keynum];  // 一直往右走就是左子树中的最大关键字
        }
        p->key[pos] = q->key[q->keynum];
        p = q;
        pos = q->keynum;
    }

    for (int i = pos; i < p->keynum; i++) {  // 在叶结点里删掉这个关键字
        p->key[i] = p->key[i + 1];
    }
    p->keynum--;

    FixUnderflow(T, p);  // 关键字个数可能少于下限，逐层向上修复
    return true;
}

// ==================== 打印与校验 ====================
// 中序遍历：B 树的中序序列就是所有关键字从小到大排列的序列
void InOrder(BTree T) {
    if (T == NULL) {
        return;
    }
    for (int i = 0; i < T->keynum; i++) {
        InOrder(T->ptr[i]);
        printf("%d ", T->key[i + 1]);
    }
    InOrder(T->ptr[T->keynum]);
}

// 中序收集所有关键字到数组（用于与期望结果比对）
void CollectKeys(BTree T, int arr[], int &n) {
    if (T == NULL) {
        return;
    }
    for (int i = 0; i < T->keynum; i++) {
        CollectKeys(T->ptr[i], arr, n);
        arr[n++] = T->key[i + 1];
    }
    CollectKeys(T->ptr[T->keynum], arr, n);
}

// 层序打印整棵树：每层一行，同一个结点内的关键字用空格分隔
void PrintTree(BTree T) {
    if (T == NULL) {
        printf("  （空树）\n");
        return;
    }
    BTree queue[MAXNODE];
    int front = 0, rear = 0, levelEnd = 1, level = 1;
    queue[rear++] = T;
    printf("  第 %d 层：", level);
    while (front < rear) {
        BTNode *p = queue[front++];
        printf("[");
        for (int i = 1; i <= p->keynum; i++) {
            printf("%d", p->key[i]);
            if (i < p->keynum) {
                printf(" ");
            }
        }
        printf("] ");
        for (int i = 0; i <= p->keynum; i++) {  // 把非空子树按顺序排入队列
            if (p->ptr[i] != NULL) {
                queue[rear++] = p->ptr[i];
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

// 统计结点总数与关键字总数
void CountNodes(BTree T, int &nodeCount, int &keyCount) {
    if (T == NULL) {
        return;
    }
    nodeCount++;
    keyCount += T->keynum;
    for (int i = 0; i <= T->keynum; i++) {
        CountNodes(T->ptr[i], nodeCount, keyCount);
    }
}

// 递归检查一棵子树是否合法：
//   · 关键字个数是否在 [下限, 上限] 内（根结点只要求不超过上限）
//   · 关键字是否严格递增，且落在允许的区间 (low, high) 内
//   · 所有叶结点是否在同一层
//   · 每棵子树的双亲指针是否正确
bool CheckNode(BTNode *p, int low, int high, int depth, int &leafDepth, bool isRoot) {
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
        if (p->key[i] <= low || p->key[i] >= high) {
            printf("    × 关键字 %d 超出了它应有的范围\n", p->key[i]);
            ok = false;
        }
        if (i > 1 && p->key[i] <= p->key[i - 1]) {
            printf("    × 结点内的关键字不是严格递增的\n");
            ok = false;
        }
    }
    if (IsLeaf(p)) {
        if (leafDepth == 0) {
            leafDepth = depth;  // 记住第一个叶结点的层号
        } else if (leafDepth != depth) {
            printf("    × 叶结点不在同一层（第 %d 层与第 %d 层）\n", leafDepth, depth);
            ok = false;
        }
        return ok;
    }
    for (int i = 0; i <= p->keynum; i++) {
        if (p->ptr[i] == NULL) {
            printf("    × 内部结点缺少第 %d 棵子树\n", i);
            ok = false;
            continue;
        }
        if (p->ptr[i]->parent != p) {
            printf("    × 第 %d 棵子树的双亲指针错误\n", i);
            ok = false;
        }
        int lo = (i == 0) ? low : p->key[i];              // 第 i 棵子树的关键字必须落在
        int hi = (i == p->keynum) ? high : p->key[i + 1]; // (key[i], key[i+1]) 之间
        if (!CheckNode(p->ptr[i], lo, hi, depth + 1, leafDepth, false)) {
            ok = false;
        }
    }
    return ok;
}

// 校验整棵树是否满足 5 阶 B 树的性质，并打印规模信息
bool VerifyBTree(BTree T) {
    int nodeCount = 0, keyCount = 0, leafDepth = 0;
    CountNodes(T, nodeCount, keyCount);
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
        if (!CheckNode(T, -1000000, 1000000, 1, leafDepth, true)) {
            ok = false;
        }
    }
    printf("    结点数 = %d，关键字总数 = %d，叶结点层号 = %d，检查结果：%s\n", nodeCount,
           keyCount, leafDepth, ok ? "满足 5 阶 B 树的性质" : "存在问题");
    return ok;
}

// 比较树中的关键字集合与期望数组是否完全一致（两边都是递增序列）
bool SameAsExpected(BTree T, const int expect[], int expectCount) {
    int keys[200], n = 0;
    CollectKeys(T, keys, n);
    if (n != expectCount) {
        printf("    关键字个数不一致：实际 %d 个，期望 %d 个\n", n, expectCount);
        return false;
    }
    for (int i = 0; i < n; i++) {
        if (keys[i] != expect[i]) {
            printf("    第 %d 个关键字不一致：实际 %d，期望 %d\n", i + 1, keys[i], expect[i]);
            return false;
        }
    }
    return true;
}

// 参考用的有序表：插入一个关键字（重复则不插入），返回是否真的插入了
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

// 参考用的有序表：删除一个关键字，返回是否真的删除了
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
    BTree T = NULL;
    int keys[] = {22, 5, 11, 36, 8, 14, 30, 40, 3, 7, 9, 12, 17, 20, 25, 28, 33, 38, 41, 45};
    int n = sizeof(keys) / sizeof(keys[0]);

    printf("=== 1. 依次插入 %d 个关键字 ===\n", n);
    printf("（%d 阶 B 树：每个结点最多 %d 个关键字，除根外最少 %d 个关键字）\n\n", M, MAXKEY,
           MINKEY);
    for (int i = 0; i < n; i++) {
        BTInsert(T, keys[i]);
        printf("  插入 %-3d 后中序：", keys[i]);
        InOrder(T);
        printf("\n");
        VerifyBTree(T);
    }

    printf("\n=== 2. 插入完成后的树结构 ===\n");
    PrintTree(T);
    printf("  中序遍历：");
    InOrder(T);
    printf("\n");

    printf("\n=== 3. 查找 ===\n");
    int findKeys[] = {25, 45, 22, 10, 50, 3};
    for (int i = 0; i < 6; i++) {
        int pos;
        BTNode *p = BTSearch(T, findKeys[i], pos);
        printf("  查找 %-3d：%s\n", findKeys[i], p == NULL ? "未找到" : "找到");
    }
    printf("  说明：B 树的查找只需从根走到叶，比较次数等于树高（结点内是顺序查找）。\n");

    printf("\n=== 4. 删除（每次删除后都检查 B 树性质）===\n");
    int delKeys[] = {22, 5, 36, 11, 8};
    for (int i = 0; i < 5; i++) {
        bool ok = BTDelete(T, delKeys[i]);
        printf("  删除 %-3d：%s\n    中序：", delKeys[i], ok ? "成功" : "失败（关键字不存在）");
        InOrder(T);
        printf("\n");
        VerifyBTree(T);
    }
    printf("  再删除一个不存在的关键字 99：%s\n",
           BTDelete(T, 99) ? "成功" : "失败（关键字不存在）");

    printf("\n=== 5. 删除后的树结构 ===\n");
    PrintTree(T);

    // 用期望的剩余关键字集合核对一次
    int expect[] = {3, 7, 9, 12, 14, 17, 20, 25, 28, 30, 33, 38, 40, 41, 45};
    int expectCount = sizeof(expect) / sizeof(expect[0]);
    printf("  与期望的剩余关键字集合（删掉 22、5、36、11、8 之后）比对：%s\n",
           SameAsExpected(T, expect, expectCount) ? "完全一致" : "不一致");
    VerifyBTree(T);

    printf("\n=== 6. 把剩余关键字全部删除，观察树的变化 ===\n");
    for (int i = 0; i < expectCount; i++) {
        BTDelete(T, expect[i]);
        printf("  删除 %-3d 后：", expect[i]);
        if (T == NULL) {
            printf("树已空\n");
        } else {
            printf("中序：");
            InOrder(T);
            printf("\n");
        }
    }
    printf("  全部删除后树是否为空：%s\n", T == NULL ? "是" : "否");

    // 7. 随机压力测试：反复插入与删除，每一步都与参考有序表比对，并检查 B 树性质
    printf("\n=== 7. 随机压力测试：2000 步插入/删除 ===\n");
    int ref[600], refCount = 0, nodeCount = 0, keyCount = 0;
    unsigned seed = 20240607u;
    bool allOK = true;
    for (int step = 0; step < 2000; step++) {
        seed = seed * 1103515245u + 12345u;  // 线性同余法产生伪随机数
        int key = (int)((seed >> 16) % 300);
        bool doInsert = ((seed >> 9) & 1) == 0;
        bool okTree, okRef;
        if (doInsert) {
            okTree = BTInsert(T, key);
            okRef = RefInsert(ref, refCount, key);
        } else {
            okTree = BTDelete(T, key);
            okRef = RefDelete(ref, refCount, key);
        }
        if (okTree != okRef) {  // 操作结果应与参考有序表完全一致
            printf("  × 第 %d 步：对 %d 执行%s时结果不一致\n", step + 1, key,
                   doInsert ? "插入" : "删除");
            allOK = false;
            break;
        }
        if (!SameAsExpected(T, ref, refCount)) {
            printf("  × 第 %d 步：树中的关键字集合与参考有序表不一致\n", step + 1);
            allOK = false;
            break;
        }
        int leafDepth = 0;
        if (T != NULL &&
            (!CheckNode(T, -1000000, 1000000, 1, leafDepth, true) || T->parent != NULL)) {
            printf("  × 第 %d 步：B 树的性质被破坏\n", step + 1);
            allOK = false;
            break;
        }
    }
    if (allOK) {
        printf("  2000 步操作全部通过校验：\n");
        printf("    · 每一步的关键字集合都与参考有序表完全一致；\n");
        printf("    · 每一步都满足关键字有序、结点关键字个数不越界、所有叶结点在同一层等性质。\n");
    }
    CountNodes(T, nodeCount, keyCount);
    printf("  测试结束时树中有 %d 个关键字，共 %d 个结点\n", refCount, nodeCount);

    printf("\n结论：B 树靠“分裂”处理上溢、靠“借位 / 合并”处理下溢，始终让所有叶结点处在同一层，\n");
    printf("      因而是绝对平衡的：5 阶 B 树的高度约为 log3(n)；\n");
    printf("      查找、插入、删除都只需 O(log n) 次结点访问，而每个结点正好对应一次磁盘读取，\n");
    printf("      这就是 B 树（以及它的变体 B+ 树）成为数据库索引、文件系统基础结构的原因。\n");
    return 0;
}
