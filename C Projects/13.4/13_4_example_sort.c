#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void sortArr(int * p, int n) // сотрировка массива
{
    int tmp;

    for(int i = 0; i < n - 1; ++i)
    {
        for(int j = i + 1; j < n; ++j)
        {
            if(p[j] < p[i])
            {
                tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }
}

void bubbleSort(int * p, int n) // сотрировка массива
{
    int tmp = 0;

    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(p[j] > p[j+1])
            {
                tmp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = tmp;
            }
        }
    }
}

void printArr(int * p, int n) // вывод массива
{
    for(int i = 0; i < n; i++)
    {
        printf("%d ", p[i]);
    }
}

void fillArr(int * p, int n) // наполнение массива
{
    for (int i = 0; i < n; i++)
    {
        p[i] = rand()%100;
    }
}

int main()
{
    srand(time(NULL));
    int arr[5000], brr[5000];
    int n = sizeof(arr)/sizeof(arr[0]);
    
    fillArr(arr, n);
    fillArr(brr, n);

    //printf("\nArray A before sort: \n");
    //printArr(arr, n);
    clock_t startA = clock();
    sortArr(arr, n);
    clock_t endA = clock();
    double timeA = (double)(endA - startA) / CLOCKS_PER_SEC;

    //printf("\n\nArray A after sort: \n");
    //printArr(arr, n);
    printf("\nTime taken to sort Array A: %.6f seconds\n", timeA);

    //printf("\nArray B before sort: \n");
    //printArr(brr, n);
    clock_t startB = clock();
    bubbleSort(brr, n);
    clock_t endB = clock();
    double timeB = (double)(endB - startB) / CLOCKS_PER_SEC;

    //printf("\n\nArray B after sort: \n");
    //printArr(brr, n);
    printf("\nTime taken to sort Array B: %.6f seconds\n", timeB);
}