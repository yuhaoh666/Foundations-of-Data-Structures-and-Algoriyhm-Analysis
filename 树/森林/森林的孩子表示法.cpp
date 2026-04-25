#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100
// 孩子表示法表示森林

typedef struct CTNode {
    int childIndex; // 孩子结点在数组中的下标
    struct CTNode* nextChild; // 指向下一个孩子
};

typedef struct {
    int data; // 结点数据
    CTNode* firstChild; // 指向第一个孩子
} CTBox;

typedef struct {
    CTBox nodes[MaxSize]; // 存储结点的数组
    int n; // 结点数量
} CForest;

// 初始化森林
void initForest(CForest* forest) {
    forest->n = 0;
}

// 添加结点
void addNode(CForest* forest, int data, int parent) {
    if (forest->n >= MaxSize) {
        printf("森林已满，无法添加更多结点。\n");
        return;
    }
    forest->nodes[forest->n].data = data;
    forest->nodes[forest->n].firstChild = NULL; // 初始化孩子指针为NULL
    if (parent != -1) { // 如果不是根节点       
        CTNode* newChild = (CTNode*)malloc(sizeof(CTNode));
        newChild->childIndex = forest->n;
        newChild->nextChild = forest->nodes[parent].firstChild; // 将新孩子插入到父节点的孩子链表头部
        forest->nodes[parent].firstChild = newChild; // 更新父节点的第一个孩子指针
    }
    forest->n++;
}

// 找p结点的父节点
int findParent(CForest* forest, CTBox p) {
    for (int i = 0; i < forest->n; i++) { 
        CTNode* child = forest->nodes[i].firstChild;
        while (child != NULL) {
            if (child->childIndex == p.data) {
                return i; // 返回父节点索引
            }
            child = child->nextChild;
        }
    }
    return -1; // 没有找到父节点，返回-1
}
    

// 找p结点的孩子节点
void findChildren(CTBox p) {
    printf("结点%d的孩子节点: ", p.data);
    for (int i = 0; i < MaxSize; i++) {
        CTNode* child = p.firstChild;
        while (child != NULL) {
            printf("%d ", child->childIndex);
            child = child->nextChild;
        }
    }
    printf("\n");
}

int main() {
    CForest forest;
    initForest(&forest);
    
    addNode(&forest, 1, -1); // 根节点
    addNode(&forest, 2, 0);  // 结点2，父节点为1
    addNode(&forest, 3, 0);  // 结点3，父节点为1
    addNode(&forest, 4, -1);  // 第二棵树的根节点
    addNode(&forest, 5, 4);  // 结点5，父节点为4

    // 测试找父节点
    int parentIndex = findParent(&forest, forest.nodes[2]); // 找结点3的父节点
    if (parentIndex != -1) {
        printf("结点%d的父节点是%d\n", forest.nodes[2].data, forest.nodes[parentIndex].data);
    } else {
        printf("结点%d没有父节点\n", forest.nodes[2].data);
    }

    // 测试找孩子节点
    findChildren(forest.nodes[0]); // 找结点1的孩子节点
    findChildren(forest.nodes[3]); // 找结点4的孩子节点

    return 0;
}