#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100

// 图的邻接表存储结构

typedef struct ArcNode
{
    int adjvex; // 邻接点域，存储该顶点对应的下标
    // 其他信息域，如权值等 int weight;
    struct ArcNode *nextarc; // 链域，指向下一个邻接点
} ArcNode;

typedef struct VBox
{
    char data; // 顶点信息
    ArcNode *firstarc; // 指向第一条依附该顶点的弧的指针
} VBox, AdjList[MaxSize];

typedef struct
{
    AdjList vertices; // 顶点表
    int vexnum, arcnum; // 图的当前顶点数和弧数
} ALGraph;

// 初始化图
void InitALGraph(ALGraph *G)
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

