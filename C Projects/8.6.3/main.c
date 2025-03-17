#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    int arr[5] = {5, 2, 8, 1, 9};
    int *ptr = arr;

    for (int i = 0; i < 5; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");

    float sum = 0;
    int min = *ptr;
    int max = *ptr;

    for (int i = 0; i < 5; i++) {
        int val = *(ptr + i);
        sum += val;
        if (val < min) min = val;
        if (val > max) max = val;
    }

    float average = sum / 5;
    printf("Среднее: %.2f\n", average);
    printf("Минимум: %d\n", min);
    printf("Максимум: %d\n", max);

    int sortedArr[5];
    for(int i=0; i<5; i++){
        sortedArr[i] = arr[i];
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4 - i; j++) {
            if (sortedArr[j] > sortedArr[j + 1]) {
                int temp = sortedArr[j];
                sortedArr[j] = sortedArr[j + 1];
                sortedArr[j + 1] = temp;
            }
        }
    }

    float median = (float)sortedArr[2];
    printf("Медиана: %.2f\n", median);

    return 0;
}
