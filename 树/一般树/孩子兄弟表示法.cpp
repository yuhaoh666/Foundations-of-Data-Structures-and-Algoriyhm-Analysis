// 树的孩子兄弟表示法（又称二叉树表示法）：每个结点有 firstChild（第一个孩子）
// 和 nextSibling（下一个兄弟）两个指针，任何一棵树都能用二叉链表存下来；
// 它也是“树 <-> 二叉树”相互转换的基础
#include <stdio.h>
#include <stdlib.h>

typedef struct CSNode {
    int data;                              // 结点数据
    struct CSNode *firstChild, *nextSibling;  // 第一个孩子、下一个兄弟
} CSNode, *CSTree;

// 初始化：空树用 NULL 表示
void InitTree(CSTree *tree) {
    *tree = NULL;
}

// 判空
bool IsEmpty(CSTree tree) {
    return tree == NULL;
}

// 创建结点
CSNode *CreateNode(int data) {
    CSNode *node = (CSNode *)malloc(sizeof(CSNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    node->data = data;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

// 建立根结点：只有空树才能建根
bool SetRoot(CSTree *tree, int data) {
    if (*tree != NULL) {
        return false;
    }
    *tree = CreateNode(data);
    return *tree != NULL;
}

// 在 parent 结点的孩子链表末尾追加一个孩子（保持孩子顺序与添加顺序一致）
bool AddChild(CSNode *parent, int data) {
    if (parent == NULL) {
        return false;
    }
    CSNode *node = CreateNode(data);
    if (node == NULL) {
        return false;
    }
    if (parent->firstChild == NULL) {
        parent->firstChild = node;  // 还没有孩子，直接作为第一个孩子
    } else {
        CSNode *child = parent->firstChild;  // 否则找到最后一个孩子
        while (child->nextSibling != NULL) {
            child = child->nextSibling;
        }
        child->nextSibling = node;
    }
    return true;
}

// 按数据查找结点，未找到返回 NULL
CSNode *FindNode(CSTree tree, int data) {
    if (tree == NULL) {
        return NULL;
    }
    if (tree->data == data) {
        return tree;
    }
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        CSNode *res = FindNode(child, data);
        if (res != NULL) {
            return res;
        }
    }
    return NULL;
}

// 找结点 p 的双亲：只能遍历整棵树
CSNode *FindParent(CSTree tree, CSNode *p) {
    if (tree == NULL) {
        return NULL;
    }
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        if (child == p) {
            return tree;  // 在 tree 的孩子链表中命中了 p
        }
        CSNode *res = FindParent(child, p);  // 否则到子树中继续找
        if (res != NULL) {
            return res;
        }
    }
    return NULL;  // 没找到（p 不是 tree 的后代）
}

// 输出结点 p 的所有孩子
void FindChildren(CSNode *p) {
    if (p == NULL) {
        printf("结点不存在\n");
        return;
    }
    printf("结点 %d 的孩子：", p->data);
    if (p->firstChild == NULL) {
        printf("无");
    }
    for (CSNode *child = p->firstChild; child != NULL; child = child->nextSibling) {
        printf("%d ", child->data);
    }
    printf("\n");
}

// 先根遍历：先访问根，再依次先根遍历每棵子树
void PreOrder(CSTree tree) {
    if (tree == NULL) {
        return;
    }
    printf("%d ", tree->data);
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PreOrder(child);
    }
}

// 后根遍历：先依次后根遍历每棵子树，最后访问根
void PostOrder(CSTree tree) {
    if (tree == NULL) {
        return;
    }
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PostOrder(child);
    }
    printf("%d ", tree->data);
}

// 求树的深度：空树深度为 0，否则是各子树深度的最大值加 1
int TreeDepth(CSTree tree) {
    if (tree == NULL) {
        return 0;
    }
    int maxChildDepth = 0;
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        int d = TreeDepth(child);
        if (d > maxChildDepth) {
            maxChildDepth = d;
        }
    }
    return maxChildDepth + 1;
}

// 统计结点个数
int CountNodes(CSTree tree) {
    if (tree == NULL) {
        return 0;
    }
    int count = 1;
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        count += CountNodes(child);
    }
    return count;
}

// 用括号表示法打印树，便于直观检查结构
void PrintTree(CSTree tree) {
    if (tree == NULL) {
        return;
    }
    printf("%d", tree->data);
    if (tree->firstChild == NULL) {
        return;
    }
    printf("(");
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PrintTree(child);
        if (child->nextSibling != NULL) {
            printf(",");
        }
    }
    printf(")");
}

// 销毁树：先释放子树，再释放兄弟链
void DestroyTree(CSTree *tree) {
    if (*tree == NULL) {
        return;
    }
    DestroyTree(&((*tree)->firstChild));
    CSTree sibling = (*tree)->nextSibling;
    free(*tree);
    *tree = NULL;
    if (sibling != NULL) {
        DestroyTree(&sibling);
    }
}

int main() {
    CSTree tree;
    InitTree(&tree);
    printf("初始化后是否为空树：%s\n", IsEmpty(tree) ? "是" : "否");

    // 构造如下树：
    //        1
    //      /   \
    //     2     3
    //    / \     \
    //   4   5     6
    SetRoot(&tree, 1);
    CSNode *n1 = FindNode(tree, 1);
    AddChild(n1, 2);
    AddChild(n1, 3);
    AddChild(FindNode(tree, 2), 4);
    AddChild(FindNode(tree, 2), 5);
    AddChild(FindNode(tree, 3), 6);
    printf("建根重复调用：%s\n", SetRoot(&tree, 99) ? "成功" : "失败（树已存在根结点）");

    printf("树（括号表示法）：");
    PrintTree(tree);
    printf("\n结点总数：%d，树的深度：%d\n", CountNodes(tree), TreeDepth(tree));

    printf("先根遍历：");
    PreOrder(tree);
    printf("\n后根遍历：");
    PostOrder(tree);
    printf("\n");

    // 测试找双亲
    CSNode *p = FindNode(tree, 5);
    CSNode *parent = FindParent(tree, p);
    if (parent == NULL) {
        printf("结点 %d 没有双亲（它是根结点）\n", p->data);
    } else {
        printf("结点 %d 的双亲是 %d\n", p->data, parent->data);
    }

    // 测试找孩子
    FindChildren(FindNode(tree, 2));
    FindChildren(FindNode(tree, 5));

    DestroyTree(&tree);
    printf("销毁后是否为空树：%s\n", IsEmpty(tree) ? "是" : "否");
    return 0;
}
