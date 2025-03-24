#include <stdio.h>
#include <stdlib.h>

int main() {
    short arr[3] = {1, 2, 3};

    printf("MemVolume char %d\n", sizeof(char));
    printf("MemVolume short %d\n", sizeof(short));
    printf("MemVolume int %d\n", sizeof(int));
    printf("MemVolume float %d\n", sizeof(float));
    printf("MemVolume double %d\n", sizeof(double));

    printf("MemVolume short arr %d\n", (sizeof(arr)));
    return 0;
}