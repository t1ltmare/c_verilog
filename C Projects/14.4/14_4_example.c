#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void main()
{
    //* Вытаскиваем число из строки
/*
    const char * str = "14 module";
    printf("%s\n", str);
    long a = strtol(str, NULL, 10);
    printf("Data %d\n", a);
*/
    //* Переводим число в строку: sprintf или snprintf
/*
    int p = 52;
    char str2[10];
    //snprintf(str2, sizeof str2, "Hello %d", p);
    sprintf(str2, "Hello %d", p);
    printf("%s\n", str2);
*/
    //* Копируем одну строку в другую

    char * strtmp = "Hello world";
    char * str2[11];
    printf("strtmp: %s\n", strtmp);
    printf("str2: %s\n", str2);
    size_t l = strlen(strtmp);
    printf("strtmp length: %d\n", l);
    strcpy(str2, strtmp);
    printf("strtmp copied to str2 \n", str2);
    printf("str2: %s\n", str2);

}