#include <stdio.h>
#include <stdlib.h>

int main()
{
    char a[6] = {'S', 'k', 'i', 'l', 'l'};
    char b[6] = {'B', '\n', 'o', '\n', 'x'};
    printf("%d %d %d %d \n\n", 23, 56, 73, 22); //целые в строку
    printf("%d \n%d \n%d \n%d \n\n", 443, 345, 21, 78);
    printf("%2.2f %2.2f %2.2f %2.2f \n\n", 1.27, 2.71, 3.14, 9.81);
    printf("%2.2f \n%2.2f \n%2.2f \n%2.2f \n\n", 6.62, 1.6, 7.81, 5.68);
    printf("%s\n\n",a);
    printf("%s\n",b);
}
