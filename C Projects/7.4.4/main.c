#include <stdio.h>
#include <stdbool.h> // ��� ������������� ���� bool
#include <locale.h>
#define SIZE 10

// ������� ��� ��������, ���������� �� ������ �� �����������
bool isAscending(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) {
            return false; // ���� ������ �������, ���������� �������, ���������� false
        }
    }
    return true; // ���� ����� �� ����� �������, ������, �� ����������
}

// ������� ��� �������������� �������: ������ ������������� ��������� �� 0
void transformArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] > 0) {
            arr[i] = 0;
        }
    }
}

// ������� ��� ������ ������� �� �����
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

    // ���� ��������
    printf("������� �������� ������� X[10]:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("X[%d] = ", i);
        scanf("%d", &X[i]);
    }

    printf("������� �������� ������� Y[10]:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("Y[%d] = ", i);
        scanf("%d", &Y[i]);
    }

    // ����� �������� ��������
    printArray(X, SIZE, "�������� ������ X");
    printArray(Y, SIZE, "�������� ������ Y");

    // �������������� ��������
    if (isAscending(X, SIZE)) {
        transformArray(X, SIZE);
    }

    if (isAscending(Y, SIZE)) {
        transformArray(Y, SIZE);
    }

    // ����� ��������� ��������
    printArray(X, SIZE, "���������� ������ X");
    printArray(Y, SIZE, "���������� ������ Y");

    return 0;
}
