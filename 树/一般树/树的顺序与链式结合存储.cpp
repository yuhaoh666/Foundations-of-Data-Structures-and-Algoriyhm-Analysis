#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100
// 孩子表示法表示树

 struct CTNode {
    int childIndex; // 孩子结点在数组中的下标
    struct CTNode* nextChild; // 指向下一个孩子
};

typedef struct {
    int data; // 结点数据
    CTNode* firstChild; // 指向第一个孩子
} CTBox;

typedef struct {
    CTBox nodes[MaxSize]; // 存储结点的数组
    int n, root; // 结点数量和根节点索引
} CTree;

void InitTree(CTree* tree) {
    tree->n = 0;
    tree->root = -1; // 初始化根节点索引为-1，表示树为空
}

void AddNode(CTree* tree, int data, int parent) {
    if (tree->n >= MaxSize) {
        printf("树已满，无法添加更多结点。\n");
        return;
    }
    tree->nodes[tree->n].data = data;
    tree->nodes[tree->n].firstChild = NULL; // 初始化孩子指针为NULL
    if (parent == -1) {
        tree->root = tree->n; // 设置根节点索引
    } else {
        CTNode* newChild = (CTNode*)malloc(sizeof(CTNode));
        newChild->childIndex = tree->n;
        newChild->nextChild = tree->nodes[parent].firstChild; // 将新孩子插入到父节点的孩子链表头部
        tree->nodes[parent].firstChild = newChild; // 更新父节点的第一个孩子指针
    }
    tree->n++;
}

// 找p结点的父节点
int FindParent(CTree* tree, CTBox p) {
    for (int i = 0; i < tree->n; i++) { 
        CTNode* child = tree->nodes[i].firstChild;
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
void FindChildren(CTBox p) {
    printf("结点%d的孩子节点: ", p.data);
    CTNode* child = p.firstChild;
    while (child != NULL) {
        printf("%d ", child->childIndex); // 输出孩子结点的索引
        child = child->nextChild; // 查找下一个兄弟
    }
    printf("\n");
}

int main() {
    CTree tree;
    InitTree(&tree);
    
    AddNode(&tree, 1, -1); // 根节点
    AddNode(&tree, 2, 0);  // 结点2，父节点为1
    AddNode(&tree, 3, 0);  // 结点3，父节点为1
    AddNode(&tree, 4, 1);  // 结点4，父节点为2
    AddNode(&tree, 5, 1);  // 结点5，父节点为2
    
    // 测试找父节点
    int parentIndex = FindParent(&tree, tree.nodes[3]); // 找结点3的父节点
    if (parentIndex != -1) {
        printf("结点%d的父节点是%d\n", tree.nodes[3].data, tree.nodes[parentIndex].data);
    } else {
        printf("结点%d没有父节点\n", tree.nodes[3].data);
    }

    // 测试找孩子节点
    FindChildren(tree.nodes[1]); // 找结点2的孩子节点
    
    return 0;
}




