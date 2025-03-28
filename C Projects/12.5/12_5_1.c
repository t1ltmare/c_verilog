// TODO С помощью malloc, calloc, free выделить память для динамического массива, размер которого вводится из консоли
// TODO Массив заполняется из консоли
// TODO Полученный массив выводится в консоль поэлементно в строку
// TODO После вывода массива необходимо освободить память, далее заново попробовать вывести этот массив

#include <stdio.h>
#include <stdlib.h>

int main() {
    int size;
    int *array;
  
    // 1. Input array size from console
    printf("Enter array size: ");
    scanf("%d", &size);

    // 2. Allocate memory for array
    int *ptr = malloc(size * sizeof(int));

    // 3. Fill an array
    for (int i = 0; i < size; i++)
    {
      printf("Enter the %d element of array ", i+1);
      scanf("%d", &ptr[i]);
    }

    // 4. Print an array
    for (int i = 0; i < size; i++)
    {
      printf("\nThe %d element of array is %d", i+1, ptr[i]);
    }

    // 5. Free the memory
    printf("\n\nMemory is freed");
    free(ptr);

    // 6. Print an array
    for (int i = 0; i < size; i++)
    {
      printf("\nThe %d element of array is %d", i+1, ptr[i]);
    }

    return 0;
  }