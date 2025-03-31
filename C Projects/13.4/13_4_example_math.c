#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main()
{
    const float u = 230.2;
    
    int a = 4;
    int b = sqrt(a);
    float c;
    float d = 4.4;
    printf("SQRT(%d) = %d\n", a, b); // корень из a

    b = pow(a,a);
    printf("POW(%d ^ %d) = %d\n", a, a, b); // возведение а в степень а

    c = powf(d, a);
    printf("POWF(%f ^ %d) = %.2f\n", d, a, c); // возведение d в степень а

    c = cos(0);
    printf("COS(0) = %.2f\n", c); // косинус

    for (int i = 0; i < 10; i++) // построение синусоидального графика из 10 точек
    {
        c = u*sin(0.3*i);
        printf("%f ", c);
    }

    return 0;
}