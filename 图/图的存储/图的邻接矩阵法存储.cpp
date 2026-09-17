// 图的邻接矩阵存储（顺序存储）：用一个二维数组表示顶点之间的邻接关系
// 适合稠密图；判断两点之间是否有边只需 O(1)，但空间复杂度是 O(n²)，与边数无关
#include <stdio.h>
#define MaxSize 100
#define INF 32767  // 无穷大，带权图中表示两点之间没有边

typedef struct {
    char vexs[MaxSize];          // 顶点表（存放顶点信息）
    int arcs[MaxSize][MaxSize];  // 邻接矩阵（无权图用 0/1，带权图存权值）
    int vexnum, arcnum;          // 当前顶点数和边数
} MGraph;

// 初始化：顶点数、边数清零，邻接矩阵全部置 0
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

// 定位顶点：返回顶点 v 在顶点表中的下标，未找到返回 -1
int LocateVex(MGraph *G, char v) {
    for (int i = 0; i < G->vexnum; i++) {
        if (G->vexs[i] == v) {
            return i;
        }
    }
    return -1;
}

// 插入顶点：返回新顶点的下标，失败返回 -1
int InsertVertex(MGraph *G, char v) {
    if (G->vexnum >= MaxSize) {
        printf("顶点数已达上限，插入失败！\n");
        return -1;
    }
    if (LocateVex(G, v) != -1) {
        printf("顶点 %c 已存在，插入失败！\n", v);
        return -1;
    }
    G->vexs[G->vexnum] = v;
    return G->vexnum++;
}

// 插入边：weight 为权值（无权图传 1），isDirected 为 true 表示有向图
bool InsertEdge(MGraph *G, char v1, char v2, int weight, bool isDirected) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) {
        printf("顶点 %c 或 %c 不存在，插入边失败！\n", v1, v2);
        return false;
    }
    if (G->arcs[i][j] != 0) {
        printf("边 (%c, %c) 已存在，插入失败！\n", v1, v2);
        return false;
    }
    G->arcs[i][j] = weight;
    if (!isDirected) {         // 无向图要对称地存两条边
        G->arcs[j][i] = weight;
    }
    G->arcnum++;
    return true;
}

// 删除边
bool RemoveEdge(MGraph *G, char v1, char v2, bool isDirected) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1 || G->arcs[i][j] == 0) {
        return false;
    }
    G->arcs[i][j] = 0;
    if (!isDirected) {
        G->arcs[j][i] = 0;
    }
    G->arcnum--;
    return true;
}

// 判断两点之间是否有边
bool HasEdge(MGraph *G, char v1, char v2) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    return (i != -1 && j != -1 && G->arcs[i][j] != 0);
}

// 求无向图中顶点 v 的度：第 v 行非零元素的个数
int Degree(MGraph *G, char v) {
    int i = LocateVex(G, v);
    if (i == -1) {
        return -1;
    }
    int degree = 0;
    for (int j = 0; j < G->vexnum; j++) {
        if (G->arcs[i][j] != 0) {
            degree++;
        }
    }
    return degree;
}

// 打印邻接矩阵
void PrintMatrix(MGraph *G) {
    printf("邻接矩阵（%d 个顶点，%d 条边）：\n     ", G->vexnum, G->arcnum);
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

// 打印所有顶点的度（无向图）
void PrintDegrees(MGraph *G) {
    printf("各顶点的度：");
    for (int i = 0; i < G->vexnum; i++) {
        printf("%c:%d  ", G->vexs[i], Degree(G, G->vexs[i]));
    }
    printf("\n");
}

int main() {
    MGraph G;
    InitMGraph(&G);

    // 建立无向图：顶点 A B C D E，边 AB、AC、BC、CD、DE
    char vs[] = {'A', 'B', 'C', 'D', 'E'};
    for (int i = 0; i < 5; i++) {
        InsertVertex(&G, vs[i]);
    }
    InsertEdge(&G, 'A', 'B', 1, false);
    InsertEdge(&G, 'A', 'C', 1, false);
    InsertEdge(&G, 'B', 'C', 1, false);
    InsertEdge(&G, 'C', 'D', 1, false);
    InsertEdge(&G, 'D', 'E', 1, false);
    InsertVertex(&G, 'A');            // 重复插入顶点，应被拦截
    InsertEdge(&G, 'A', 'B', 1, false);  // 重复插入边，应被拦截

    PrintMatrix(&G);
    PrintDegrees(&G);
    printf("A 与 B 是否有边：%s\n", HasEdge(&G, 'A', 'B') ? "是" : "否");
    printf("A 与 D 是否有边：%s\n", HasEdge(&G, 'A', 'D') ? "是" : "否");

    RemoveEdge(&G, 'A', 'B', false);
    printf("删除边 (A, B) 后，边数：%d，A 的度：%d\n", G.arcnum, Degree(&G, 'A'));
    return 0;
}
