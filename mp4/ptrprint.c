/* ptrprint.c
 *
 * An example showing printing an array using pointers
 * and marking the position of a selected item
 */

#include <stdio.h>

void InitMem(char * c, int size)
{
    int i;
    for (i=0; i<size; i++) c[i] = 'A' + i;
}

void PrintMem(char *c, int size, char *ptrc)
{
    int i, n;

    n = (int) (ptrc - c);  // offset from start of array
    printf("ptrc = &c[%d], *ptrc = %c\n", n, *ptrc);
    for (i=0; i<size; i++)
        printf(" %c ", c[i]);
    printf("\n");
    for (i=0; i<size; i++)
        printf(" %02X", c[i]);
    printf("\n");
    for (i=0; i<3*n; i++)
        printf(" ");

    printf(" ^\n\n");

}

//--------------------------------------------------------------------

int main(void)
{
    char c[10];
    char *ptrc;

    InitMem(c, 10);
    ptrc = (char *)c;

    ptrc += 5;
    printf("ptrc += 5;\n");
    PrintMem(c, 10, ptrc);

    ptrc -= 2;
    printf("ptrc -= 2;\n");
    PrintMem(c, 10, ptrc);

    InitMem(c, 10);
    *ptrc++;              // Note this causes a warning!!!
    printf("*ptrc++;\n");
    PrintMem(c, 10, ptrc);

    InitMem(c, 10);
    (*ptrc)++;
    printf("(*ptrc)++;\n");
    PrintMem(c, 10, ptrc);

    InitMem(c, 10);
    ++*ptrc;
    printf("++*ptrc;\n");
    PrintMem(c, 10, ptrc);

    InitMem(c, 10);
    ++(*ptrc);
    printf("++(*ptrc);\n");
    PrintMem(c, 10, ptrc);

    return 0;
}
