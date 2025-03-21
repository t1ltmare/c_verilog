#include <stdio.h>

int main()
{
    struct myStr
    {
        int a;
        char * str;
    }

    typedef struct
    {
        int a;
        char * b;
    }myStr_t;

    struct myStr name1 = {5, "Hello"};
    struct myStr name2 = {.a = 6, .str = ""};

    myStr_t name3;
    name3.a = 3;
    name3.b = "Hello";

    return 0;    
}