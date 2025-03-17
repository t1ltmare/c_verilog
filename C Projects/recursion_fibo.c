#include <stdio.h>

int fibo(int n)
{
    if (n <= 1) return n;
    else return fibo(n - 1) + fibo(n - 2);
}

int main() {
    printf("Fibo numbers:\n");
    int i = 0;
    while (fibo(i) <= 2584) {
        printf("%d ", fibo(i));
        i++;
    }
    printf("\n");
    return 0;
}
