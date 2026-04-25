#include <stdio.h>
#define MaxSize 100

// 使用邻接矩阵存储结构
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

// 辅助队列
typedef struct
{
    char data[MaxSize];
    int front, rear;
} Queue;

void InitQueue(Queue *Q)
{
    Q->front = 0;
    Q->rear = 0;
}

bool IsEmpty(Queue *Q)
{
    return Q->front == Q->rear;
}

void EnQueue(Queue *Q, int x)
{
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear + 1) % MaxSize;
}

char DeQueue(Queue *Q)
{
    char x = Q->data[Q->front];
    Q->front = (Q->front + 1) % MaxSize;
    return x;
}

void visit(MGraph G, int i)
{
    printf("%c ", G.vexs[i]); // 访问顶点i，假设顶点信息是字符类型
}

int FirstNeighbor(MGraph G, int v)
{
    int i;
    for (i = 0; i < G.vexnum; i++)
    {
        if (G.arcs[v][i])
        {
            return i; // 返回第一个邻接点的下标
        }
    }
    return -1; // 没有邻接点
}

int NextNeighbor(MGraph G, int v, int w)
{
    int i;
    for (i = w + 1; i < G.vexnum; i++)
    {
        if (G.arcs[v][i])
        {
            return i; // 返回下一个邻接点的下标
        }
    }
    return -1; // 没有更多邻接点
}

void BFSTraverse(MGraph G)
{
    bool visited[G.vexnum];
    for (int i = 0; i < G.vexnum; i++)
        visited[i] = false;
    for (int v = 0; v < G.vexnum; v++)
        if (!visited[v])
            BFS(G, v, visited);
}

void BFS(MGraph G, int v, bool visited[]) // 从顶点v出发进行广度优先遍历
{
    Queue Q;
    InitQueue(&Q);
    visited[v] = true;
    EnQueue(&Q, v);
    while (!IsEmpty(&Q))
    {
        int u = DeQueue(&Q);
        visit(G, u); // 访问顶点u
        for (int w = FirstNeighbor(G, u); w >= 0; w = NextNeighbor(G, u, w))
        {
            if (!visited[w])
            {
                visited[w] = true;
                EnQueue(&Q, w);
            }
        }
    }
}

void DFS(MGraph G, int v, bool visited[])
{
    visited[v] = true;  // 标记当前顶点已访问
    visit(G, v);        // 访问当前顶点
    
    // 递归访问所有未访问的邻接点
    for (int w = FirstNeighbor(G, v); w >= 0; w = NextNeighbor(G, v, w))
        if (!visited[w])
            DFS(G, w, visited); // 递归遍历邻接点
}

void DFSTraverse(MGraph G)
{
    bool visited[G.vexnum];
    for (int i = 0; i < G.vexnum; i++)
        visited[i] = false;
    for (int v = 0; v < G.vexnum; v++)
        if (!visited[v])
            DFS(G, v, visited);
}

