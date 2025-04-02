#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Function to allocate memory for an array
void memAlloc(double **p, int n)
{
    *p = malloc(n * sizeof(double));
    if (*p == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
}

// Function to print an array
void printArr(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%.2f ", p[i]);
    }
}

// Function to fill an array with sine values
void fillArr(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        p[i] = sin(i);
    }
}

void addNoise(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        double variation = (double)(rand() % 10 - 5) / 100.0; 
        p[i] += variation;
    }
}

int main()
{
    printf("Enter the array size\n");
    int size;
    if (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("Error: Invalid array size input.\n");
        return 1;
    }

    double *arr = NULL;

    srand(time(NULL));

    memAlloc(&arr, size);
    fillArr(arr, size);
    printArr(arr, size);
    printf("\n");
    addNoise(arr, size);
    printArr(arr, size);

    // Free the allocated memory
    free(arr);

    return 0;
}