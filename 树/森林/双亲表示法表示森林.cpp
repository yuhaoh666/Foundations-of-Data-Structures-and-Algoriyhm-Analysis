#include <stdio.h>
#define MAX_SIZE 100
// 双亲表示法表示森林

typedef struct {
    int data; // 结点数据
    int parent; // 双亲结点的下标，-1表示没有父节点（即根节点）
} TreeNode;

typedef struct {
    TreeNode nodes[MAX_SIZE]; // 存储结点的数组
    int n; // 结点数量
} Forest;

// 初始化森林
void initForest(Forest* forest) {
    forest->n = 0;
}

// 添加结点
void addNode(Forest* forest, int data, int parent) {
    if (forest->n >= MAX_SIZE) {
        printf("森林已满，无法添加更多结点。\n");
        return;
    }
    forest->nodes[forest->n].data = data;
    forest->nodes[forest->n].parent = parent;
    forest->n++; 
}

// 找p结点的父节点
int findParent(Forest* forest, int p) {
    return forest->nodes[p].parent;
}

// 找p结点的孩子节点
void findChildren(Forest* forest, int p) {
    printf("结点%d的孩子节点: ", forest->nodes[p].data);
    for (int i = 0; i < forest->n; i++) {
        if (forest->nodes[i].parent == p) { 
            printf("%d ", forest->nodes[i].data);
        }
    }
    printf("\n");
}

int main() {
    Forest forest;
    initForest(&forest);
    
    addNode(&forest, 1, -1); // 根节点
    addNode(&forest, 2, 0);  // 结点2，父节点为1
    addNode(&forest, 3, 0);  // 结点3，父节点为1
    addNode(&forest, 4, 1);  // 结点4，父节点为2
    addNode(&forest, 5, 1);  // 结点5，父节点为2
    
    // 测试找父节点
    int parentIndex = findParent(&forest, 3); // 找结点3的父节点
    if (parentIndex != -1) {
        printf("结点%d的父节点是%d\n", forest.nodes[3].data, forest.nodes[parentIndex].data);
    } else {
        printf("结点%d没有父节点\n", forest.nodes[3].data);
    }
    
    // 测试找孩子节点
    findChildren(&forest, 1); // 找结点1的孩子节点
    
    return 0;
}