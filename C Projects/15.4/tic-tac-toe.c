//TODO Сделать игру крестики нолики PVP + PVE на выбор
//TODO 1. Для PVP ввести 1, для PVE ввести 2
//TODO 2. Создать массив с игровым полем
//TODO 3. Создать функцию вывода массива
//TODO 4. Создать функцию в которой число проверяется на четность (для определения порядка хода. четные - крестик, нечетные - нолик)
//TODO 5. Ввод игроком строки и столбца, чтобы походить
//TODO 6. Запись нужного знака в указанную клетку
//TODO 7. Проверка выполнения условия победы
//TODO 8. Добавление игры PVE. Случайные числа в качестве хода соперника
//TODO 9. Добавление функции случайного начала хода - вы или ИИ
//TODO 10. Добавление задержек между игровыми действиями для постепенного вывода в консоль

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   

int start()
{
    int x;
    printf("Welcome to the tic-tac-toe!\n");
    printf("1 for PVP, 2 for PVE\n");
    scanf("%d", &x);
    return x;
}

void print(char arr[3][3])
{
    printf("    1   2   3\n\n");
    printf("1   %c   %c   %c\n\n", arr[0][0], arr[0][1], arr[0][2]);
    printf("2   %c   %c   %c\n\n", arr[1][0], arr[1][1], arr[1][2]);
    printf("3   %c   %c   %c\n\n", arr[2][0], arr[2][1], arr[2][2]);
}

int winCheck(char arr[3][3])
{
    int win;
    if(arr[0][0] == arr[0][1] && arr[0][1] == arr[0][2] && arr[0][0] != '*') win = 1;
    else if(arr[1][0] == arr[1][1] && arr[1][1] == arr[1][2] && arr[1][0] != '*') win = 1;
    else if(arr[2][0] == arr[2][1] && arr[2][1] == arr[2][2] && arr[2][0] != '*') win = 1;
    else if(arr[0][0] == arr[1][0] && arr[1][0] == arr[2][0] && arr[0][0] != '*') win = 1;
    else if(arr[0][1] == arr[1][1] && arr[1][1] == arr[2][1] && arr[0][1] != '*') win = 1;
    else if(arr[0][2] == arr[1][2] && arr[1][2] == arr[2][2] && arr[0][2] != '*') win = 1;
    else if(arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2] && arr[0][0] != '*') win = 1;
    else if(arr[0][2] == arr[1][1] && arr[1][1] == arr[2][0] && arr[0][2] != '*') win = 1;
    else if(arr[0][0] != '*' && arr[0][1] != '*' && arr[0][2] != '*' && arr[1][0] != '*' && arr[1][1] != '*' && arr[1][2] != '*' && arr[2][0] != '*' && arr[2][1] != '*' && arr[2][2] != '*') win = 2;
    else win = 0;
    return win;
}

int main()
{
    int row, col, turn = 0, win = 0;
    srand(time(NULL)); 
    
    char arr[3][3] = {{'*', '*', '*'},
                      {'*', '*', '*'},
                      {'*', '*', '*'}};

    if (start() == 1)
    {
        print(arr);
        printf("\n");

        while (!win)
        {
            if (turn % 2) printf("2'nd player's turn\n\n");
            else printf("1'st player's turn\n\n");
            printf("Enter the row ");
            scanf("%d", &row);
            printf("Enter the column ");
            scanf("%d", &col);
            
            if (turn % 2 && arr[row - 1][col - 1] == '*') arr[row - 1][col - 1] = 'O';
            else if (arr[row - 1][col - 1] == '*') arr[row - 1][col - 1] = 'X';
            else printf("\nError: Cell already occupied!\n");
            
            print(arr);

            win = winCheck(arr);
            if (win == 1) printf("Player %d wins!\n", turn % 2 + 1);
            else if (win == 2) printf("Draw!\n");

            turn++;
        }
    }
    else
    {
        turn = rand() % 2;
       
    }
    
    return 0;
}