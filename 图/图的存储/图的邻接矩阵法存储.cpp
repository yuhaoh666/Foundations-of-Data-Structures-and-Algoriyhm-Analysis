#include <stdio.h>
#define MaxSize 100
// 图的邻接矩阵存储结构

typedef struct
{
    char vexs[MaxSize]; // 顶点表
    bool arcs[MaxSize][MaxSize]; // 邻接矩阵
    int vexnum, arcnum; // 图的当前顶点数和弧数
} MGraph;

// 初始化图
void InitMGraph(MGraph *G)
{
    int i, j;
    G->vexnum = 0;
    G->arcnum = 0;
    for (i = 0; i < MaxSize; i++)
    {
        for (j = 0; j < MaxSize; j++)
        {
            G->arcs[i][j] = false;
        }
    }
}

