#include <stdio.h>
#include <float.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "Russian");
    double A[4][5];
    double B[4];
    int i, j;

    // ���������� ������� A (�������� �� ���� ������ ����������)
    printf("������� �������� ������� A[4][5]:\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            printf("A[%d][%d] = ", i, j);
            scanf("%lf", &A[i][j]); // ���������� %lf ��� double
        }
    }

    // ���������� �������� ������� B
    for (i = 0; i < 4; i++) {
        if (A[i][0] >= 0) {
            // ������� �������� � i-� ������
            B[i] = -DBL_MAX; // �������������� B[i] ����� ��������� ���������
            for (j = 0; j < 5; j++) {
                if (A[i][j] > B[i]) {
                    B[i] = A[i][j];
                }
            }
        } else {
            // ������� ������� � i-� ������
            B[i] = DBL_MAX; // �������������� B[i] ����� ������� ���������
            for (j = 0; j < 5; j++) {
                if (A[i][j] < B[i]) {
                    B[i] = A[i][j];
                }
            }
        }
    }

    // ����� ������� A
    printf("������ A:\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            printf("%.2lf ", A[i][j]);
        }
        printf("\n");
    }

    // ����� ������� B
    printf("������ B:\n");
    for (i = 0; i < 4; i++) {
        printf("B[%d] = %.2lf\n", i, B[i]);
    }
    return 0;
}
