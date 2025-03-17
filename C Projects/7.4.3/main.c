#include <stdio.h>
#include <float.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "Russian");
    double A[4][5];
    double B[4];
    int i, j;

    // Заполнение массива A (замените на свой способ заполнения)
    printf("Введите элементы массива A[4][5]:\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            printf("A[%d][%d] = ", i, j);
            scanf("%lf", &A[i][j]); // Используем %lf для double
        }
    }

    // Вычисление значений массива B
    for (i = 0; i < 4; i++) {
        if (A[i][0] >= 0) {
            // Находим максимум в i-й строке
            B[i] = -DBL_MAX; // Инициализируем B[i] очень маленьким значением
            for (j = 0; j < 5; j++) {
                if (A[i][j] > B[i]) {
                    B[i] = A[i][j];
                }
            }
        } else {
            // Находим минимум в i-й строке
            B[i] = DBL_MAX; // Инициализируем B[i] очень большим значением
            for (j = 0; j < 5; j++) {
                if (A[i][j] < B[i]) {
                    B[i] = A[i][j];
                }
            }
        }
    }

    // Вывод массива A
    printf("Массив A:\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            printf("%.2lf ", A[i][j]);
        }
        printf("\n");
    }

    // Вывод массива B
    printf("Массив B:\n");
    for (i = 0; i < 4; i++) {
        printf("B[%d] = %.2lf\n", i, B[i]);
    }
    return 0;
}
