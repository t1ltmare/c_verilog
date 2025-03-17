#include <stdio.h>
#include <stdlib.h>

float A = 2.78;
int B = 651;
int C = 22;
int D = 57;
float E = 518.;
float F = 43.15432;

void printVar(void)
{
    float a = 1.32;
    int b = 15;
    float c = 0.1567;
    int d = 22;
    int e = 518;
    float f = 287.154;
    printf("%.2f, %d, %.4f, %d, %d, %.3f\n", a, b, c, d, e, f);
}

void printVar2(void)
{
    static int x = 5;
    ++x;
    printf("%d\n", x);
    return 0;
}

int main()
{
    int x;
    printVar();
    printf("%.2f, %d, %d, %d, %.1f, %.5f\n\n", A, B, C, D, E, F);
    printVar2();
    printVar2();
    printVar2();
    printVar2();
    printVar2();
    return 0;
}
