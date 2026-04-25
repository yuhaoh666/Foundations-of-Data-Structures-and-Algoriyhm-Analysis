#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *lchild, *rchild;
} Node, *HuffmanTree;

void InitHuffmanTree(HuffmanTree *T)
{
    *T = NULL;
}

// 根据数组创建哈夫曼树
HuffmanTree createHuffmanTree(int *weight, int n)    
{
    Node *nodes = (Node *)malloc(n * sizeof(Node));
    for (int i = 0; i < n; i++)
    {
        nodes[i].data = weight[i];
        nodes[i].lchild = NULL;
        nodes[i].rchild = NULL;
    }

    for (int i = 0; i < n - 1; i++)
    {
        // 找到最小的两个节点
        int min1 = 0, min2 = 1;
        if (nodes[min1].data > nodes[min2].data)
        {
            int temp = min1;
            min1 = min2;
            min2 = temp;
        }
        for (int j = 2; j < n - i; j++)
        {
            if (nodes[j].data < nodes[min1].data)
            {
                min2 = min1;
                min1 = j;
            }
            else if (nodes[j].data < nodes[min2].data)
            {
                min2 = j;
            }
        }

        // 创建新节点
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->data = nodes[min1].data + nodes[min2].data;
        newNode->lchild = &nodes[min1];
        newNode->rchild = &nodes[min2];

        // 替换最小的两个节点
        nodes[min1] = *newNode;
        nodes[min2] = nodes[n - i - 1];
    }

    return &nodes[0]; // 返回根节点
}

int main()
{
    int weight[] = {5, 9, 12, 13, 16, 45};
    int n = sizeof(weight) / sizeof(weight[0]);

    HuffmanTree T = createHuffmanTree(weight, n);
    printf("哈夫曼树创建成功！\n");

    return 0;
}