#include <stdio.h>
#include <stdlib.h>
// 孩子兄弟表示法表示树

typedef struct CSNode {
    int data; // 结点数据
    struct CSNode* firstChild, *nextSibling; // 指向第一个孩子和右兄弟
} CSNode, *CSTree;

void initTree(CSTree* tree) {
    *tree = NULL; // 初始化树为空
}

void addNode(CSTree* tree, int data, CSNode* parent) {
    CSNode* newNode = (CSNode*)malloc(sizeof(CSNode));
    newNode->data = data;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    if (parent == NULL) { // 如果没有父节点，说明是树中的根节点
        newNode->nextSibling = *tree; // 将新树连接到树的前面
        *tree = newNode; // 更新树的头指针
    } else { // 否则，将新结点添加到父节点的孩子链表中
        if (parent->firstChild == NULL) {
            parent->firstChild = newNode; // 作为第一个孩子
        } else {
            CSNode* child = parent->firstChild;
            while (child->nextSibling != NULL) {
                child = child->nextSibling; // 找到最后一个孩子
            }
            child->nextSibling = newNode; // 将新结点添加为最后一个孩子的右兄弟
        }
    }
}

// 找到p结点的父节点
CSNode* findParent(CSTree tree, CSNode* p) {
    if (tree == NULL) return NULL;
    if (tree->firstChild != NULL) {
        CSNode* child = tree->firstChild;
        while (child != NULL) {
            if (child == p) {
                return tree; // 返回父节点
            }
            CSNode* result = findParent(child, p);
            if (result != NULL) return result; // 在子树中找到父节点
            child = child->nextSibling; // 查找下一个兄弟
        }
    }
    return NULL; // 没有找到父节点
}

// 找到p结点的孩子节点
void findChildren(CSTree tree, CSNode* p) {
    if (tree == NULL) return;
    if (tree == p) {
        printf("结点%d的孩子节点: ", p->data);
        CSNode* child = tree->firstChild;
        while (child != NULL) {
            printf("%d ", child->data);
            child = child->nextSibling; // 查找下一个兄弟
        }
        printf("\n");
    } else {    
        CSNode* child = tree->firstChild;
        while (child != NULL) {
            findChildren(child, p); // 在子树中查找p结点
            child = child->nextSibling; // 查找下一个兄弟
        }
    }
}

int main() {
    CSTree tree;
    initTree(&tree);
    
    // 构造树
    addNode(&tree, 1, NULL); // 根节点
    addNode(&tree, 2, tree); // 结点2，父节点为1
    addNode(&tree, 3, tree); // 结点3，父节点为1
    addNode(&tree, 4, tree->firstChild); // 结点4，父节点为2
    addNode(&tree, 5, tree->firstChild); // 结点5，父节点为2
    addNode(&tree, 6, tree->firstChild->nextSibling); // 结点6，父节点为3

    // 测试找父节点
    CSNode* parent = findParent(tree, tree->firstChild); // 找结点2的父节点
    if (parent != NULL) {
        printf("结点%d的父节点是%d\n", tree->firstChild->data, parent->data);
    } else {
        printf("结点%d没有父节点\n", tree->firstChild->data);
    }

    // 测试找孩子节点
    findChildren(tree, tree->firstChild); // 找结点2的孩子节点
    
    return 0;
}