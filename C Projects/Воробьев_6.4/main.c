#include <stdio.h>
#include <stdlib.h>
#define MAX 100

//1. ������� � ������� ������� �� ������ �� ���� � ������� ����� while.
//2. ������� ������� ��������� �������� �� �������.
//3. ������� ���� ������ � ������� � ������� ����� � ��� ������������������,
//   � ������� �� �����, � ����� ��� ������ ����� for � �������� ����������
//   ��������� ������ ����� �� ����������� � ������� �� � ������� ������ ��
//   ������� �������� ������.
//4. ����� � ������������� ����� ��������� �� ����������: ��� ��������
//   ������������� �������������� ������� �� ������� ��������� ����� �
//   ����������� �������� ������� �� ������.

int main()
{
    /* // ������� 1
    int i = 9;
    while (i >= 0) {
        printf("%d\n", i);
        --i;
    }
    */

    /* // ������� 2
    int i = 1;
    int A[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    while (i <= 9) {
        int c = 0;
        while (c <= 9) {
            printf("%d ", A[c]*i);
            ++c;
        }
        printf("\n");
    ++i;
    }
    */



    /*
    // ������� 3

    // ���������� ���������� � �������
    int n, i, array[MAX];
    char num[MAX];
    int res;

    // ���� ������
    printf("Enter a list of integers [ctrl+d] to end\n");

    for(i = 0; i <= MAX; ++i){
        printf("> ");
        fgets(num, sizeof(num), stdin);
        res = sscanf(num, "%d", &n);
        if(res != 1)
            break;
        n = atoi(num);
        array[i] = n;
    }
    puts ("");

    // ���������� �� �����������

    int j, t = 0;
    n = i;

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (array[i] > array[j]) {
                t = array[i];
                array[i] = array[j];
                array[j] = t;
            }
        }
    }

    // ����� ������
    int z;
    for (z = 0; z < i; z++)
        printf("%d\n", array[z]);

    return 0;
    */



    // ������� 4

    printf( "Type a command\n" );
    printf( "1. Ready!\n" );
    printf( "2. Steady!!\n" );
    printf( "3. Go!!!\n" );
    printf( "Type a command: " );
    int input;
    scanf( "%d", &input );
    switch ( input ) {
        case 1:
            printf("Prepare!\n");
            break;
        case 2:
            printf("Concentrate!\n");
            break;
        case 3:
            printf("Run!!!\n");
            break;
        default:
            printf("Error. Type the right command\n" );
    }
    getchar();
    return 0;

}
