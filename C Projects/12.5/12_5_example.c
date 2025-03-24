#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() 
{
    int *p1 = malloc(4);
    *p1 = 5;
    printf("malloc %d\n", *p1);
    free(p1);
    printf("free %d\n\n", *p1);

    char *p2 = calloc(5, 1);
    char *str = "Hello";
    strcpy(p2, str);
    printf("%s\n", p2);
    free(p2);
    printf("%s\n", p2);
    
    /*
    short arr[3] = {1, 2, 3};

    printf("MemVolume char %d\n", sizeof(char));
    printf("MemVolume short %d\n", sizeof(short));
    printf("MemVolume int %d\n", sizeof(int));
    printf("MemVolume float %d\n", sizeof(float));
    printf("MemVolume double %d\n", sizeof(double));

    printf("MemVolume short arr %d\n", (sizeof(arr)));
    return 0;
    */
}