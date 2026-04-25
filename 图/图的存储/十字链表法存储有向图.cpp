#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100
// 图的十字链表存储结构
// 十字链表中的顶点结点
typedef struct VNode
{
    char data; // 顶点信息
    struct ArcNode *firstin; // 指向第一条以该顶点为终点的弧的指针
    struct ArcNode *firstout; // 指向第一条以该顶点为起点的弧的指针
} VNode, CrossList[MaxSize];

// 十字链表中的弧结点
typedef struct ArcNode
{
    int tailvex; // 弧尾顶点下标
    int headvex; // 弧头顶点下标
    // 其他信息域，如权值等 int weight;
    struct ArcNode *tlink; // 链域，指向下一个以相同弧尾的弧结点
    struct ArcNode *hlink; // 链域，指向下一个以相同弧头的弧结点    
} ArcNode;

typedef struct
{
    CrossList vertices; // 顶点表
    int vexnum, arcnum; // 图的当前顶点数和弧数
} CLGraph;

// 初始化图
void InitCLGraph(CLGraph *G)
{
    int i;
    G->vexnum = 0;
    G->arcnum = 0;
    for (i = 0; i < MaxSize; i++)
    {
        G->vertices[i].data = 0;
        G->vertices[i].firstin = NULL;
        G->vertices[i].firstout = NULL;
    }
}