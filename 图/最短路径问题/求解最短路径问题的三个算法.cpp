// 求解最短路径问题的三个算法（结构化版本）
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// ====================== 结构体定义 ======================
// 1. 邻接表节点（用于BFS/Dijkstra，适合稀疏图）
typedef struct AdjNode {
    int vertex;       // 邻接顶点编号
    int weight;       // 边的权重（无权图时为1，有权图时为实际权重）
    struct AdjNode* next; // 下一个邻接节点
} AdjNode;

// 2. 邻接表头（每个顶点对应一个邻接表）
typedef struct AdjList {
    AdjNode* head;    // 邻接表头节点
} AdjList;

// 3. 图结构体（邻接表实现）
typedef struct Graph {
    int numVertices;  // 顶点总数
    AdjList* array;   // 邻接表数组
} Graph;

// 4. 用于Dijkstra/Floyd的辅助结构体（存储最短路径信息）
typedef struct PathInfo {
    int dist;         // 起点到该顶点的最短距离
    int prev;         // 最短路径上的前驱顶点（用于回溯路径）
    int visited;      // Dijkstra中标记是否已处理
} PathInfo;

// ====================== 辅助函数 ======================
// 1. 创建邻接表节点
AdjNode* createAdjNode(int v, int weight) {
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// 2. 创建图（初始化邻接表）
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->array = (AdjList*)malloc(vertices * sizeof(AdjList));

    // 初始化每个顶点的邻接表为空
    for (int i = 0; i < vertices; i++) {
        graph->array[i].head = NULL;
    }
    return graph;
}

// 3. 向图中添加边（无向图/有向图可通过此函数控制）
void addEdge(Graph* graph, int src, int dest, int weight, int isDirected) {
    // 添加 src -> dest 的边
    AdjNode* newNode = createAdjNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // 无向图需添加 dest -> src 的边
    if (!isDirected) {
        newNode = createAdjNode(src, weight);
        newNode->next = graph->array[dest].head;
        graph->array[dest].head = newNode;
    }
}

// 4. 初始化路径信息结构体（Dijkstra专用）
void initPathInfo(PathInfo* info, int n, int start) {
    for (int i = 0; i < n; i++) {
        info[i].dist = INT_MAX;  // 初始距离为无穷大
        info[i].prev = -1;       // 无前驱
        info[i].visited = 0;     // 未访问
    }
    info[start].dist = 0;  // 起点到自身距离为0
}

// 5. 打印最短路径（通过前驱节点回溯）
void printPath(PathInfo* info, int v) {
    if (info[v].prev == -1) {
        printf("%d", v);  // 递归到起点
        return;
    }
    printPath(info, info[v].prev);
    printf(" -> %d", v);
}

// 6. 释放图的内存（避免内存泄漏）
void freeGraph(Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->numVertices; i++) {
        AdjNode* curr = graph->array[i].head;
        while (curr) {
            AdjNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(graph->array);
    free(graph);
}

// ====================== 核心算法 ======================
// 1. BFS算法（求解无权图最短路径）
void BFS(Graph* graph, int start) {
    int n = graph->numVertices;
    int* dist = (int*)malloc(n * sizeof(int));  // 存储最短距离
    int* prev = (int*)malloc(n * sizeof(int));  // 存储前驱节点
    int* visited = (int*)malloc(n * sizeof(int)); // 访问标记

    // 初始化
    for (int i = 0; i < n; i++) {
        dist[i] = -1;    // -1表示不可达
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;
    visited[start] = 1;

    // 队列实现BFS
    int* queue = (int*)malloc(n * sizeof(int));
    int front = 0, rear = 0;
    queue[rear++] = start;

    while (front < rear) {
        int u = queue[front++];
        AdjNode* curr = graph->array[u].head;
        while (curr) {
            int v = curr->vertex;
            if (!visited[v]) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                prev[v] = u;
                queue[rear++] = v;
            }
            curr = curr->next;
        }
    }

    // 打印结果
    printf("=== BFS 最短路径（无权图）===\n");
    for (int i = 0; i < n; i++) {
        printf("起点 %d 到 %d: ", start, i);
        if (dist[i] == -1) {
            printf("不可达\n");
        } else {
            printf("距离=%d, 路径: ", dist[i]);
            // 复用printPath（需构造临时PathInfo）
            PathInfo tempInfo;
            memcpy(&tempInfo, &(PathInfo){.prev = prev[i]}, sizeof(PathInfo));
            printPath(&(PathInfo){.prev = prev[i]}, i); // 简化打印
            printf("\n");
        }
    }

    // 释放内存
    free(dist);
    free(prev);
    free(visited);
    free(queue);
}

// 2. Dijkstra算法（求解带权非负图最短路径）
void Dijkstra(Graph* graph, int start) {
    int n = graph->numVertices;
    PathInfo* info = (PathInfo*)malloc(n * sizeof(PathInfo));
    initPathInfo(info, n, start);

    // 遍历所有顶点
    for (int i = 0; i < n - 1; i++) {
        // 找到未访问的距离最小的顶点u
        int u = -1, minDist = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (!info[j].visited && info[j].dist < minDist) {
                minDist = info[j].dist;
                u = j;
            }
        }
        if (u == -1) break; // 剩余顶点不可达
        info[u].visited = 1;

        // 松弛操作：更新u的邻接顶点距离
        AdjNode* curr = graph->array[u].head;
        while (curr) {
            int v = curr->vertex;
            int weight = curr->weight;
            if (!info[v].visited && info[u].dist != INT_MAX 
                && info[u].dist + weight < info[v].dist) {
                info[v].dist = info[u].dist + weight;
                info[v].prev = u;
            }
            curr = curr->next;
        }
    }

    // 打印结果
    printf("\n=== Dijkstra 最短路径（带权非负图）===\n");
    for (int i = 0; i < n; i++) {
        printf("起点 %d 到 %d: ", start, i);
        if (info[i].dist == INT_MAX) {
            printf("不可达\n");
        } else {
            printf("距离=%d, 路径: ", info[i].dist);
            printPath(info, i);
            printf("\n");
        }
    }

    free(info);
}

// 3. Floyd算法（求解任意两点最短路径，支持负权边但无负环）
void Floyd(int** graph, int n) {
    // 初始化距离矩阵和前驱矩阵
    int** dist = (int**)malloc(n * sizeof(int*));
    int** prev = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        dist[i] = (int*)malloc(n * sizeof(int));
        prev[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            dist[i][j] = graph[i][j];
            if (graph[i][j] != INT_MAX && i != j) {
                prev[i][j] = i;
            } else {
                prev[i][j] = -1;
            }
        }
    }

    // 动态规划更新最短路径
    for (int k = 0; k < n; k++) {  // 中间顶点
        for (int i = 0; i < n; i++) {  // 起点
            for (int j = 0; j < n; j++) {  // 终点
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX 
                    && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    prev[i][j] = prev[k][j];
                }
            }
        }
    }

    // 打印结果
    printf("\n=== Floyd 任意两点最短路径 ===\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("起点 %d 到 %d: ", i, j);
            if (dist[i][j] == INT_MAX) {
                printf("不可达\n");
            } else {
                printf("距离=%d, 路径: ", dist[i][j]);
                // 构造临时PathInfo用于打印
                PathInfo* tempInfo = (PathInfo*)malloc(n * sizeof(PathInfo));
                for (int x = 0; x < n; x++) {
                    tempInfo[x].prev = prev[i][x];
                }
                printPath(tempInfo, j);
                free(tempInfo);
                printf("\n");
            }
        }
        printf("-----------------\n");
    }

    // 释放内存
    for (int i = 0; i < n; i++) {
        free(dist[i]);
        free(prev[i]);
    }
    free(dist);
    free(prev);
}

// ====================== 主函数测试 ======================
int main() {
    // 测试1：BFS和Dijkstra（邻接表）
    int vertices = 5;
    Graph* graph = createGraph(vertices);
    // 添加有向边（src, dest, weight, isDirected=1）
    addEdge(graph, 0, 1, 2, 1);
    addEdge(graph, 0, 2, 1, 1);
    addEdge(graph, 1, 3, 1, 1);
    addEdge(graph, 2, 1, 3, 1);
    addEdge(graph, 2, 3, 4, 1);
    addEdge(graph, 3, 4, 2, 1);

    BFS(graph, 0);          // BFS测试（无权图时weight无意义，按1处理）
    Dijkstra(graph, 0);     // Dijkstra测试

    // 测试2：Floyd（邻接矩阵）
    int n = 4;
    int** floydGraph = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        floydGraph[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            floydGraph[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
    floydGraph[0][1] = 2;
    floydGraph[0][2] = 6;
    floydGraph[1][2] = 3;
    floydGraph[1][3] = 1;
    floydGraph[2][3] = 2;

    Floyd(floydGraph, n);   // Floyd测试

    // 释放内存
    freeGraph(graph);
    for (int i = 0; i < n; i++) {
        free(floydGraph[i]);
    }
    free(floydGraph);

    return 0;
}