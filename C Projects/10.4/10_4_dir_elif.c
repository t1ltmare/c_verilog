#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define startnum 0
#define ENTER printf("\n")
#define CHOOSEPROG 1
#define FIBO printfibo()
#define SAPER saper()

int fibo(int n)
{
    if (n <= 1) return n;
    else return fibo(n - 1) + fibo(n - 2);
}

int printfibo() 
{
    printf("Fibo numbers:\n");
    int i = startnum;
    while (fibo(i) <= 2584) {
        printf("%d ", fibo(i));
        i++;
    }
    ENTER;
    return 0;
}

// * Начало программы saper

int saper() 
{
    srand(time(NULL));

    int x;
    int ind = 0;
    int arr1[5][5];
    int arr2[5][5];
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            arr1[i][j] = rand() % 10;
            arr2[i][j] = 0;
            //printf("%d ", arr1[i][j]);
        }
        //enter;
    }

    while(ind < 25) {
        printf("Enter your num from 0 to 9\n");
        scanf("%d", &x);
    
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (arr1[i][j] == x){
                    printf("%d ", x);
                    arr2[i][j] = 1;
                    ind++;
                } else if (arr2[i][j] == 1){
                    printf("%d ", arr1[i][j]);
                } else {
                    printf("[x]");
               }
            }
            ENTER;
        }
    }
    printf("YOU WIN!");
    return 0;
}

int main() 
{
    #if CHOOSEPROG == 0
    FIBO;
    #elif CHOOSEPROG != 0
    SAPER;
    #endif

    return 0;
}