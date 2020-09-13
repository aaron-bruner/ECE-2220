#include <stdio.h>

int main (void)
{
    char c[80];
    int n=0;
    printf("Enter a three-digit nonnegative number: ");
    fgets(c, sizeof(c), stdin);
    n = (c[0]-48)*2*2+(c[1]-48)*2+(c[0]-48);
    printf("The number is %d\n", n);

} // ends main





/*#include <stdio.h>

int main (void)
{
    char c[80];
    int n=0, o=0, u, temp1, temp2 = 1;
    printf("Enter a three-digit nonnegative number: ");
    fgets(c, sizeof(c), stdin);

    while (c[o] != 10) {
        o++;
    }
    u = o;
    for (int y = 0; y < u; y++, o-=1) {
        if (c[y] == '1') {
            if (o != 0) {
                temp1 = o;
                while (temp1 != 0) {
                    temp2 *= 2;
                    --temp1;
                } // ends while loop
            } //end second if
            else
                temp2 = 1;
        } // ends first if
            n += temp2;

    } // ends for
    printf("The number is %d\n", n);

} // ends main
*/




/*#include <stdio.h>
#include <math.h>

int main (void)
{
    char c[80];
    int n=0, o=0, u, temp1, temp2;
    printf("Enter a three-digit nonnegative number: ");
    fgets(c, sizeof(c), stdin);

    while (c[o] != 10) {
        o++;
    }
    u = o;
    for (int y = 0; y < u; y++, o-=1) {
        if (c[y] == '1') {
            n += pow(2, o-1);
        }
    }
    printf("The number is %d\n", n);

}*/