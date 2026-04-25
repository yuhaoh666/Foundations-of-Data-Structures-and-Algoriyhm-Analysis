// 链表的游标实现，可用于其他不含指针的高级语言

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100

typedef struct {
    int data;
    int next;
} SLinkList[MAXSIZE];

void InitSpace(SLinkList space) { // 声明一个数组
    space[0].next = -1;
    for (int i = 1; i < MAXSIZE - 1; i++)  // 把其他节点的next设为某个特殊的值表示空闲
        space[i].next = -2;
}

bool SpaceInsert(SLinkList &space, int i, int e) { // 在备用链表中分配一个节点
    int i = space[0].next;  // 从备用链表的头节点开始
    int j = 0;
    while (i != -1 && j < i) {  // 遍历备用链表，找到第i个空闲节点
        i = space[i].next;
        j++;
    }
    if (i == -1) 
        return false;
    space[i].data = e;  // 设置分配节点的数据域
    space[i].next = -1;  // 更新备用链表的头节点指向下一个空闲节点   
    return true;
}