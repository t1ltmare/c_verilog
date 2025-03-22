#include <stdio.h>

/*
struct square_t
{
    int a;
    int b;
    int S;
};
*/

typedef struct 
{
    int a;
    int b;
    float h;
    int S;
    float V;    
}volume_t;

void Volume(volume_t * vl)
{
    vl->S = vl->a * vl->b;
    vl->V = (float)(vl->S)*vl->h;
}

void Print(volume_t * v)
{
    printf("Square %d\n", v->S);
    printf("Volume %f\n", v->V);
}

/*
void Square(struct square_t * sq)
{
    sq->S = sq->a * sq->b;
}


void PrintSquare(struct square_t * sq)
{
    printf("Square %d\n", sq->S);
}
*/

int main()
{
    volume_t vol;
    vol.a = 5;
    vol.b = 2;
    vol.h = 3.14;

    Volume(&vol);
    Print(&vol);

    /*
    struct square_t sqr = {.a = 5, .b = 5};
    struct square_t sqr2;
    sqr2.a = 2;
    sqr2.b = 3;
    Square(&sqr2);
    PrintSquare(&sqr2);
    */

    return 0;  
}

/*
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
*/

      
