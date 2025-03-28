#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h> // For time functions

int main() {
    clock_t start1, end1, start2, end2;
    double cpu_time_used1;
    double cpu_time_used2;

    int size;
    int *array;
    int i;
  
    // 1. Input array size from console
    printf("Enter array size: ");
    scanf("%d", &size);

    // 2. Allocate memory for array
    int *ptr = malloc(size * sizeof(int));

    // 3. Fill an array
    for (i = 0; i < size; i++)
    {
      printf("Enter the %d element of array ", i+1);
      scanf("%d", &ptr[i]);
    }

    int memsize;
    int arrsize;
    printf("Enter the array size ");
    scanf("%d",&arrsize);
    memsize = arrsize * sizeof(int);
    printf("%d bytes will be allocated\n", memsize);

    LPVOID memoryBlock = VirtualAlloc(
        NULL,               // Let the system choose the address
        memsize,        // Size of the allocation
        MEM_COMMIT | MEM_RESERVE,  // Allocate physical memory
        PAGE_READWRITE      // Read-write access
    );

    printf("Memory allocated at address: %p\n\n", memoryBlock);

    // Use the allocated memory
    int* arr = (int*)memoryBlock;
    for (i = 0; i < arrsize; i++) // Fill an array
    {
        printf("Enter the %d array number ", i+1);
        scanf("%d", &arr[i]);
    }

    start1 = clock(); // Record the starting time

    // 4. Print an array
    printf("\nFull array:\n");
    for (int i = 0; i < size; i++)
    {
      printf("%d ", ptr[i]);
    }

    // 5. Free the memory
    printf("\n\nMemory is freed");
    free(ptr);

    end1 = clock(); // Record the ending time

    cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC; // Calculate execution time in seconds

    printf("\nExecution time of 2nd program: %f seconds\n", cpu_time_used1);

    start2 = clock(); // Record the starting time

    printf("\nFull array:\n");
    for (int i = 0; i < arrsize; i++) // Print an array
    {
        printf("%d ", arr[i]);
    }

    BOOL freed = VirtualFree(
        memoryBlock,    // Address of the region to free
        0,              // Free the entire region
        MEM_RELEASE      // Decommit and release
    );

    printf("\n\nMemory freed.\n\n");

    end2 = clock(); // Record the ending time

    cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC; // Calculate execution time in seconds

    printf("\nExecution time of 1st program: %f seconds\n", cpu_time_used2);

    return 0;
}