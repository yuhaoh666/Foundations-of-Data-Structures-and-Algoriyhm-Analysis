#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100
// 无向图的邻接多重表存储结构

// 邻接多重表中的顶点结点
typedef struct VNode
{
    char data; // 顶点信息
    struct ArcNode *firstarc; // 指向第一条依附该顶点的弧的指针
} VNode, AMLGraph[MaxSize];

// 邻接多重表中的弧结点
typedef struct ArcNode
{
    int i, j; // 邻接点域，存储该边依附的两个顶点的下标
    // 其他信息域，如权值等 int weight;
    struct ArcNode *ilink; // 链域
    struct ArcNode *jlink; // 链域
} ArcNode;

typedef struct
{
    AMLGraph vertices; // 顶点表
    int vexnum, arcnum; // 图的当前顶点数和弧数
} AMLGraphType;

// 初始化图
void InitAMLGraph(AMLGraphType *G)
{
    int i;
    G->vexnum = 0;
    G->arcnum = 0;
    for (i = 0; i < MaxSize; i++)
    {
        G->vertices[i].data = 0;
        G->vertices[i].firstarc = NULL;
    }
}

