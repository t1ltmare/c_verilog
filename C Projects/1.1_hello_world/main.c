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
        printf("�����, ��� ��������� = true");
    else if (((c = getchar()) != EOF) == false)
        printf("������, ��� ��������� = false");
    else printf("�����, ����� �� �����");
    }
    printf("Answer is %d\n",nl);
    printf ("EOF = %i\n", EOF);
}
