#include <stdio.h>
#include <stdlib.h>
// 孩子兄弟表示法表示森林

typedef struct CSNode {
    int data; // 结点数据
    struct CSNode* firstChild, *nextSibling; // 指向第一个孩子和右兄弟
} CSNode, *CSTree;

// 初始化森林
void initForest(CSTree* forest) {
    *forest = NULL; // 初始化森林为空
}

// 添加结点
void addNode(CSTree* forest, int data, CSNode* parent) {
    CSNode* newNode = (CSNode*)malloc(sizeof(CSNode));
    newNode->data = data;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    if (parent == NULL) { // 如果没有父节点，说明是森林中的一棵新树
        newNode->nextSibling = *forest; // 将新树连接到森林的前面
        *forest = newNode; // 更新森林的头指针
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
CSNode* findParent(CSTree forest, CSNode* p) {
    if (forest == NULL) return NULL;
    if (forest->firstChild != NULL) {
        CSNode* child = forest->firstChild;
        while (child != NULL) {
            if (child == p) {
                return forest; // 返回父节点
            }
            CSNode* result = findParent(child, p);
            if (result != NULL) return result; // 在子树中找到父节点
            child = child->nextSibling; // 查找下一个兄弟
        }
    }
    return NULL; // 没有找到父节点
}

// 找到p结点的孩子节点
void findChildren(CSTree forest, CSNode* p) {
    if (forest == NULL) return;
    if (forest == p) {
        printf("结点%d的孩子节点: ", p->data);
        CSNode* child = forest->firstChild;
        while (child != NULL) {
            printf("%d ", child->data);
            child = child->nextSibling; // 查找下一个兄弟
        }
        printf("\n");
    } else {    
        CSNode* child = forest->firstChild;
        while (child != NULL) {
            findChildren(child, p); // 在子树中查找p结点
            child = child->nextSibling; // 查找下一个兄弟
        }
    }
}

int main() {
    CSTree forest;
    initForest(&forest);
    
    // 构造森林
    addNode(&forest, 1, NULL); // 添加树1的根节点
    CSNode* tree1 = forest; // 保存树1的根节点指针
    addNode(&forest, 2, tree1); // 添加树1的孩子节点
    addNode(&forest, 3, tree1); // 添加树1的孩子节点
    CSNode* child1 = tree1->firstChild; // 保存树1的第一个孩子节点指针

    addNode(&forest, 4, NULL); // 添加树2的根节点
    CSNode* tree2 = forest; // 保存树2的根节点指针
    addNode(&forest, 5, tree2); // 添加树2的孩子节点
    
    // 测试findParent和findChildren函数
    CSNode* parent = findParent(forest, child1);
    if (parent != NULL) {
        printf("结点%d的父节点是%d\n", child1->data, parent->data);
    } else {
        printf("没有找到结点%d的父节点\n", child1->data);
    }
    
    findChildren(forest, tree1); // 查找tree1的孩子节点
    findChildren(forest, tree2); // 查找tree2的孩子节点
    
    return 0;
}