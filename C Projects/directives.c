#include <stdio.h>

#define HELLO printf("Hello \n")
#define BYE   printf("Good bye \n");

#define cycle(x) for(int i = 0; i<x; i++)

int main()
{
    cycle(10) HELLO;

    return 0;
}