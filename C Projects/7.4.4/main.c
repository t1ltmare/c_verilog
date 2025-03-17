#include <stdio.h>
#include <stdbool.h> // Для использования типа bool
#include <locale.h>
#define SIZE 10

// Функция для проверки, упорядочен ли массив по возрастанию
bool isAscending(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) {
            return false; // Если найден элемент, нарушающий порядок, возвращаем false
        }
    }
    return true; // Если дошли до конца массива, значит, он упорядочен
}

// Функция для преобразования массива: замена положительных элементов на 0
void transformArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] > 0) {
            arr[i] = 0;
        }
    }
}

// Функция для вывода массива на экран
void printArray(int arr[], int size, const char* arrayName) {
    printf("%s: ", arrayName);
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "Russian");
    int X[SIZE];
    int Y[SIZE];

    // Ввод массивов
    printf("Введите элементы массива X[10]:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("X[%d] = ", i);
        scanf("%d", &X[i]);
    }

    printf("Введите элементы массива Y[10]:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("Y[%d] = ", i);
        scanf("%d", &Y[i]);
    }

    // Вывод исходных массивов
    printArray(X, SIZE, "Исходный массив X");
    printArray(Y, SIZE, "Исходный массив Y");

    // Преобразование массивов
    if (isAscending(X, SIZE)) {
        transformArray(X, SIZE);
    }

    if (isAscending(Y, SIZE)) {
        transformArray(Y, SIZE);
    }

    // Вывод изменённых массивов
    printArray(X, SIZE, "Измененный массив X");
    printArray(Y, SIZE, "Измененный массив Y");

    return 0;
}
