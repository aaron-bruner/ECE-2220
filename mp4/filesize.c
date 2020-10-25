/* filedump.c
 * Open a file and store all characters in dynamic memory
 */
#include <stdio.h>
#include <stdlib.h> /* for exit() function */

int main(int argc, char *argv[])
{
    FILE    *fpt;
    char    ch;
    int     count;
    char    *ptrFileInMemory;

    if (argc != 2) {
        printf("Usage:  filesize file\n");
        exit(1);
    }

    fpt=fopen(argv[1],"r");
    if (fpt == NULL) {
        printf("Unable to open %s for reading\n",argv[1]);
        exit(1);
    }

    count = 0;
    while (fscanf(fpt,"%c",&ch) == 1) {
        count++;
    }
    fclose(fpt);
    printf("The file contains %d bytes\n", count);

    ptrFileInMemory = (char *) malloc(count * sizeof(char));

    if (ptrFileInMemory ==  NULL) {
        printf("Error: Cannot Allocate Space for File\n");
        exit (1);
    }

    // read in the file and save in memory starting at ptrFileInMemory
    count = 0;
    fpt=fopen(argv[1],"r");
    while (fscanf(fpt,"%c",&ch) == 1) {
        // same as *(ptrFileInMemory + count) = ch;
        ptrFileInMemory[count] = ch;
        // to print address, same as &(ptrFileInMemory[count])
        printf("Byte at address %p is %u\n", ptrFileInMemory + count, ch);
        count++;
    }
    fclose(fpt);
    return 0;
}
