#include <stdio.h>
#include <stdlib.h>

int main()
{
    // #2
    // OR
    int a = 12;
    int b = 5;
    int c = a | b;
    printf("12 | 5 = %d\n", c); //13

    int d = 8;
    int e = 7;
    int f = d | e;
    printf("8 | 7 = %d\n", f); //15

    int g = 24;
    int h = 64;
    int i = g | h;
    printf("24 | 64 = %d\n\n", i); //88

    // AND
    int a1 = 27;
    int b1 = 3;
    int c1 = a1 & b1;
    printf("27 & 3 = %d\n", c1); //3

    int d1 = 16;
    int e1 = 0;
    int f1 = d1 & e1;
    printf("16 & 0 = %d\n", f1); //0

    int g1 = 31;
    int h1 = 1;
    int i1 = g1 & h1;
    printf("31 & 1 = %d\n\n", i1); //1

    // EXC
    int a2 = 5;
    int b2 = 6;
    int c2 = a2 ^ b2;
    printf("5 ^ 6 = %d\n", c2); //3

    int d2 = 5;
    int e2 = 5;
    int f2 = d2 ^ e2;
    printf("5 ^ 5 = %d\n", f2); //0

    int g2 = 8;
    int h2 = 0;
    int i2 = g2 ^ h2;
    printf("31 ^ 1 = %d\n\n", i2); //8

    // NOT (по условию задачи нужно сделать 6~1, 1~1, 0~0,
    // но такой записи не существует и компилятор ругается.
    // наверное, в задании ошибка)
    int a3 = 6;
    int b3 = ~a3;
    printf("~6 = %d\n", b3); //

    int c3 = 1;
    int d3 = ~c3;
    printf("~1 = %d\n", d3);

    int e3 = 0;
    int f3 = ~e3;
    printf("~0 = %d\n", f3);
}
