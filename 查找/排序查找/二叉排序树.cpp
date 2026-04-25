#include <stdio.h>
#include <stdlib.h>

typedef struct BSTNode {
    int key;
    struct BSTNode *lchild, *rchild;
} BSTNode,*BSTree;

// 在二叉排序树中查找值为 key 的结点
BSTNode *BST_Search(BSTree T,int key){
    while(T!=NULL&&key!=T->key){ // 若树空或等于根结点值，则结束循环
        if(key<T->key) T=T->lchild;
        else T=T->rchild;
    }
    return T;
}

// 在二叉排序树中插入值为 key 的结点
bool BST_Insert(BSTree &T, int key){
    if(!T){
        T = (BSTree)malloc(sizeof(BSTNode));
        T->key = key;
        T->lchild = T->rchild = NULL;
        return true;
    }
    else if(key == T->key) return false;
    else if(key < T->key) return BST_Insert(T->lchild, key);
    else return BST_Insert(T->rchild, key); 
}

// 按照 str[] 中的关键字序列建立二叉排序树
void Creat_BST(BSTree &T, int str[], int n){
    T = NULL;
    int i = 0;
    while(i < n){
        BST_Insert(T, str[i]);
        i++;
    }
}

// 辅助函数：删除以p为根的子树中值最小的节点，并将最小值存入minKey
void DeleteMin(BSTree &p, int &minKey) {
    if (p->lchild == NULL) {
        // 找到最小值节点
        minKey = p->key;
        BSTNode *temp = p;
        p = p->rchild; // 用右孩子替代当前节点
        free(temp);
    } else {
        DeleteMin(p->lchild, minKey);
    }
}

// 在二叉排序树中删除值为 key 的结点
bool BST_Delete(BSTree &T, int key, int &out) {
    out = -1; // 默认删除失败
    if (T == NULL) {
        return false; // 树空，删除失败
    }
    
    if (key < T->key) {
        // 递归删除左子树中的节点
        return BST_Delete(T->lchild, key, out);
    } else if (key > T->key) {
        // 递归删除右子树中的节点
        return BST_Delete(T->rchild, key, out);
    } else {
        // 找到待删除节点，记录被删除的key
        out = T->key;
        
        // 情况1：叶子节点（无左右孩子）
        if (T->lchild == NULL && T->rchild == NULL) {
            free(T);
            T = NULL;
        }
        // 情况2：只有右孩子
        else if (T->lchild == NULL) {
            BSTNode *temp = T;
            T = T->rchild;
            free(temp);
        }
        // 情况2：只有左孩子
        else if (T->rchild == NULL) {
            BSTNode *temp = T;
            T = T->lchild;
            free(temp);
        }
        // 情况3：有左右两个孩子
        else {
            // 方法：用右子树的最小值节点替代当前节点，再删除最小值节点
            int minKey;
            DeleteMin(T->rchild, minKey);
            T->key = minKey;
        }
        return true; // 删除成功
    }
}

// 辅助函数：中序遍历（验证二叉排序树的正确性）
void InOrder(BSTree T) {
    if (T != NULL) {
        InOrder(T->lchild);
        printf("%d ", T->key);
        InOrder(T->rchild);
    }
}

// 测试示例
int main() {
    int arr[] = {5, 3, 7, 2, 4, 6, 8};
    int n = sizeof(arr) / sizeof(arr[0]);
    BSTree T;
    
    // 构建二叉排序树
    Creat_BST(T, arr, n);
    printf("构建后中序遍历：");
    InOrder(T); // 应输出：2 3 4 5 6 7 8
    printf("\n");
    
    // 删除叶子节点（2）
    int delKey;
    BST_Delete(T, 2, delKey);
    printf("删除%d后中序遍历：", delKey);
    InOrder(T); // 应输出：3 4 5 6 7 8
    printf("\n");
    
    // 删除单孩子节点（3）
    BST_Delete(T, 3, delKey);
    printf("删除%d后中序遍历：", delKey);
    InOrder(T); // 应输出：4 5 6 7 8
    printf("\n");
    
    // 删除双孩子节点（5）
    BST_Delete(T, 5, delKey);
    printf("删除%d后中序遍历：", delKey);
    InOrder(T); // 应输出：4 6 7 8
    printf("\n");
    
    // 删除不存在的节点（9）
    BST_Delete(T, 9, delKey);
    printf("删除不存在的9，返回key：%d\n", delKey); // 输出-1
    
    return 0;
}