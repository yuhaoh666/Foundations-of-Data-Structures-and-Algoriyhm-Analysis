// 树/森林与二叉树的相互转换（孩子兄弟表示法 <-> 二叉链表），另附符号转换规则的说明
// 转换规则：树的“第一个孩子”对应二叉树的“左孩子”，“下一个兄弟”对应二叉树的“右孩子”
// 结论：同一棵树的先根遍历序列 等于 对应二叉树的先序遍历序列；
//       同一棵树的后根遍历序列 等于 对应二叉树的中序遍历序列
#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode {   // 二叉链表结点
    char data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

typedef struct CSNode {    // 孩子兄弟表示法的结点
    char data;             // 与二叉树保持相同的数据类型，转换时才不会丢失信息
    struct CSNode *firstChild, *nextSibling;
} CSNode, *CSTree;

// 初始化
void InitBiTree(BiTree *T) {
    *T = NULL;
}

void InitCSTree(CSTree *T) {
    *T = NULL;
}

// 创建二叉树结点
BiTNode *CreateBiTNode(char data) {
    BiTNode *node = (BiTNode *)malloc(sizeof(BiTNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->lchild = node->rchild = NULL;
    return node;
}

// 创建孩子兄弟结点
CSNode *CreateCSNode(char data) {
    CSNode *node = (CSNode *)malloc(sizeof(CSNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->firstChild = node->nextSibling = NULL;
    return node;
}

// 树（森林）转二叉树：递归地把第一个孩子接到左子树、把兄弟接到右子树
void CSTreeToBiTree(CSTree CST, BiTree *T) {
    if (CST == NULL) {
        *T = NULL;  // 空树对应空二叉树
        return;
    }
    *T = CreateBiTNode(CST->data);  // 复制数据
    if (CST->firstChild != NULL) {  // 第一个孩子 -> 左孩子
        CSTreeToBiTree(CST->firstChild, &((*T)->lchild));
    }
    if (CST->nextSibling != NULL) {  // 右兄弟 -> 右孩子
        CSTreeToBiTree(CST->nextSibling, &((*T)->rchild));
    }
}

// 二叉树转树（森林）：左孩子变回第一个孩子，右孩子变回右兄弟
void BiTreeToCSTree(BiTree T, CSTree *CST) {
    if (T == NULL) {
        *CST = NULL;
        return;
    }
    *CST = CreateCSNode(T->data);
    if (T->lchild != NULL) {
        BiTreeToCSTree(T->lchild, &((*CST)->firstChild));
    }
    if (T->rchild != NULL) {
        BiTreeToCSTree(T->rchild, &((*CST)->nextSibling));
    }
}

// 用括号表示法打印树（森林），例如 A(B(D,E),C(F))
void PrintCSTree(CSTree CST) {
    if (CST == NULL) {
        return;
    }
    printf("%c", CST->data);
    if (CST->firstChild == NULL) {  // 叶子结点直接输出
        return;
    }
    printf("(");
    for (CSTree child = CST->firstChild; child != NULL; child = child->nextSibling) {
        PrintCSTree(child);
        if (child->nextSibling != NULL) {
            printf(",");
        }
    }
    printf(")");
}

// 先序遍历二叉树（等价于树的先根遍历）
void BiTreePreOrder(BiTree T) {
    if (T != NULL) {
        printf("%c ", T->data);
        BiTreePreOrder(T->lchild);
        BiTreePreOrder(T->rchild);
    }
}

// 中序遍历二叉树（等价于树的后根遍历）
void BiTreeInOrder(BiTree T) {
    if (T != NULL) {
        BiTreeInOrder(T->lchild);
        printf("%c ", T->data);
        BiTreeInOrder(T->rchild);
    }
}

// 树的先根遍历：先访问根，再依次先根遍历每棵子树
void CSTreePreOrder(CSTree CST) {
    if (CST == NULL) {
        return;
    }
    printf("%c ", CST->data);
    for (CSTree child = CST->firstChild; child != NULL; child = child->nextSibling) {
        CSTreePreOrder(child);
    }
}

// 树的后根遍历：先依次后根遍历每棵子树，最后访问根
void CSTreePostOrder(CSTree CST) {
    if (CST == NULL) {
        return;
    }
    for (CSTree child = CST->firstChild; child != NULL; child = child->nextSibling) {
        CSTreePostOrder(child);
    }
    printf("%c ", CST->data);
}

// 销毁孩子兄弟链表：先释放所有孩子，再释放兄弟链
void DestroyCSTree(CSTree *CST) {
    if (*CST == NULL) {
        return;
    }
    DestroyCSTree(&((*CST)->firstChild));  // 递归释放子树
    CSTree sibling = (*CST)->nextSibling;  // 先记住兄弟，再释放自己
    free(*CST);
    *CST = NULL;
    if (sibling != NULL) {
        DestroyCSTree(&sibling);  // 继续释放兄弟链
    }
}

// 销毁二叉树
void DestroyBiTree(BiTree *T) {
    if (*T == NULL) {
        return;
    }
    DestroyBiTree(&((*T)->lchild));
    DestroyBiTree(&((*T)->rchild));
    free(*T);
    *T = NULL;
}

int main() {
    CSTree tree;
    BiTree bt;
    InitCSTree(&tree);
    InitBiTree(&bt);

    // 手工构造如下树：
    //        A
    //      /   \
    //     B     C
    //    / \    |
    //   D   E   F
    tree = CreateCSNode('A');
    CSNode *b = CreateCSNode('B');
    CSNode *c = CreateCSNode('C');
    tree->firstChild = b;          // A 的第一个孩子是 B
    b->nextSibling = c;            // B 的下一个兄弟是 C
    b->firstChild = CreateCSNode('D');
    b->firstChild->nextSibling = CreateCSNode('E');
    c->firstChild = CreateCSNode('F');

    printf("原树（括号表示法）：");
    PrintCSTree(tree);
    printf("\n树 的先根遍历：");
    CSTreePreOrder(tree);
    printf("\n树 的后根遍历：");
    CSTreePostOrder(tree);
    printf("\n");

    // 树 -> 二叉树
    CSTreeToBiTree(tree, &bt);
    printf("\n转换为二叉树后的先序遍历：");
    BiTreePreOrder(bt);
    printf("\n转换为二叉树后的中序遍历：");
    BiTreeInOrder(bt);
    printf("\n（可见：树的先根遍历 == 二叉树先序，树的后根遍历 == 二叉树中序）\n");

    // 二叉树 -> 树（验证还原结果）
    CSTree back;
    InitCSTree(&back);
    BiTreeToCSTree(bt, &back);
    printf("\n二叉树再还原为树：");
    PrintCSTree(back);
    printf("\n");

    DestroyCSTree(&back);
    DestroyBiTree(&bt);
    DestroyCSTree(&tree);
    return 0;
}
