#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");

    int myArray[5] = {5, 2, 8, 1, 9};
    int *ptr = myArray;

    printf("Исходный массив: ");

    for (int i = 0; i < 5; i++) {
        printf("%d ", *ptr);
        ptr++;
    }

    printf("\n");

    ptr = myArray;
    for (int i = 0; i < 4; i++) {
        int *ptr1 = myArray;
        for (int j = 0; j < 4 - i; j++) {
            int *ptr2 = ptr1 + 1;
            if (*ptr1 > *ptr2) {
                int temp = *ptr1;
                *ptr1 = *ptr2;
                *ptr2 = temp;
            }
        ptr1++;
        }
    }

    ptr = myArray;
    printf("Отсортированный массив: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");

    return 0;
}
