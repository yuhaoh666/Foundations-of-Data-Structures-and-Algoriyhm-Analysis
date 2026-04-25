#include <stdio.h>
#include <stdlib.h>

typedef struct ThreadNode {
    int data;
    struct ThreadNode* left;
    struct ThreadNode* right;
    bool ltag, rtag; // 标记左右指针是否为线索
} ThreadNode, *ThreadTree;

void initTree(ThreadTree* root) {
    *root = NULL;
}

bool isEmpty(ThreadTree root) {
    return root == NULL;
}

// 中序遍历构造线索二叉树
void inOrderThreading(ThreadTree root, ThreadNode** pre) {
    if (root != NULL) {
        inOrderThreading(root->left, pre);
        if (root->left == NULL) {
            root->ltag = true;
            root->left = *pre; // 前驱线索
        }
        if (*pre != NULL && (*pre)->right == NULL) {
            (*pre)->rtag = true;
            (*pre)->right = root; // 后继线索
        }
        *pre = root; // 更新前驱节点
        inOrderThreading(root->right, pre);
    }
}

// 寻找p结点的后继
ThreadNode* inOrderSuccessor(ThreadNode* p) {
    if (p->rtag) {
        return p->right; // 直接返回后继线索    
    } else {
        ThreadNode* temp = p->right;
        while (temp != NULL && !temp->ltag) {
            temp = temp->left; // 沿右子树一直向左走
        }
        return temp; // 返回后继节点
    }
}

// 寻找p结点的前驱
ThreadNode* inOrderPredecessor(ThreadNode* p) {
    if (p->ltag) {
        return p->left; // 直接返回前驱线索
    } else {
        ThreadNode* temp = p->left;
        while (temp != NULL && !temp->rtag) {
            temp = temp->right; // 沿左子树一直向右走
        }
        return temp; // 返回前驱节点
    }
}

int main() {
    ThreadTree root;
    initTree(&root);
    return 0;
}

