// 最小生成树（MST）：带权连通无向图中，边权之和最小的生成树
//   Prim 算法：从任一顶点出发，每次并入“离当前生成树最近”的顶点，适合稠密图，O(n²)
//   Kruskal 算法：把所有边按权值从小到大排序，依次选“不会成环”的边，适合稀疏图，O(e·log e)
// 本文件用邻接矩阵存储；对下面这组数据，两种算法得到的总权值都是 15
#include <stdio.h>
#define MaxSize 20
#define INF 32767  // 无穷大，表示两点之间没有边（约定所有边的权值为正数）

typedef struct {
    char vexs[MaxSize];          // 顶点表
    int arcs[MaxSize][MaxSize];  // 邻接矩阵：0 表示没有边，否则存放权值
    int vexnum, arcnum;          // 顶点数、边数
} MGraph;

// 初始化图
void InitMGraph(MGraph *G) {
    G->vexnum = 0;
    G->arcnum = 0;
    for (int i = 0; i < MaxSize; i++) {
        G->vexs[i] = '\0';
        for (int j = 0; j < MaxSize; j++) {
            G->arcs[i][j] = 0;
        }
    }
}

// 定位顶点下标，未找到返回 -1
int LocateVex(MGraph *G, char v) {
    for (int i = 0; i < G->vexnum; i++) {
        if (G->vexs[i] == v) {
            return i;
        }
    }
    return -1;
}

// 插入顶点
int InsertVertex(MGraph *G, char v) {
    if (G->vexnum >= MaxSize || LocateVex(G, v) != -1) {
        return -1;
    }
    G->vexs[G->vexnum] = v;
    return G->vexnum++;
}

// 插入无向边（对称存两份）
bool InsertEdge(MGraph *G, char v1, char v2, int weight) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) {
        return false;
    }
    G->arcs[i][j] = weight;
    G->arcs[j][i] = weight;
    G->arcnum++;
    return true;
}

// 打印带权图的邻接矩阵
void PrintGraph(MGraph *G) {
    printf("带权无向图的邻接矩阵（0 表示无边，%d 个顶点，%d 条边）：\n     ", G->vexnum, G->arcnum);
    for (int j = 0; j < G->vexnum; j++) {
        printf("%-4c", G->vexs[j]);
    }
    printf("\n");
    for (int i = 0; i < G->vexnum; i++) {
        printf("%-4c ", G->vexs[i]);
        for (int j = 0; j < G->vexnum; j++) {
            printf("%-4d", G->arcs[i][j]);
        }
        printf("\n");
    }
}

// ==================== Prim 算法（加点法） ====================
void Prim(MGraph *G, char start) {
    int n = G->vexnum;
    int s = LocateVex(G, start);
    if (s == -1) {
        printf("起点 %c 不存在！\n", start);
        return;
    }
    int lowcost[MaxSize];  // lowcost[i]：已选集合到 i 的最小边权；0 表示 i 已并入生成树
    int closest[MaxSize];  // closest[i]：与 lowcost[i] 对应的已选顶点（即 i 的“双亲”）
    for (int i = 0; i < n; i++) {  // 初始时集合中只有起点 s
        lowcost[i] = (G->arcs[s][i] == 0) ? INF : G->arcs[s][i];
        closest[i] = s;
    }
    lowcost[s] = 0;  // 起点并入生成树
    int total = 0, count = 0;
    printf("Prim 算法选出的边：\n");
    for (int k = 1; k < n; k++) {      // 一共要选出 n-1 条边
        int min = INF, v = -1;
        for (int i = 0; i < n; i++) {  // 找出权值最小的候选边
            if (lowcost[i] != 0 && lowcost[i] < min) {
                min = lowcost[i];
                v = i;
            }
        }
        if (v == -1) {  // 还有顶点不可达，说明图不连通
            printf("  图不连通，不存在最小生成树\n");
            return;
        }
        printf("  (%c, %c) 权值 %d\n", G->vexs[closest[v]], G->vexs[v], min);
        total += min;
        lowcost[v] = 0;                // 把 v 并入集合
        for (int i = 0; i < n; i++) {  // 用 v 去更新其余顶点的候选边
            if (G->arcs[v][i] != 0 && G->arcs[v][i] < lowcost[i]) {
                lowcost[i] = G->arcs[v][i];
                closest[i] = v;
            }
        }
        count++;
    }
    printf("Prim 最小生成树总权值：%d（共 %d 条边）\n", total, count);
}

// ==================== Kruskal 算法（加边法） ====================
typedef struct {
    int u, v, w;  // 边的两个端点下标与权值
} Edge;

// 并查集查找（带路径压缩）：parent[x] < 0 表示 x 是集合的根
int FindRoot(int parent[], int x) {
    if (parent[x] < 0) {
        return x;
    }
    return parent[x] = FindRoot(parent, parent[x]);
}

void Kruskal(MGraph *G) {
    Edge edges[MaxSize * MaxSize];
    int e = 0;
    for (int i = 0; i < G->vexnum; i++) {  // 收集所有边（只取上三角，避免重复）
        for (int j = i + 1; j < G->vexnum; j++) {
            if (G->arcs[i][j] != 0) {
                edges[e].u = i;
                edges[e].v = j;
                edges[e].w = G->arcs[i][j];
                e++;
            }
        }
    }
    // 按权值从小到大排序（边数不多，冒泡即可；实际实现可用快速排序）
    for (int i = 0; i < e - 1; i++) {
        for (int j = 0; j < e - 1 - i; j++) {
            if (edges[j].w > edges[j + 1].w) {
                Edge t = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = t;
            }
        }
    }
    int parent[MaxSize];  // 并查集：判断两个顶点是否已经连通
    for (int i = 0; i < G->vexnum; i++) {
        parent[i] = -1;
    }
    int total = 0, count = 0;
    printf("Kruskal 算法选出的边：\n");
    for (int i = 0; i < e && count < G->vexnum - 1; i++) {
        int ru = FindRoot(parent, edges[i].u);
        int rv = FindRoot(parent, edges[i].v);
        if (ru == rv) {  // 两端点已连通，选这条边会成环，跳过
            continue;
        }
        printf("  (%c, %c) 权值 %d\n", G->vexs[edges[i].u], G->vexs[edges[i].v], edges[i].w);
        total += edges[i].w;
        parent[ru] = rv;  // 合并两个集合
        count++;
    }
    if (count < G->vexnum - 1) {
        printf("  图不连通，不存在最小生成树\n");
        return;
    }
    printf("Kruskal 最小生成树总权值：%d（共 %d 条边）\n", total, count);
}

int main() {
    MGraph G;
    InitMGraph(&G);

    // 教材中的经典带权无向图（6 个顶点 A~F）
    char vs[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < 6; i++) {
        InsertVertex(&G, vs[i]);
    }
    InsertEdge(&G, 'A', 'B', 6);
    InsertEdge(&G, 'A', 'C', 1);
    InsertEdge(&G, 'A', 'D', 5);
    InsertEdge(&G, 'B', 'C', 5);
    InsertEdge(&G, 'B', 'E', 3);
    InsertEdge(&G, 'C', 'D', 5);
    InsertEdge(&G, 'C', 'E', 6);
    InsertEdge(&G, 'C', 'F', 4);
    InsertEdge(&G, 'D', 'F', 2);
    InsertEdge(&G, 'E', 'F', 6);
    PrintGraph(&G);

    Prim(&G, 'A');
    printf("\n");
    Kruskal(&G);
    printf("\n（两种算法的总权值一定相同；选出的具体边可能不同，因为最小生成树未必唯一）\n");
    return 0;
}
