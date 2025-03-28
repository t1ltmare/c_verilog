#include <windows.h>
#include <stdio.h>

int main() {
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
    for (int i = 0; i < arrsize; i++) // Fill an array
    {
        printf("Enter the %d array number ", i+1);
        scanf("%d", &arr[i]);
    }
    
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

    printf("Full array after memory release:\n");
    for (int i = 0; i < arrsize; i++) // Print an array
    {
        printf("%d ", arr[i]);
    }

    return 0;
}