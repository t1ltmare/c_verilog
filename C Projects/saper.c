#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
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
        //printf("\n");
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
            printf("\n");
        }
    }
    printf("YOU WIN!");
    scanf("%d", &x);
}
