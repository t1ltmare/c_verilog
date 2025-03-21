#include <stdio.h>

#define CMPL 1
#define HELLO printf("Hello \n")
#define BYE   printf("Good bye \n");
#define count 1

#define cycle(x) for(int i = 0; i<x; i++)

int main()
{
    #if CMPL == 0
    HELLO;
    #elif CMPL != 0
    BYE
    #endif

    return 0;
}