// 并查集（双亲表示法的典型应用）：用一棵“倒着长的树”表示一个集合
// parent[i] < 0 表示 i 是该集合的根结点，其绝对值等于集合中元素的个数；
// parent[i] >= 0 表示 i 的双亲下标
// 优化：查找时“路径压缩”，合并时“小集合并入大集合”，均摊时间复杂度接近 O(1)
#include <stdio.h>
#define MAX_SIZE 100

typedef struct {
    int parent[MAX_SIZE];  // 负数表示根（绝对值为集合大小），非负数表示双亲下标
    int count;             // 当前集合的个数
} UnionFind;

// 初始化：n 个元素各自成为一个集合（都是根，大小为 1）
void InitUnionFind(UnionFind *uf, int n) {
    if (n > MAX_SIZE) {
        n = MAX_SIZE;
    }
    uf->count = n;
    for (int i = 0; i < n; i++) {
        uf->parent[i] = -1;
    }
}

// 查找 x 所在集合的根，同时做路径压缩（把查找路径上的结点都直接挂到根上）
int Find(UnionFind *uf, int x) {
    if (uf->parent[x] < 0) {  // 负数说明 x 就是根
        return x;
    }
    uf->parent[x] = Find(uf, uf->parent[x]);  // 路径压缩
    return uf->parent[x];
}

// 判断两个元素是否属于同一个集合
bool SameSet(UnionFind *uf, int x, int y) {
    return Find(uf, x) == Find(uf, y);
}

// 求 x 所在集合的元素个数
int SetSize(UnionFind *uf, int x) {
    return -uf->parent[Find(uf, x)];
}

// 合并（不比较集合大小，仅用于对比）：把 rootX 挂到 rootY 下
void UnionSets(UnionFind *uf, int x, int y) {
    int rootX = Find(uf, x);
    int rootY = Find(uf, y);
    if (rootX == rootY) {  // 已经在同一个集合里
        return;
    }
    int sizeX = -uf->parent[rootX];
    uf->parent[rootX] = rootY;    // 让 rootX 的双亲指向 rootY
    uf->parent[rootY] -= sizeX;   // 集合大小累加到新的根上
    uf->count--;
}

// 合并（按集合大小合并）：把“小树”挂到“大树”下面，避免树变高
void UnionSetsBySize(UnionFind *uf, int x, int y) {
    int rootX = Find(uf, x);
    int rootY = Find(uf, y);
    if (rootX == rootY) {
        return;  // 已属于同一集合，无需合并
    }
    if (uf->parent[rootX] > uf->parent[rootY]) {  // rootX 的集合更小（绝对值小）
        uf->parent[rootY] += uf->parent[rootX];
        uf->parent[rootX] = rootY;
    } else {  // rootY 的集合更小
        uf->parent[rootX] += uf->parent[rootY];
        uf->parent[rootY] = rootX;
    }
    uf->count--;
}

// 打印每个元素所属的集合
void PrintSets(UnionFind *uf, int n) {
    printf("当前共有 %d 个集合：\n", uf->count);
    for (int i = 0; i < n; i++) {
        if (uf->parent[i] < 0) {  // i 是某个集合的根
            printf("  集合 { ");
            for (int j = 0; j < n; j++) {
                if (Find(uf, j) == i) {
                    printf("%d ", j);
                }
            }
            printf("}，大小 %d，根为 %d\n", -uf->parent[i], i);
        }
    }
}

int main() {
    int n = 7;
    UnionFind uf;
    InitUnionFind(&uf, n);
    printf("初始化 %d 个元素后的集合个数：%d\n", n, uf.count);

    // 按大小合并成 {0,1,2,3}
    UnionSetsBySize(&uf, 0, 1);
    UnionSetsBySize(&uf, 2, 3);
    UnionSetsBySize(&uf, 0, 2);
    printf("合并 0-1、2-3、{0,1}-{2,3} 后：0 与 3 是否同集合：%s\n",
           SameSet(&uf, 0, 3) ? "是" : "否");
    printf("集合 {0,1,2,3} 的元素个数：%d\n", SetSize(&uf, 2));

    // 再合并出 {4,5}
    UnionSetsBySize(&uf, 4, 5);
    // 结点 6 单独一个集合
    printf("合并 4-5 后，0 与 4 是否同集合：%s\n", SameSet(&uf, 0, 4) ? "是" : "否");

    // 用不带优化的合并做一个对比（结果集合相同，只是树可能更高）
    UnionSets(&uf, 0, 4);
    printf("再把两个大集合合并后，0 与 5 是否同集合：%s\n",
           SameSet(&uf, 0, 5) ? "是" : "否");

    PrintSets(&uf, n);
    return 0;
}
