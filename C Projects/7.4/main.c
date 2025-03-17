#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "Russian");
    int A[10];
    int i;
    float sum = 0;
    float average;

    // Заполнение массива
    printf("Введите 5 целых чисел:\n");
    for (i = 0; i < 5; i++) {
        printf("A[%d] = ", i);
        scanf("%d", &A[i]);
        sum += A[i]; // Суммируем элементы массива
    }

    // Вычисление среднего значения
    average = sum / 5;

    // Вывод массива
    printf("Массив A: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    // Вывод среднего значения
    printf("Среднее значение: %.2f\n", average);

    int max;
    int maxIndex;

    // Заполнение массива
    printf("\nВведите 10 целых чисел:\n");
    for (i = 0; i < 10; i++) {
        printf("A[%d] = ", i);
        scanf("%d", &A[i]);
    }

    // Поиск максимального значения и его индекса
    max = A[0];  // Предполагаем, что первый элемент - максимальный
    maxIndex = 0; // Индекс максимального элемента пока равен 0

    for (i = 1; i < 10; i++) {
        if (A[i] > max) {
            max = A[i];       // Нашли новый максимум
            maxIndex = i;    // Обновляем индекс максимального элемента
        }
    }

    // Вывод массива
    printf("Массив A: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    // Вывод максимального значения и его индекса
    printf("Максимальное значение: %d\n", max);
    printf("Индекс максимального элемента: %d\n", maxIndex);

    return 0;
}
