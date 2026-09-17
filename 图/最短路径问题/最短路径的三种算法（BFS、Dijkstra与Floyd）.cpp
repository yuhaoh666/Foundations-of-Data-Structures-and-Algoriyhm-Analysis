// 求解最短路径的三个算法
//   1. BFS      ：无权图（或边权相同的图）的单源最短路径，时间复杂度 O(n+e)
//   2. Dijkstra ：带权非负图的单源最短路径，时间复杂度 O(n²)
//   3. Floyd    ：任意两点之间的最短路径（动态规划），时间复杂度 O(n³)，
//                 允许负权边但不能有负权回路
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXV 100  // 顶点数上限（用于数组模拟队列与栈）

// ==================== 邻接表存储（供 BFS 和 Dijkstra 使用） ====================
typedef struct AdjNode {    // 边结点
    int vertex;             // 邻接顶点编号
    int weight;             // 边权（无权图可统一取 1）
    struct AdjNode *next;   // 下一条边
} AdjNode;

typedef struct {
    AdjNode *head;          // 每个顶点对应一条邻接链表
} AdjList;

typedef struct {
    int numVertices;        // 顶点总数
    AdjList *array;         // 邻接表数组
} Graph;

// 创建图：分配邻接表并置空
Graph *CreateGraph(int vertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->numVertices = vertices;
    graph->array = (AdjList *)malloc(vertices * sizeof(AdjList));
    if (graph->array == NULL) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < vertices; i++) {
        graph->array[i].head = NULL;
    }
    return graph;
}

// 创建边结点
static AdjNode *CreateAdjNode(int v, int weight) {
    AdjNode *node = (AdjNode *)malloc(sizeof(AdjNode));
    if (node == NULL) {
        return NULL;
    }
    node->vertex = v;
    node->weight = weight;
    node->next = NULL;
    return node;
}

// 添加边：isDirected 为 1 表示有向边，为 0 表示无向边（对称添加）
bool AddEdge(Graph *graph, int src, int dest, int weight, int isDirected) {
    AdjNode *node = CreateAdjNode(dest, weight);  // src -> dest
    if (node == NULL) {
        return false;
    }
    node->next = graph->array[src].head;  // 头插
    graph->array[src].head = node;

    if (!isDirected) {  // 无向图还要添加 dest -> src
        AdjNode *back = CreateAdjNode(src, weight);
        if (back == NULL) {
            return false;
        }
        back->next = graph->array[dest].head;
        graph->array[dest].head = back;
    }
    return true;
}

// 释放图占用的内存
void FreeGraph(Graph *graph) {
    if (graph == NULL) {
        return;
    }
    for (int i = 0; i < graph->numVertices; i++) {
        AdjNode *p = graph->array[i].head;
        while (p != NULL) {
            AdjNode *next = p->next;
            free(p);
            p = next;
        }
    }
    free(graph->array);
    free(graph);
}

// ==================== 路径打印 ====================
// 根据前驱数组 prev 打印 start 到 v 的路径
// prev[cur] 表示 cur 在最短路径上的前驱，起点的前驱为 -1
void PrintPathByPrev(const int prev[], int start, int v) {
    int path[MAXV];
    int len = 0;
    for (int cur = v; cur != -1 && len < MAXV; cur = prev[cur]) {
        path[len++] = cur;  // 从终点沿前驱一路退到起点（得到的是逆序路径）
    }
    if (len == 0 || path[len - 1] != start) {  // 没退到起点说明两点不连通
        printf("(不可达)");
        return;
    }
    for (int i = len - 1; i >= 0; i--) {  // 倒序输出即为正序路径
        printf("%d", path[i]);
        if (i > 0) {
            printf(" -> ");
        }
    }
}

// ==================== 1. BFS：无权图的单源最短路径 ====================
void BFS(Graph *graph, int start) {
    int n = graph->numVertices;
    int *dist = (int *)malloc(n * sizeof(int));     // 起点到各点的最少边数
    int *prev = (int *)malloc(n * sizeof(int));     // 最短路径上的前驱
    int *visited = (int *)malloc(n * sizeof(int));  // 访问标记
    int *queue = (int *)malloc(n * sizeof(int));    // 数组模拟队列
    if (dist == NULL || prev == NULL || visited == NULL || queue == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        dist[i] = -1;  // -1 表示不可达
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;
    visited[start] = 1;
    int front = 0, rear = 0;
    queue[rear++] = start;

    while (front < rear) {
        int u = queue[front++];  // 取出队头
        for (AdjNode *p = graph->array[u].head; p != NULL; p = p->next) {
            int v = p->vertex;
            if (!visited[v]) {          // BFS 中第一次访问到 v 一定是最短路径
                visited[v] = 1;
                dist[v] = dist[u] + 1;  // 无权图的“长度”就是边数
                prev[v] = u;
                queue[rear++] = v;
            }
        }
    }

    printf("=== BFS：无权图从顶点 %d 出发的最短路径（以边数计）===\n", start);
    for (int i = 0; i < n; i++) {
        printf("  %d -> %d：", start, i);
        if (dist[i] == -1) {
            printf("不可达\n");
        } else {
            printf("最少边数 = %d，路径 ", dist[i]);
            PrintPathByPrev(prev, start, i);
            printf("\n");
        }
    }
    free(dist);
    free(prev);
    free(visited);
    free(queue);
}

// ==================== 2. Dijkstra：带权非负图的单源最短路径 ====================
// 思路：把顶点分成“已确定最短路径”和“未确定”两类，
//       每轮从未确定集合中取 dist 最小的顶点 u 并确定下来，再用 u 去松弛它的邻接点
void Dijkstra(Graph *graph, int start) {
    int n = graph->numVertices;
    int *dist = (int *)malloc(n * sizeof(int));     // 起点到各点的最短距离
    int *prev = (int *)malloc(n * sizeof(int));     // 最短路径上的前驱
    int *visited = (int *)malloc(n * sizeof(int));  // 最短距离是否已确定
    if (dist == NULL || prev == NULL || visited == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;  // INT_MAX 表示暂时不可达
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int i = 0; i < n; i++) {
        // (1) 在未确定的顶点中挑出 dist 最小者
        int u = -1, minDist = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        if (u == -1) {  // 剩下的顶点都不可达
            break;
        }
        visited[u] = 1;  // u 的最短距离已经确定
        // (2) 以 u 作为中转，松弛它的所有邻接点
        for (AdjNode *p = graph->array[u].head; p != NULL; p = p->next) {
            int v = p->vertex;
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + p->weight < dist[v]) {
                dist[v] = dist[u] + p->weight;
                prev[v] = u;
            }
        }
    }

    printf("\n=== Dijkstra：带权非负图从顶点 %d 出发的最短路径 ===\n", start);
    for (int i = 0; i < n; i++) {
        printf("  %d -> %d：", start, i);
        if (dist[i] == INT_MAX) {
            printf("不可达\n");
        } else {
            printf("距离 = %d，路径 ", dist[i]);
            PrintPathByPrev(prev, start, i);
            printf("\n");
        }
    }
    free(dist);
    free(prev);
    free(visited);
}

// ==================== 3. Floyd：任意两点之间的最短路径 ====================
// 打印 Floyd 结果中 i 到 j 的路径：沿 prev[i][*] 从 j 一路退回 i
void PrintFloydPath(int **prev, int i, int j) {
    int path[MAXV];
    int len = 0;
    for (int cur = j; cur != -1 && cur != i && len < MAXV; cur = prev[i][cur]) {
        path[len++] = cur;  // 逆序记录中间顶点
    }
    printf("%d", i);  // 先输出起点
    for (int k = len - 1; k >= 0; k--) {
        printf(" -> %d", path[k]);
    }
}

// graph 为邻接矩阵：graph[i][j] 是边权，INT_MAX 表示没有边，约定 graph[i][i] = 0
void Floyd(int **graph, int n) {
    int **dist = (int **)malloc(n * sizeof(int *));  // 最短距离矩阵
    int **prev = (int **)malloc(n * sizeof(int *));  // 前驱矩阵：prev[i][j] 表示 i->j 路径上 j 的前驱
    if (dist == NULL || prev == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        dist[i] = (int *)malloc(n * sizeof(int));
        prev[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            dist[i][j] = graph[i][j];
            // 初始时只允许直达：i->j 有边则 j 的前驱就是 i
            prev[i][j] = (i != j && graph[i][j] != INT_MAX) ? i : -1;
        }
    }

    // 三重循环：外层 k 表示“允许经过顶点 k 作为中转”，这是动态规划的思想
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] == INT_MAX || dist[k][j] == INT_MAX) {
                    continue;  // 不检查会溢出，INT_MAX + 权值 会变成负数
                }
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    prev[i][j] = prev[k][j];  // 新路径中 j 的前驱，沿用 k->j 路径上 j 的前驱
                }
            }
        }
    }

    printf("\n=== Floyd：任意两点之间的最短路径 ===\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("  %d -> %d：", i, j);
            if (dist[i][j] == INT_MAX) {
                printf("不可达\n");
            } else {
                printf("距离 = %d，路径 ", dist[i][j]);
                PrintFloydPath(prev, i, j);
                printf("\n");
            }
        }
    }

    for (int i = 0; i < n; i++) {  // 释放二维数组
        free(dist[i]);
        free(prev[i]);
    }
    free(dist);
    free(prev);
}

int main() {
    // ---------- 测试 1：BFS 与 Dijkstra（用邻接表存有向带权图） ----------
    int n1 = 5;
    Graph *graph = CreateGraph(n1);
    if (graph == NULL) {
        printf("创建图失败！\n");
        return 0;
    }
    AddEdge(graph, 0, 1, 2, 1);
    AddEdge(graph, 0, 2, 1, 1);
    AddEdge(graph, 1, 3, 1, 1);
    AddEdge(graph, 2, 1, 3, 1);
    AddEdge(graph, 2, 3, 4, 1);
    AddEdge(graph, 3, 4, 2, 1);
    printf("有向图的边：0->1(2) 0->2(1) 1->3(1) 2->1(3) 2->3(4) 3->4(2)\n");
    BFS(graph, 0);       // BFS 忽略权值，按边数计算
    Dijkstra(graph, 0);  // Dijkstra 考虑权值

    // ---------- 测试 2：Floyd（用邻接矩阵存有向带权图） ----------
    int n2 = 4;
    int **matrix = (int **)malloc(n2 * sizeof(int *));
    for (int i = 0; i < n2; i++) {
        matrix[i] = (int *)malloc(n2 * sizeof(int));
        for (int j = 0; j < n2; j++) {
            matrix[i][j] = (i == j) ? 0 : INT_MAX;  // 自己到自己为 0，其余先设为不可达
        }
    }
    matrix[0][1] = 2;
    matrix[0][2] = 6;
    matrix[1][2] = 3;
    matrix[1][3] = 1;
    matrix[2][3] = 2;
    printf("\n有向图的边：0->1(2) 0->2(6) 1->2(3) 1->3(1) 2->3(2)");
    Floyd(matrix, n2);

    FreeGraph(graph);
    for (int i = 0; i < n2; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}
