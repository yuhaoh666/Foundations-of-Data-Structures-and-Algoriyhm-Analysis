#include <stdio.h>
#define MAX_SIZE 100

// 双亲表示法实现并查集
typedef struct {
    int parent[MAX_SIZE]; // parent[i] 存储元素 i 的父节点索引
    int size;            // 集合的数量
} UnionFind;

// 初始化并查集
void initUnionFind(UnionFind *uf, int n) {
    uf->size = n;
    for (int i = 0; i < n; i++) {
        uf->parent[i] = -1; // -1 表示该元素是一个集合的根节点
    }
}

// 查找元素所在的集合（路径压缩）
int find(UnionFind *uf, int x) {
    if (uf->parent[x] < 0) {
        return x; // x 是根节点
    }
    // 路径压缩
    uf->parent[x] = find(uf, uf->parent[x]);
    return uf->parent[x];
}

// 合并两个集合
void unionSets(UnionFind *uf, int x, int y) {
    int rootX = find(uf, x);
    int rootY = find(uf, y);
    if (rootX != rootY) {
        uf->parent[rootX] = rootY;
        uf->size--;
    }
}

// 用根节点的绝对值来表示集合的大小，优化合并操作
void unionSetsOptimized(UnionFind *uf, int x, int y) {
    int rootX = find(uf, x);
    int rootY = find(uf, y);
    if (rootX != rootY) {
        if (uf->parent[rootX] > uf->parent[rootY]) {  // rootX 的绝对值较小
            uf->parent[rootY] += uf->parent[rootX];
            uf->parent[rootX] = rootY;
        } else {
            uf->parent[rootX] += uf->parent[rootY];
            uf->parent[rootY] = rootX;
        }
        uf->size--;
    }
}

int main() {
    UnionFind uf;
    initUnionFind(&uf, 5); // 初始化一个包含 5 个元素的并查集

    unionSetsOptimized(&uf, 0, 1); // 合并集合 {0} 和 {1}
    unionSetsOptimized(&uf, 2, 3); // 合并集合 {2} 和 {3}
    unionSetsOptimized(&uf, 1, 2); // 合并集合 {0,1} 和 {2,3}

    printf("集合数量: %d\n", uf.size); // 输出集合数量
    printf("元素 0 的根节点: %d\n", find(&uf, 0)); // 输出元素 0 的根节点
    printf("元素 4 的根节点: %d\n", find(&uf, 4)); // 输出元素 4 的根节点

    return 0;
}