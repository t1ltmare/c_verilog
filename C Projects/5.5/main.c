#include <stdio.h>
#include <stdlib.h>

int main()
{
    // #1
    // SUM
    int a = 25;
    int b = 35;
    float d = 12.78;
    float e = 18.22;
    float g = 21.35;
    int h = 15;

    int c = a + b;
    float f = d + e;
    float i = g + h;

    printf("25 + 35 = %d\n12.78 + 18.22 = %.2f\n21.35 + 15 = %.2f\n\n", c, f, i);

    // SUB
    int j = 17;
    int k = 6;
    int m = 25;
    int n = 42;
    int p = 21;
    float q = 13.54;

    int l = j - k;
    int o = m - n;
    float r = p - q;

    printf("17 - 6 = %d\n25 - 42 = %d\n21 - 13.54 = %.2f\n\n", l, o, r);

    // MUL
    int a1 = 65;
    int b1 = 3;
    float d1 = 3.14;
    float e1 = 2.71;
    float g1 = 9.8;
    int h1 = 4;

    int c1 = a1 * b1;
    float f1 = d1 * e1;
    float i1 = g1 * h1;

    printf("65 * 3 = %d\n3.14 * 2.71 = %f\n9.8 * 4 = %.2f\n\n", c1, f1, i1);

    // DIV
    int j1 = 12;
    int k1 = 4;
    int m1 = 15;
    int n1 = 4;
    int p1 = 7;
    int q1 = 3;
    float s1 = 21.4;
    float t1 = 4.0;

    int l1 = j1%k1;
    int o1 = m1%n1;
    int r1 = p1 % q1;
    float u1 = fmod(21.4, 4.0);;

    printf("12 % 4 = %d\n15 % 4 = %d\n27 % 3 = %d\n21.4 % 4 = %.1f\n", l1, o1, r1, u1);
}
