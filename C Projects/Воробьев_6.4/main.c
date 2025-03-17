#include <stdio.h>
#include <stdlib.h>
#define MAX 100

//1. Вывести в столбик счетчик от девяти до нуля с помощью цикла while.
//2. Выводит таблицу умножения Пифагора на консоль.
//3. Ожидает ввод данных с консоли и выводит числа в той последовательности,
//   в которой их ввели, а затем при помощи цикла for и условных операторов
//   сортирует данные числа по возрастанию и выводит их в консоль следом за
//   выводом исходных данных.
//4. Задаёт в переключателе режим «Значение по умолчанию»: при введении
//   пользователем несуществующей команды на консоль выводится текст с
//   требованием введения команды из списка.

int main()
{
    /* // задание 1
    int i = 9;
    while (i >= 0) {
        printf("%d\n", i);
        --i;
    }
    */

    /* // задание 2
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
    // задание 3

    // объявление переменных и массива
    int n, i, array[MAX];
    char num[MAX];
    int res;

    // ввод данных
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

    // сортировка по возрастанию

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

    // вывод ответа
    int z;
    for (z = 0; z < i; z++)
        printf("%d\n", array[z]);

    return 0;
    */



    // задание 4

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
