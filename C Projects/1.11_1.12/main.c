 #include <stdio.h>
#include <stdlib.h>

int main()
{
    int c, nl, ns, nt;

    nl = 0;
    ns = 0;
    nt = 0;

    while ((c = getchar()) != EOF)
        if (c == '\n'){++nl;}
        else if (c == ' '){++ns;}
        else if (c == '\t'){++nt;}

    printf("Number of strings = %d\n", nl);
    printf("Number of spaces = %d\n", ns);
    printf("Number of tabs = %d\n", nt);
}
