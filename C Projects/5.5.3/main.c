#include <stdio.h>
#include <stdlib.h>

int main()
{
    //float b = (?) 2.4 + 3.7
    //float x = 2.4;
    //float y = 3.7;
    //float b = x + y.

    //int a = (?) 72.45 + 84.32
    int x = 72.45;
    int y = 84.32;
    int a = x + y;
    printf("%d\n", a);

    //float b = (?) 44.32 + 51.54
    float x1 = 44.32;
    float y1 = 51.54;
    float b1 = x1 + y1;
    printf("%.2f\n", b1);

    //float b = (?) 53 + 2.54
    float x2 = 53;
    float y2 = 2.54;
    float b2 = x2 + y2;
    printf("%.2f\n", b2);

    //float b = (?) 12 + 22
    float x3 = 12;
    float y3 = 22;
    float b3 = x3 + y3;
    printf("%.2f\n", b3);

    //int a = (?) 64.2154
    int a2 = 64.2154;
    printf("%d\n", a2);

    //float a = (?) 42
    float a3 = (int)42;
    printf("%.0f\n", a3);
}
