/* Demonstrate writing to another terminal by opening device file */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUMTTYS 4

int main(int argc, char *argv[])
{ 
    FILE *fpt[NUMTTYS];
    int ttyindex;
    char strDev[100];
    int Terminal[NUMTTYS];
    int term_cnt = 0;

    if (argc != NUMTTYS+1) {
        printf("Usage: ./lab7 1 2 3 4, where the numbers are the terminal pts values\n");
        exit(1);
    }

    // determine which ttys are open and available to this user
    for (term_cnt = 0; term_cnt < NUMTTYS; term_cnt++)
    { 
        ttyindex = -1;
        ttyindex = atoi(argv[term_cnt+1]);
        if (ttyindex < 1) {
            printf("invalid terminal number %s\n", argv[term_cnt+1]);
            exit(1);
        }
                
	sprintf(strDev, "/dev/pts/%d", ttyindex);

	if ((fpt[term_cnt] = fopen(strDev,"w")) != NULL) {
	    Terminal[term_cnt] = ttyindex;
	} else {
            printf("failed to open terminal %s\n", strDev);
            exit(1);
        }
    }

    printf("This program can write to %d terminals\n", term_cnt);
    for (ttyindex=0; ttyindex < term_cnt; ttyindex++)
    { 
	printf("Terminal[%d] = %d\n", ttyindex, Terminal[ttyindex]);
        fprintf(fpt[ttyindex], "Hello terminal %d, your are %d in my list\n",
                Terminal[ttyindex], ttyindex);
    }

    for (ttyindex=0; ttyindex < term_cnt; ttyindex++)
    { 
	fclose(fpt[ttyindex]);
    }
    exit(EXIT_SUCCESS);
}