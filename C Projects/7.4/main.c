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

    // ���������� �������
    printf("������� 5 ����� �����:\n");
    for (i = 0; i < 5; i++) {
        printf("A[%d] = ", i);
        scanf("%d", &A[i]);
        sum += A[i]; // ��������� �������� �������
    }

    // ���������� �������� ��������
    average = sum / 5;

    // ����� �������
    printf("������ A: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    // ����� �������� ��������
    printf("������� ��������: %.2f\n", average);

    int max;
    int maxIndex;

    // ���������� �������
    printf("\n������� 10 ����� �����:\n");
    for (i = 0; i < 10; i++) {
        printf("A[%d] = ", i);
        scanf("%d", &A[i]);
    }

    // ����� ������������� �������� � ��� �������
    max = A[0];  // ������������, ��� ������ ������� - ������������
    maxIndex = 0; // ������ ������������� �������� ���� ����� 0

    for (i = 1; i < 10; i++) {
        if (A[i] > max) {
            max = A[i];       // ����� ����� ��������
            maxIndex = i;    // ��������� ������ ������������� ��������
        }
    }

    // ����� �������
    printf("������ A: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    // ����� ������������� �������� � ��� �������
    printf("������������ ��������: %d\n", max);
    printf("������ ������������� ��������: %d\n", maxIndex);

    return 0;
}
