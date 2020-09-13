#include <stdio.h>

int main()
{
    unsigned var;
    int bits;

    printf("Enter a number: ");
    scanf("%d", &var);

    while (var != 0) {
        if (x & 01)
            bits++;
        else
            x >>= 1;
    }

    printf("The amount of bits in the var are %d\n", bits);

    return 0;
}

/*#include <stdio.h>

int main()
{
    int bitCounter (unsigned x);
    unsigned var;
    //unsigned z;
    printf("Enter a number: ");
    scanf("%d", &var);
    //z = (int)sizeof(var);

    printf("The amount of bits in the var are %d\n", bitCounter(var));

    return 0;
}

int bitCounter (unsigned x) {
    int bits;
    for (bits = 0; x != 0; x >>= 1) {
        if (x & 01)
            bits++;
    }
    return bits;
}*/