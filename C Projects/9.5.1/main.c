#include <stdio.h>
#include <locale.h>

int fibo(int n)
{
    if (n <= 1) return n;
    else return fibo(n - 1) + fibo(n - 2);
}

int main() {
    setlocale(LC_ALL, "Russian");
    printf("Последовательность Фибоначчи:\n");
    int i = 0;
    while (fibo(i) <= 2584) {
        printf("%d ", fibo(i));
        i++;
    }
    printf("\n");
    return 0;
}
