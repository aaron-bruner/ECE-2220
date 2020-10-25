/*
 * Demonstrate dynamic memory allocation to get a chunk of
 * memory from the O/S.  Shows malloc, free, and accessing
 * the dynamically allocated memory using array indices and
 * using pointer arithmetic.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    double  *array;
    double  *d_ptr;
    double  value;
    int	i,offset;

    // memory for an array of 10 doubles
    array = (double *) malloc (10 * sizeof(double));
    if (array == NULL) {    // make sure O/S returned valid address
        printf("Unable to allocate array\n");
        exit(1);
    }
    for (i=0; i<10; i++)
        array[i] = (double) i + 10.0;  // access memory using array index

    while (1) {
        printf("Address(hex)\tAddress(base10)\tValue\n");
        for (i=0; i<10; i++)
            printf("%p\t%lu\t%g\n", &(array[i]),
                   (unsigned long int) &(array[i]), array[i]);
        printf("Enter offset value (0 0 to quit): ");
        scanf("%d %lf",&offset,&value);
        if (offset == 0  &&  value == 0.0)
            break;               // break out of loop
        if (offset < 0  ||  offset > 9) {
            printf("Offset out of bounds\n");
            continue;            // go back to start of loop
        }
        d_ptr = &(array[0]);     // access memory using pointer
        d_ptr += offset;         // pointer arithmetic
        *d_ptr = value;          // what is equivalent array statement?
    }
    free(array);                 // release chunk of memory back to O/S
    return 0;
}
