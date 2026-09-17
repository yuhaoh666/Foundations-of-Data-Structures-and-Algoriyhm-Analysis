// 无向图的邻接多重表存储：每条边只用一个结点表示
//（普通邻接表中同一条无向边要存两个结点，删除边/顶点很不方便）
// 顶点结点：data、firstarc（第一条依附于该顶点的边）
// 边结点：ivex、jvex（该边依附的两个顶点下标）、
//         ilink（依附于 ivex 的下一条边）、jlink（依附于 jvex 的下一条边）
#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100

typedef struct ArcNode {            // 边结点
    int ivex, jvex;                 // 该边依附的两个顶点下标
    int weight;                     // 权值（无权图可统一取 1）
    struct ArcNode *ilink, *jlink;  // 依附于 ivex / jvex 的下一条边
} ArcNode;

typedef struct VNode {   // 顶点结点
    char data;           // 顶点信息
    ArcNode *firstarc;   // 指向第一条依附于该顶点的边
} VNode, AMLList[MaxSize];

typedef struct {
    AMLList vertices;     // 顶点表
    int vexnum, arcnum;   // 当前顶点数和边数
} AMLGraph;

// 初始化
void InitAMLGraph(AMLGraph *G) {
    G->vexnum = 0;
    G->arcnum = 0;
    for (int i = 0; i < MaxSize; i++) {
        G->vertices[i].data = '\0';
        G->vertices[i].firstarc = NULL;
    }
}

// 定位顶点
int LocateVex(AMLGraph *G, char v) {
    for (int i = 0; i < G->vexnum; i++) {
        if (G->vertices[i].data == v) {
            return i;
        }
    }
    return -1;
}

// 插入顶点
int InsertVertex(AMLGraph *G, char v) {
    if (G->vexnum >= MaxSize || LocateVex(G, v) != -1) {
        printf("顶点 %c 无法插入（已存在或已达上限）\n", v);
        return -1;
    }
    G->vertices[G->vexnum].data = v;
    G->vertices[G->vexnum].firstarc = NULL;
    return G->vexnum++;
}

// 插入无向边 (v1, v2)：只创建一个边结点，同时挂到两个顶点的边链表中
bool InsertEdge(AMLGraph *G, char v1, char v2, int weight) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) {
        printf("顶点 %c 或 %c 不存在，插入边失败！\n", v1, v2);
        return false;
    }
    ArcNode *edge = (ArcNode *)malloc(sizeof(ArcNode));
    if (edge == NULL) {
        printf("内存分配失败！\n");
        return false;
    }
    edge->ivex = i;
    edge->jvex = j;
    edge->weight = weight;
    // 头插到顶点 i 的边链表（用 ilink 链接）
    edge->ilink = G->vertices[i].firstarc;
    G->vertices[i].firstarc = edge;
    // 头插到顶点 j 的边链表（用 jlink 链接）
    edge->jlink = G->vertices[j].firstarc;
    G->vertices[j].firstarc = edge;
    G->arcnum++;
    return true;
}

// 判断两点之间是否有边：遍历其中一个顶点的边链表
bool HasEdge(AMLGraph *G, char v1, char v2) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) {
        return false;
    }
    for (ArcNode *p = G->vertices[i].firstarc; p != NULL;
         p = (p->ivex == i) ? p->ilink : p->jlink) {
        if ((p->ivex == i && p->jvex == j) || (p->ivex == j && p->jvex == i)) {
            return true;
        }
    }
    return false;
}

// 求顶点 v 的度：它的边链表上有几条边
int Degree(AMLGraph *G, char v) {
    int i = LocateVex(G, v);
    if (i == -1) {
        return -1;
    }
    int degree = 0;
    for (ArcNode *p = G->vertices[i].firstarc; p != NULL;
         p = (p->ivex == i) ? p->ilink : p->jlink) {
        degree++;
    }
    return degree;
}

// 打印邻接多重表：每个顶点后面列出它所依附的边（另一个端点）
void PrintGraph(AMLGraph *G) {
    printf("邻接多重表（%d 个顶点，%d 条边）：\n", G->vexnum, G->arcnum);
    for (int i = 0; i < G->vexnum; i++) {
        printf("  %c 依附的边：", G->vertices[i].data);
        for (ArcNode *p = G->vertices[i].firstarc; p != NULL;
             p = (p->ivex == i) ? p->ilink : p->jlink) {
            int other = (p->ivex == i) ? p->jvex : p->ivex;  // 边的另一个端点
            printf("(%c,%c)%d ", G->vertices[i].data, G->vertices[other].data, p->weight);
        }
        printf("\n");
    }
}

// 销毁图：边结点同时挂在两个顶点的链上，必须先收集、去重，再统一释放，
// 否则遍历另一条链时会访问到已释放的内存
void DestroyGraph(AMLGraph *G) {
    ArcNode *edges[MaxSize];
    int count = 0;
    for (int i = 0; i < G->vexnum; i++) {
        for (ArcNode *p = G->vertices[i].firstarc; p != NULL;
             p = (p->ivex == i) ? p->ilink : p->jlink) {
            bool found = false;
            for (int k = 0; k < count; k++) {
                if (edges[k] == p) {  // 该边已经记录过（从另一个端点访问而来）
                    found = true;
                    break;
                }
            }
            if (!found) {
                edges[count++] = p;
            }
        }
        G->vertices[i].firstarc = NULL;
    }
    for (int k = 0; k < count; k++) {
        free(edges[k]);
    }
    G->vexnum = 0;
    G->arcnum = 0;
}

int main() {
    AMLGraph G;
    InitAMLGraph(&G);

    // 无向图：顶点 A B C D，边 AB、AC、BC、CD
    char vs[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        InsertVertex(&G, vs[i]);
    }
    InsertEdge(&G, 'A', 'B', 1);
    InsertEdge(&G, 'A', 'C', 1);
    InsertEdge(&G, 'B', 'C', 1);
    InsertEdge(&G, 'C', 'D', 1);
    InsertEdge(&G, 'A', 'Z', 1);  // 顶点不存在，应被拦截

    PrintGraph(&G);
    printf("各顶点的度：");
    for (int i = 0; i < G.vexnum; i++) {
        printf("%c:%d  ", G.vertices[i].data, Degree(&G, G.vertices[i].data));
    }
    printf("\nA 与 B 是否有边：%s，A 与 D 是否有边：%s\n",
           HasEdge(&G, 'A', 'B') ? "是" : "否", HasEdge(&G, 'A', 'D') ? "是" : "否");

    DestroyGraph(&G);
    printf("销毁后顶点数：%d，边数：%d\n", G.vexnum, G.arcnum);
    return 0;
}
