#include <stdio.h>

void test (int & x)
{
    x = 36;
    printf("%d\n", x);
}

int main () 
{
    int x = 6;
    printf("%d\n", x);
    test (x);
    printf("%d\n", x);
    return 0;
}