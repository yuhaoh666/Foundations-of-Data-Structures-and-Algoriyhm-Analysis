// 图的遍历：广度优先遍历（BFS）与深度优先遍历（DFS）
//   BFS：借助队列，先访问离起点近的顶点，类似树的层次遍历
//   DFS：借助递归（或栈），一条路走到黑，类似树的先序遍历
// 两者时间复杂度相同：邻接矩阵实现 O(n²)，邻接表实现 O(n+e)
// 下面用邻接矩阵存储，并给出从某点出发的访问序列以及整图的连通分量个数
#include <stdio.h>
#define MaxSize 100

typedef struct {
    char vexs[MaxSize];          // 顶点表
    int arcs[MaxSize][MaxSize];  // 邻接矩阵：非 0 表示有边（这里存权值）
    int vexnum, arcnum;          // 顶点数、边数
} MGraph;

// ==================== 辅助队列（存顶点下标） ====================
typedef struct {
    int data[MaxSize];
    int front, rear;  // 队头、队尾指针（队尾指向下一个可写位置）
} Queue;

void InitQueue(Queue *Q) {
    Q->front = Q->rear = 0;
}

bool IsEmptyQueue(Queue *Q) {
    return Q->front == Q->rear;
}

bool EnQueue(Queue *Q, int x) {
    if ((Q->rear + 1) % MaxSize == Q->front) {  // 队满（牺牲一个单元判满）
        return false;
    }
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear + 1) % MaxSize;
    return true;
}

bool DeQueue(Queue *Q, int &x) {
    if (IsEmptyQueue(Q)) {
        return false;
    }
    x = Q->data[Q->front];
    Q->front = (Q->front + 1) % MaxSize;
    return true;
}

// ==================== 图的基本操作 ====================
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

int LocateVex(MGraph *G, char v) {
    for (int i = 0; i < G->vexnum; i++) {
        if (G->vexs[i] == v) {
            return i;
        }
    }
    return -1;
}

int InsertVertex(MGraph *G, char v) {
    if (G->vexnum >= MaxSize) {
        return -1;
    }
    G->vexs[G->vexnum] = v;
    return G->vexnum++;
}

bool InsertEdge(MGraph *G, char v1, char v2, int weight, bool isDirected) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) {
        return false;
    }
    G->arcs[i][j] = weight;
    if (!isDirected) {
        G->arcs[j][i] = weight;
    }
    G->arcnum++;
    return true;
}

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

// ==================== 遍历相关辅助函数 ====================
// 访问顶点 v
void Visit(MGraph *G, int v) {
    printf("%c ", G->vexs[v]);
}

// 求顶点 v 的第一个邻接点下标，没有则返回 -1
int FirstNeighbor(MGraph *G, int v) {
    for (int i = 0; i < G->vexnum; i++) {
        if (G->arcs[v][i] != 0) {
            return i;
        }
    }
    return -1;
}

// 求顶点 v 在顶点 w 之后的下一个邻接点下标，没有则返回 -1
int NextNeighbor(MGraph *G, int v, int w) {
    for (int i = w + 1; i < G->vexnum; i++) {
        if (G->arcs[v][i] != 0) {
            return i;
        }
    }
    return -1;
}

// 把访问标记数组全部置为 false
void ResetVisited(MGraph *G, bool visited[]) {
    for (int i = 0; i < G->vexnum; i++) {
        visited[i] = false;
    }
}

// ==================== 广度优先遍历（BFS） ====================
// 从顶点 v 出发：v 入队，然后不断取出队头，把队头所有未访问的邻接点访问并入队
void BFS(MGraph *G, int v, bool visited[]) {
    Queue Q;
    InitQueue(&Q);
    Visit(G, v);         // 先访问起点
    visited[v] = true;   // 立刻标记为已访问，避免同一顶点重复入队
    EnQueue(&Q, v);
    while (!IsEmptyQueue(&Q)) {
        int u;
        DeQueue(&Q, u);  // 取出队头顶点
        for (int w = FirstNeighbor(G, u); w >= 0; w = NextNeighbor(G, u, w)) {
            if (!visited[w]) {  // 逐个访问未访问的邻接点
                Visit(G, w);
                visited[w] = true;
                EnQueue(&Q, w);
            }
        }
    }
}

// ==================== 深度优先遍历（DFS） ====================
// 从顶点 v 出发：访问 v 后递归地深入到每一个未访问的邻接点
void DFS(MGraph *G, int v, bool visited[]) {
    Visit(G, v);
    visited[v] = true;
    for (int w = FirstNeighbor(G, v); w >= 0; w = NextNeighbor(G, v, w)) {
        if (!visited[w]) {
            DFS(G, w, visited);  // 递归深入
        }
    }
}

// 整图的广度优先遍历：对非连通图也能访问到所有顶点，返回连通分量的个数
int BFSTraverse(MGraph *G) {
    bool visited[MaxSize];
    ResetVisited(G, visited);
    int components = 0;
    for (int v = 0; v < G->vexnum; v++) {
        if (!visited[v]) {  // 发现一个尚未访问的顶点，说明出现了新的连通分量
            if (components > 0) {
                printf("| ");  // 用竖线分隔不同的连通分量
            }
            components++;
            BFS(G, v, visited);
        }
    }
    return components;
}

// 整图的深度优先遍历
int DFSTraverse(MGraph *G) {
    bool visited[MaxSize];
    ResetVisited(G, visited);
    int components = 0;
    for (int v = 0; v < G->vexnum; v++) {
        if (!visited[v]) {
            if (components > 0) {
                printf("| ");
            }
            components++;
            DFS(G, v, visited);
        }
    }
    return components;
}

int main() {
    MGraph G;
    InitMGraph(&G);

    // 无向图：顶点 A~F，边 AB、AC、BD、CD、EF
    // 它有两个连通分量：{A,B,C,D} 与 {E,F}
    char vs[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < 6; i++) {
        InsertVertex(&G, vs[i]);
    }
    InsertEdge(&G, 'A', 'B', 1, false);
    InsertEdge(&G, 'A', 'C', 1, false);
    InsertEdge(&G, 'B', 'D', 1, false);
    InsertEdge(&G, 'C', 'D', 1, false);
    InsertEdge(&G, 'E', 'F', 1, false);
    PrintMatrix(&G);

    bool visited[MaxSize];

    // 1. 从 A 出发的 BFS
    ResetVisited(&G, visited);
    printf("从 A 出发的广度优先遍历：");
    BFS(&G, LocateVex(&G, 'A'), visited);
    printf("\n");

    // 2. 从 A 出发的 DFS
    ResetVisited(&G, visited);
    printf("从 A 出发的深度优先遍历：");
    DFS(&G, LocateVex(&G, 'A'), visited);
    printf("\n");

    // 3. 整图遍历：非连通图也能访问到全部顶点，并统计连通分量
    printf("整图广度优先遍历：");
    int components = BFSTraverse(&G);
    printf("\n整图深度优先遍历：");
    DFSTraverse(&G);
    printf("\n连通分量个数：%d\n", components);
    printf("（提示：邻接点的先后次序会影响遍历序列，所以 BFS/DFS 序列并不唯一）\n");
    return 0;
}
