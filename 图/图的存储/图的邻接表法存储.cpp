// 图的邻接表存储（顺序存储 + 链式存储）：顶点用数组存放，
// 每个顶点的邻接点用一条单链表串起来
// 适合稀疏图；空间复杂度 O(n+e)，找某点的所有邻接点很方便，
// 但判断两点之间是否有边需要遍历链表（O(该点的度)）
#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100

typedef struct ArcNode {      // 边结点（弧结点）
    int adjvex;               // 该边所指向的顶点在顶点表中的下标
    int weight;               // 边的权值（无权图可统一取 1）
    struct ArcNode *nextarc;  // 指向下一条依附于同一顶点的边
} ArcNode;

typedef struct VNode {        // 顶点结点
    char data;                // 顶点信息
    ArcNode *firstarc;        // 指向第一条依附于该顶点的边
} VNode, AdjList[MaxSize];

typedef struct {
    AdjList vertices;     // 顶点表（数组）
    int vexnum, arcnum;   // 当前顶点数和边数
} ALGraph;

// 初始化图：顶点数、边数清零，所有顶点的邻接表置空
void InitALGraph(ALGraph *G) {
    G->vexnum = 0;
    G->arcnum = 0;
    for (int i = 0; i < MaxSize; i++) {
        G->vertices[i].data = '\0';
        G->vertices[i].firstarc = NULL;
    }
}

// 定位顶点：返回下标，未找到返回 -1
int LocateVex(ALGraph *G, char v) {
    for (int i = 0; i < G->vexnum; i++) {
        if (G->vertices[i].data == v) {
            return i;
        }
    }
    return -1;
}

// 创建边结点
static ArcNode *CreateArcNode(int adjvex, int weight) {
    ArcNode *node = (ArcNode *)malloc(sizeof(ArcNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->adjvex = adjvex;
    node->weight = weight;
    node->nextarc = NULL;
    return node;
}

// 插入顶点：返回下标，失败返回 -1
int InsertVertex(ALGraph *G, char v) {
    if (G->vexnum >= MaxSize) {
        printf("顶点数已达上限，插入失败！\n");
        return -1;
    }
    if (LocateVex(G, v) != -1) {
        printf("顶点 %c 已存在，插入失败！\n", v);
        return -1;
    }
    G->vertices[G->vexnum].data = v;
    G->vertices[G->vexnum].firstarc = NULL;
    return G->vexnum++;
}

// 插入边：采用“头插法”把边结点插入邻接表，插入顺序为 O(1)
// 对于无向图，需要在两个顶点的邻接表中各插入一个边结点
bool InsertEdge(ALGraph *G, char v1, char v2, int weight, bool isDirected) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) {
        printf("顶点 %c 或 %c 不存在，插入边失败！\n", v1, v2);
        return false;
    }
    // 插入 i -> j 的边
    ArcNode *node = CreateArcNode(j, weight);
    if (node == NULL) {
        return false;
    }
    node->nextarc = G->vertices[i].firstarc;
    G->vertices[i].firstarc = node;

    if (!isDirected) {  // 无向图还要插入 j -> i 的边
        ArcNode *back = CreateArcNode(i, weight);
        if (back == NULL) {
            return false;
        }
        back->nextarc = G->vertices[j].firstarc;
        G->vertices[j].firstarc = back;
    }
    G->arcnum++;
    return true;
}

// 判断两点之间是否有边：遍历其中一个顶点的邻接表
bool HasEdge(ALGraph *G, char v1, char v2) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) {
        return false;
    }
    for (ArcNode *p = G->vertices[i].firstarc; p != NULL; p = p->nextarc) {
        if (p->adjvex == j) {
            return true;
        }
    }
    return false;
}

// 求无向图中顶点 v 的度：邻接表中边结点的个数
int Degree(ALGraph *G, char v) {
    int i = LocateVex(G, v);
    if (i == -1) {
        return -1;
    }
    int degree = 0;
    for (ArcNode *p = G->vertices[i].firstarc; p != NULL; p = p->nextarc) {
        degree++;
    }
    return degree;
}

// 打印邻接表
void PrintGraph(ALGraph *G) {
    printf("邻接表（%d 个顶点，%d 条边）：\n", G->vexnum, G->arcnum);
    for (int i = 0; i < G->vexnum; i++) {
        printf("  %c(%d)", G->vertices[i].data, i);
        for (ArcNode *p = G->vertices[i].firstarc; p != NULL; p = p->nextarc) {
            printf(" -> %c(%d)", G->vertices[p->adjvex].data, p->weight);
        }
        printf("\n");
    }
}

// 销毁图：释放所有边结点
void DestroyGraph(ALGraph *G) {
    for (int i = 0; i < G->vexnum; i++) {
        ArcNode *p = G->vertices[i].firstarc;
        while (p != NULL) {
            ArcNode *next = p->nextarc;
            free(p);
            p = next;
        }
        G->vertices[i].firstarc = NULL;
    }
    G->vexnum = 0;
    G->arcnum = 0;
}

int main() {
    ALGraph G;
    InitALGraph(&G);

    // 建立有向图：顶点 A B C D，边 A->B、A->C、B->D、C->D
    char vs[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        InsertVertex(&G, vs[i]);
    }
    InsertEdge(&G, 'A', 'B', 1, true);
    InsertEdge(&G, 'A', 'C', 1, true);
    InsertEdge(&G, 'B', 'D', 1, true);
    InsertEdge(&G, 'C', 'D', 1, true);
    InsertVertex(&G, 'A');              // 重复插入顶点，应被拦截
    InsertEdge(&G, 'A', 'Z', 1, true);  // 顶点不存在，应被拦截

    PrintGraph(&G);
    printf("A 的出度：%d，D 的出度：%d\n", Degree(&G, 'A'), Degree(&G, 'D'));
    printf("A 到 B 是否有边：%s\n", HasEdge(&G, 'A', 'B') ? "是" : "否");
    printf("B 到 A 是否有边：%s（有向图中方向很重要）\n", HasEdge(&G, 'B', 'A') ? "是" : "否");

    DestroyGraph(&G);
    printf("销毁后顶点数：%d，边数：%d\n", G.vexnum, G.arcnum);
    return 0;
}
