// 有向图的十字链表存储：把“出弧表”和“入弧表”结合成一张结构（邻接表只能方便地表示出弧）
// 顶点结点：data、firstin（第一条入弧）、firstout（第一条出弧）
// 弧结点：tailvex（弧尾下标）、headvex（弧头下标）、
//         hlink（下一个弧头相同的弧）、tlink（下一个弧尾相同的弧）
// 优点：求某顶点的出度/入度只需遍历对应链表，删除弧也只需 O(1) 的指针操作
#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100

typedef struct ArcNode {           // 弧结点
    int tailvex, headvex;          // 弧尾、弧头的顶点下标
    int weight;                    // 权值
    struct ArcNode *hlink, *tlink;  // 弧头相同的下一条弧、弧尾相同的下一条弧
} ArcNode;

typedef struct VNode {   // 顶点结点
    char data;           // 顶点信息
    ArcNode *firstin;    // 指向第一条以该顶点为弧头的弧
    ArcNode *firstout;   // 指向第一条以该顶点为弧尾的弧
} VNode, CrossList[MaxSize];

typedef struct {
    CrossList vertices;   // 顶点表
    int vexnum, arcnum;   // 当前顶点数和弧数
} OLGraph;

// 初始化
void InitOLGraph(OLGraph *G) {
    G->vexnum = 0;
    G->arcnum = 0;
    for (int i = 0; i < MaxSize; i++) {
        G->vertices[i].data = '\0';
        G->vertices[i].firstin = NULL;
        G->vertices[i].firstout = NULL;
    }
}

// 定位顶点
int LocateVex(OLGraph *G, char v) {
    for (int i = 0; i < G->vexnum; i++) {
        if (G->vertices[i].data == v) {
            return i;
        }
    }
    return -1;
}

// 插入顶点
int InsertVertex(OLGraph *G, char v) {
    if (G->vexnum >= MaxSize || LocateVex(G, v) != -1) {
        printf("顶点 %c 无法插入（已存在或已达上限）\n", v);
        return -1;
    }
    G->vertices[G->vexnum].data = v;
    G->vertices[G->vexnum].firstin = NULL;
    G->vertices[G->vexnum].firstout = NULL;
    return G->vexnum++;
}

// 插入弧：<tail, head>，把它分别头插到弧尾的出弧表和弧头的入弧表
bool InsertArc(OLGraph *G, char tail, char head, int weight) {
    int i = LocateVex(G, tail);
    int j = LocateVex(G, head);
    if (i == -1 || j == -1) {
        printf("顶点 %c 或 %c 不存在，插入弧失败！\n", tail, head);
        return false;
    }
    ArcNode *arc = (ArcNode *)malloc(sizeof(ArcNode));
    if (arc == NULL) {
        printf("内存分配失败！\n");
        return false;
    }
    arc->tailvex = i;
    arc->headvex = j;
    arc->weight = weight;
    // 头插法挂到弧尾 i 的“出弧表”（用 tlink 链接）
    arc->tlink = G->vertices[i].firstout;
    G->vertices[i].firstout = arc;
    // 头插法挂到弧头 j 的“入弧表”（用 hlink 链接）
    arc->hlink = G->vertices[j].firstin;
    G->vertices[j].firstin = arc;
    G->arcnum++;
    return true;
}

// 求顶点的出度 / 入度
int OutDegree(OLGraph *G, char v) {
    int i = LocateVex(G, v);
    if (i == -1) {
        return -1;
    }
    int degree = 0;
    for (ArcNode *p = G->vertices[i].firstout; p != NULL; p = p->tlink) {
        degree++;
    }
    return degree;
}

int InDegree(OLGraph *G, char v) {
    int i = LocateVex(G, v);
    if (i == -1) {
        return -1;
    }
    int degree = 0;
    for (ArcNode *p = G->vertices[i].firstin; p != NULL; p = p->hlink) {
        degree++;
    }
    return degree;
}

// 判断弧 <tail, head> 是否存在
bool HasArc(OLGraph *G, char tail, char head) {
    int i = LocateVex(G, tail);
    int j = LocateVex(G, head);
    if (i == -1 || j == -1) {
        return false;
    }
    for (ArcNode *p = G->vertices[i].firstout; p != NULL; p = p->tlink) {
        if (p->headvex == j) {
            return true;
        }
    }
    return false;
}

// 打印十字链表：分别给出每个顶点的出弧表和入弧表
void PrintGraph(OLGraph *G) {
    printf("十字链表（%d 个顶点，%d 条弧）：\n", G->vexnum, G->arcnum);
    for (int i = 0; i < G->vexnum; i++) {
        printf("  %c 的出弧：", G->vertices[i].data);
        for (ArcNode *p = G->vertices[i].firstout; p != NULL; p = p->tlink) {
            printf("<%c,%c>%d ", G->vertices[p->tailvex].data, G->vertices[p->headvex].data,
                   p->weight);
        }
        printf("\n  %c 的入弧：", G->vertices[i].data);
        for (ArcNode *p = G->vertices[i].firstin; p != NULL; p = p->hlink) {
            printf("<%c,%c>%d ", G->vertices[p->tailvex].data, G->vertices[p->headvex].data,
                   p->weight);
        }
        printf("\n");
    }
}

// 销毁图：每条弧只出现在一个顶点的“出弧链”上，遍历所有出弧链即可覆盖全部弧结点；
// 这里再借助一个数组去重，避免任何重复释放的可能
void DestroyGraph(OLGraph *G) {
    ArcNode *arcs[MaxSize];
    int count = 0;
    for (int i = 0; i < G->vexnum; i++) {
        // 沿着属于顶点 i 的那条链走：i 是弧尾则走 tlink
        for (ArcNode *p = G->vertices[i].firstout; p != NULL;
             p = (p->tailvex == i) ? p->tlink : p->hlink) {
            bool found = false;
            for (int k = 0; k < count; k++) {
                if (arcs[k] == p) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                arcs[count++] = p;
            }
        }
        G->vertices[i].firstin = NULL;
        G->vertices[i].firstout = NULL;
    }
    for (int k = 0; k < count; k++) {
        free(arcs[k]);
    }
    G->vexnum = 0;
    G->arcnum = 0;
}

int main() {
    OLGraph G;
    InitOLGraph(&G);

    // 顶点 A B C D，弧 A->B、A->C、B->D、C->D、D->A
    char vs[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        InsertVertex(&G, vs[i]);
    }
    InsertArc(&G, 'A', 'B', 1);
    InsertArc(&G, 'A', 'C', 1);
    InsertArc(&G, 'B', 'D', 1);
    InsertArc(&G, 'C', 'D', 1);
    InsertArc(&G, 'D', 'A', 1);
    InsertArc(&G, 'A', 'Z', 1);  // 顶点不存在，应被拦截

    PrintGraph(&G);
    for (int i = 0; i < G.vexnum; i++) {
        printf("顶点 %c：出度 %d，入度 %d\n", G.vertices[i].data, OutDegree(&G, G.vertices[i].data),
               InDegree(&G, G.vertices[i].data));
    }
    printf("是否存在弧 <A,C>：%s，是否存在弧 <C,A>：%s\n",
           HasArc(&G, 'A', 'C') ? "是" : "否", HasArc(&G, 'C', 'A') ? "是" : "否");

    DestroyGraph(&G);
    printf("销毁后顶点数：%d，弧数：%d\n", G.vexnum, G.arcnum);
    return 0;
}
