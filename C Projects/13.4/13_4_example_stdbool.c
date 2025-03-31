#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

bool FLAG = false;

int main()
{
    int arr[10];
    srand(time(NULL));
    for(int i = 0; i < 10; i++)
    {
        arr[i] = rand()%100;
        printf(" %d", arr[i]);
    }
    
    //if (FLAG == true) printf("FLAG TRUE");
    //else printf("FLAG FALSE");

    return 0;
}