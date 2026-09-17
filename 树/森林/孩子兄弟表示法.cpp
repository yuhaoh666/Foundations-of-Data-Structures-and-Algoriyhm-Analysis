// 孩子兄弟表示法表示森林：森林中每棵树的根结点互为“兄弟”，
// 所以把各棵树的根用 nextSibling 串起来就能表示整个森林，结构上与一棵“虚根”的树完全一致
#include <stdio.h>
#include <stdlib.h>

typedef struct CSNode {
    int data;                                 // 结点数据
    struct CSNode *firstChild, *nextSibling;  // 第一个孩子、下一个兄弟（也可以是一棵新树的根）
} CSNode, *CSTree;

// 初始化森林
void InitForest(CSTree *forest) {
    *forest = NULL;
}

// 判空
bool IsEmpty(CSTree forest) {
    return forest == NULL;
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

// 在森林中新增一棵只有根结点的树（把新根接到根结点链表的末尾）
bool AddTree(CSTree *forest, int data) {
    CSNode *root = CreateNode(data);
    if (root == NULL) {
        return false;
    }
    if (*forest == NULL) {
        *forest = root;  // 森林还是空的，新根就是第一棵树的根
        return true;
    }
    CSNode *t = *forest;  // 找到当前最后一棵树的根
    while (t->nextSibling != NULL) {
        t = t->nextSibling;
    }
    t->nextSibling = root;  // 新树挂到森林的末尾
    return true;
}

// 在 parent 结点下追加一个孩子
bool AddChild(CSNode *parent, int data) {
    if (parent == NULL) {
        return false;
    }
    CSNode *node = CreateNode(data);
    if (node == NULL) {
        return false;
    }
    if (parent->firstChild == NULL) {
        parent->firstChild = node;
    } else {
        CSNode *child = parent->firstChild;
        while (child->nextSibling != NULL) {
            child = child->nextSibling;
        }
        child->nextSibling = node;
    }
    return true;
}

// 按数据查找结点（先看根结点链，再逐棵子树向下找）
CSNode *FindNode(CSTree forest, int data) {
    for (CSNode *t = forest; t != NULL; t = t->nextSibling) {
        if (t->data == data) {
            return t;
        }
        CSNode *res = FindNode(t->firstChild, data);
        if (res != NULL) {
            return res;
        }
    }
    return NULL;
}

// 找结点 p 的双亲：沿着“森林 -> 每棵树 -> 每个结点的孩子链表”逐层搜索
CSNode *FindParent(CSTree forest, CSNode *p) {
    if (forest == NULL || p == NULL) {
        return NULL;
    }
    for (CSNode *t = forest; t != NULL; t = t->nextSibling) {  // 森林中的每一棵树
        for (CSNode *child = t->firstChild; child != NULL; child = child->nextSibling) {
            if (child == p) {
                return t;  // 在 t 的孩子链表中命中 p
            }
            CSNode *res = FindParent(child, p);  // 以 child 为根继续向下找
            if (res != NULL) {
                return res;
            }
        }
    }
    return NULL;  // 未找到（p 是某棵树的根，或者不在森林中）
}

// 输出数据为 data 的结点的所有孩子
void FindChildren(CSTree forest, int data) {
    CSNode *p = FindNode(forest, data);
    if (p == NULL) {
        printf("森林中不存在结点 %d\n", data);
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


// 单棵树的先根遍历：访问根，再依次先根遍历每棵子树
void PreOrderTree(CSTree tree) {
    if (tree == NULL) {
        return;
    }
    printf("%d ", tree->data);
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PreOrderTree(child);
    }
}

// 单棵树的后根遍历：先依次后根遍历每棵子树，最后访问根
void PostOrderTree(CSTree tree) {
    if (tree == NULL) {
        return;
    }
    for (CSNode *child = tree->firstChild; child != NULL; child = child->nextSibling) {
        PostOrderTree(child);
    }
    printf("%d ", tree->data);
}

// 森林的先序遍历 = 依次先根遍历每一棵树（等价于对应二叉树的先序遍历）
void PreOrderForest(CSTree forest) {
    for (CSNode *t = forest; t != NULL; t = t->nextSibling) {
        PreOrderTree(t);
    }
}

// 森林的中序遍历 = 依次后根遍历每一棵树（等价于对应二叉树的中序遍历）
void InOrderForest(CSTree forest) {
    for (CSNode *t = forest; t != NULL; t = t->nextSibling) {
        PostOrderTree(t);
    }
}

// 统计森林中树的棵数
int CountTrees(CSTree forest) {
    int count = 0;
    for (CSNode *t = forest; t != NULL; t = t->nextSibling) {
        count++;
    }
    return count;
}

// 统计森林中的结点总数
int CountNodes(CSTree forest) {
    int count = 0;
    for (CSNode *t = forest; t != NULL; t = t->nextSibling) {
        count++;                             // 根结点自己
        count += CountNodes(t->firstChild);  // 再加上它的所有后代
    }
    return count;
}

// 求以 tree 为根的树的深度
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

// 求森林中最深的那棵树的深度
int ForestDepth(CSTree forest) {
    int maxDepth = 0;
    for (CSNode *t = forest; t != NULL; t = t->nextSibling) {
        int d = TreeDepth(t);
        if (d > maxDepth) {
            maxDepth = d;
        }
    }
    return maxDepth;
}

// 用括号表示法递归打印以 tree 为根的一棵树，例如 1(2(4,5),3)
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

// 打印整个森林（各棵树之间用空格分隔）
void PrintForest(CSTree forest) {
    for (CSNode *t = forest; t != NULL; t = t->nextSibling) {
        PrintTree(t);
        printf(" ");
    }
    printf("\n");
}

// 销毁森林：先释放子树，再释放兄弟（即下一棵树）
void DestroyForest(CSTree *forest) {
    if (*forest == NULL) {
        return;
    }
    DestroyForest(&((*forest)->firstChild));
    CSTree sibling = (*forest)->nextSibling;
    free(*forest);
    *forest = NULL;
    if (sibling != NULL) {
        DestroyForest(&sibling);
    }
}

int main() {
    CSTree forest;
    InitForest(&forest);
    printf("初始化后森林是否为空：%s\n", IsEmpty(forest) ? "是" : "否");

    // 森林由两棵树组成：
    //   第 1 棵：1(2(4,5), 3)      第 2 棵：6(7)
    AddTree(&forest, 1);  // 第 1 棵树
    AddTree(&forest, 6);  // 第 2 棵树（根结点 6 与根结点 1 互为兄弟）
    AddChild(FindNode(forest, 1), 2);
    AddChild(FindNode(forest, 1), 3);
    AddChild(FindNode(forest, 2), 4);
    AddChild(FindNode(forest, 2), 5);
    AddChild(FindNode(forest, 6), 7);

    printf("森林（括号表示法）：");
    PrintForest(forest);
    printf("树的棵数：%d，结点总数：%d，森林深度：%d\n",
           CountTrees(forest), CountNodes(forest), ForestDepth(forest));

    printf("森林的先序遍历：");
    PreOrderForest(forest);
    printf("\n森林的中序遍历（依次后根遍历每棵树）：");
    InOrderForest(forest);
    printf("\n");

    // 找双亲：结点 5 的双亲应是 2
    CSNode *p5 = FindNode(forest, 5);
    CSNode *parent = FindParent(forest, p5);
    if (parent == NULL) {
        printf("结点 %d 没有双亲\n", p5->data);
    } else {
        printf("结点 %d 的双亲是 %d\n", p5->data, parent->data);
    }

    // 找孩子
    FindChildren(forest, 2);
    FindChildren(forest, 6);
    FindChildren(forest, 5);

    DestroyForest(&forest);
    printf("销毁后森林是否为空：%s\n", IsEmpty(forest) ? "是" : "否");
    return 0;
}
