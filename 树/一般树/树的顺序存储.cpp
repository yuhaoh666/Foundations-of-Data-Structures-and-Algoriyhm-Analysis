#include <stdio.h>
#define MaxSize 100
// 双亲表示法表示树

typedef struct {
    int data;
    int parent;
} TreeNode;

typedef struct {
    TreeNode nodes[MaxSize];
    int n; // 结点数量
} Tree;

void InitTree(Tree* tree) {
    tree->n = 0;
}

void AddNode(Tree* tree, int data, int parent) {
    if (tree->n >= MaxSize) {
        printf("树已满，无法添加更多结点。\n");
        return;
    }
    tree->nodes[tree->n].data = data;
    tree->nodes[tree->n].parent = parent;
    tree->n++;
}

// 找p结点的父节点
int FindParent(Tree* tree, int p) {
    return tree->nodes[p].parent;
}

// 找p结点的孩子节点
void FindChildren(Tree* tree, int p) {
    printf("结点%d的孩子节点: ", tree->nodes[p].data);
    for (int i = 0; i < tree->n; i++) { 
        if (tree->nodes[i].parent == p) { 
            printf("%d ", tree->nodes[i].data);
        }
    }
    printf("\n");
}

int main() {
    Tree tree;
    InitTree(&tree);
    
    AddNode(&tree, 1, -1); // 根节点
    AddNode(&tree, 2, 0);  // 结点2，父节点为1
    AddNode(&tree, 3, 0);  // 结点3，父节点为1
    AddNode(&tree, 4, 1);  // 结点4，父节点为2
    AddNode(&tree, 5, 1);  // 结点5，父节点为2
    
    // 测试找父节点
    int parentIndex = FindParent(&tree, 3); // 找结点3的父节点
    if (parentIndex != -1) {
        printf("结点%d的父节点是%d\n", tree.nodes[3].data, tree.nodes[parentIndex].data);
    } else {
        printf("结点%d没有父节点\n", tree.nodes[3].data);
    }

    // 测试找孩子节点
    FindChildren(&tree, 1); // 找结点2的孩子节点

    return 0;
}

