#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a = 5;
    int *p;
    p = &a;
    printf("Var %d\n", a);
    printf("Add %p\n", (void*)p);
    *p = 10;
    printf("Var %d\n", a);
    printf("Add %p\n", (void*)p);
    return 0;
}
