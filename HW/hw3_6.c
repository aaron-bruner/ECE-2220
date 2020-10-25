#include <stdio.h>

int main (void) {

    void square (int *p);

    int x;
    x = 13;
    square( &x );
    printf("x^2=%d\n", x);

    return 0;
}

void square (int *p) {
    *p = *p * *p;
}