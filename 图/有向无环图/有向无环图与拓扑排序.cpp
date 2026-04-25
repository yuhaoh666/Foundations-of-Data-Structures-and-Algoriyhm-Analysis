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

void InitALGraph(ALGraph *G)  // 初始化图
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

// 有向无环图的拓扑排序
bool Topologicalsort(ALGraph G)
{
    int inDegree[MaxSize] = {0}; // 存储每个顶点的入度
    int queue[MaxSize];          // 队列，存储入度为0的顶点下标
    int front = 0, rear = 0;     // 队列头尾指针
    int count = 0;               // 记录拓扑排序输出的顶点数
    int i;
    ArcNode *p;

    // 第一步：计算每个顶点的入度
    for (i = 0; i < G.vexnum; i++)
    {
        p = G.vertices[i].firstarc; // 遍历顶点i的所有出边
        while (p != NULL)
        {
            inDegree[p->adjvex]++;  // 邻接点的入度+1
            p = p->nextarc;
        }
    }

    // 第二步：将所有入度为0的顶点入队
    for (i = 0; i < G.vexnum; i++)
        if (inDegree[i] == 0)
            queue[rear++] = i;

    // 第三步：处理队列中的顶点，生成拓扑序列
    while (front != rear) // 队列非空
    {
        int u = queue[front++]; // 出队
        printf("%c ", G.vertices[u].data); // 输出当前顶点（可根据需求修改输出形式）
        count++;                          // 计数+1

        // 遍历u的所有邻接点，将其入度减1
        p = G.vertices[u].firstarc;
        while (p != NULL)
        {
            int v = p->adjvex;
            inDegree[v]--;          // 邻接点v的入度-1
            if (inDegree[v] == 0)   // 若入度变为0，入队
                queue[rear++] = v;
            p = p->nextarc;
        }
    }

    // 第四步：判断是否拓扑排序成功（无环）
    if (count == G.vexnum)
    {
        printf("\n拓扑排序成功，无环\n");
        return true;
    }
    else
    {
        printf("\n拓扑排序失败，图中存在环，仅输出%d个顶点\n", count);
        return false;
    }
}

// 测试用例（可选）：向图中添加顶点和弧
void AddVertex(ALGraph *G, int index, char data)
{
    G->vertices[index].data = data;
    G->vexnum++;
}

void AddArc(ALGraph *G, int v1, int v2)
{
    ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
    p->adjvex = v2;
    p->nextarc = G->vertices[v1].firstarc; // 头插法（顺序不影响拓扑排序）
    G->vertices[v1].firstarc = p;
    G->arcnum++;
}

// 主函数测试
int main()
{
    ALGraph G;
    InitALGraph(&G);

    // 添加顶点（下标0-4）
    AddVertex(&G, 0, 'A');
    AddVertex(&G, 1, 'B');
    AddVertex(&G, 2, 'C');
    AddVertex(&G, 3, 'D');
    AddVertex(&G, 4, 'E');

    // 添加有向弧：A->B, A->C, B->D, C->D, D->E
    AddArc(&G, 0, 1);
    AddArc(&G, 0, 2);
    AddArc(&G, 1, 3);
    AddArc(&G, 2, 3);
    AddArc(&G, 3, 4);

    // 执行拓扑排序
    Topologicalsort(G);

    return 0;
}