#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode
{
    char data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

typedef struct CSNode
{
    int data;
    struct CSNode *firstChild, *nextSibling;
} CSNode, *CSTree;

// 初始化二叉树
void initBiTree(BiTree *T)
{
    *T = NULL; // 初始化二叉树为空
}

// 初始化一般树
void initCSTree(CSTree *T)
{
    *T = NULL; // 初始化一般树为空
}

// 二叉树转一般树
void biTreeToCSTree(BiTree T, CSTree *CST)
{
    if (T == NULL)    {
        *CST = NULL; // 如果二叉树为空，返回空的一般树
        return;
    }   
    *CST = (CSNode *)malloc(sizeof(CSNode)); // 创建一般树结点
    (*CST)->data = T->data; // 复制数据
    (*CST)->firstChild = NULL; // 初始化孩子指针
    (*CST)->nextSibling = NULL; // 初始化兄弟指针
    if (T->lchild != NULL) { // 如果左子树不为空，递归转换为第一个孩子
        biTreeToCSTree(T->lchild, &((*CST)->firstChild));
    }
    if (T->rchild != NULL) { // 如果右子树不为空，递归转换为兄弟
        biTreeToCSTree(T->rchild, &((*CST)->nextSibling));
    }
}

// 一般树转二叉树
void csTreeToBiTree(CSTree CST, BiTree *T)
{
    if (CST == NULL) {
        *T = NULL; // 如果一般树为空，返回空的二叉树
        return;
    }
    *T = (BiTNode *)malloc(sizeof(BiTNode)); // 创建二叉树结点
    (*T)->data = CST->data; // 复制数据
    (*T)->lchild = NULL; // 初始化左子树指针
    (*T)->rchild = NULL; // 初始化右子树指针
    if (CST->firstChild != NULL) { // 如果有孩子，递归转换为左子树
        csTreeToBiTree(CST->firstChild, &((*T)->lchild));
    }
    if (CST->nextSibling != NULL) { // 如果有兄弟，递归转换为右子树
        csTreeToBiTree(CST->nextSibling, &((*T)->rchild));
    }
}

int main()
{
    BiTree T;
    CSTree CST;
    initBiTree(&T);
    initCSTree(&CST);
    // 这里可以添加代码来构建二叉树T和一般树CST，并测试转换函数
    return 0;
}