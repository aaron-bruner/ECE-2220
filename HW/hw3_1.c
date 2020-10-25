#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {

    for (int i = 0; i < argc; i++) {
        for (int j = 0; j < argc-1; j++) {
            if (strcmp(argv[i],argv[j]) == 0)
                printf("ARGUMENT %d AND %d ARE THE SAME\nFIRST ARGUMENT: %s\nSECOND ARGUMENT: %s\n\n", i, j, argv[i], argv[j]);
        }
    }
    return 0;
}