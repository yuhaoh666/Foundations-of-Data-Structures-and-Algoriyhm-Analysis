#include <stdio.h>
#include <math.h>
#define MAX 100

typedef struct TreeNode {
    int data;
    bool isEmpty;
} TreeNode;

void initTree(TreeNode tree[]) {
    for (int i = 0; i < MAX; i++) {
        tree[i].isEmpty = true;
    }
}




int main ()
{
    TreeNode tree[MAX];
    initTree(tree);
    return 0;
}