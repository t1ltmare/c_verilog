#include <stdio.h>
#include <stdlib.h>

int main()
{
    int c, nl;

    nl = 0;

    while ((c = getchar()) != EOF){
        if (c == '\n')
            ++nl;
        if (((c = getchar()) != EOF) == true)
        printf("јуеть, это выражение = true");
    else if (((c = getchar()) != EOF) == false)
        printf("ѕиздец, это выражение = false");
    else printf("Ѕл€ть, ниху€ не вышло");
    }
    printf("Answer is %d\n",nl);
    printf ("EOF = %i\n", EOF);
}
