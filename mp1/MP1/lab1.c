/* lab1.c
 * Aaron Bruner
 * ajbrune
 * ECE 2220, Fall 2019
 * MP1
 *
 * NOTE:  You must update all of the following comments!
 * 
 * Purpose:  The purpose of machine problem one is to practice our C programming skills in handling inputs from the
 *           terminal along with processing outputs. Also, we are to use conditional statements and loops to get the
 *           desired output. We will also be manipulating arrays.
 *
 * Assumptions: During this machine problem we have very simple assumptions. (1) The person running the code has a
 *              simple understanding of how Linux commandline execution works (2) along with basic understanding of code
 *              structure. (3) The user is also expected to comply with the prompted input requests; however we will
 *              be feeding out input via file redirect so the person creating the file must adhere to the program
 *              specifications. (4) We also assume that the person running the code is able to compile the code since
 *              we will be providing the source code in the .c format. (5) Lastly, we must assume that the user is using
 *              the ASCII characters in the input file.
 *
 * Bugs:  A predicted bug would be if a user inputs a non ASCII character into the input file. Since we cannot make the
 *        user use the ASCII character encoding standard we have to make an assumption that they will. Another predicted
 *        bug would be if someone compiles the code on a non standard machine which may produce errors that were not
 *        initially produced.
 *
 * See the ECE programming guide for requirements
 *
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux to see different options.  Or, set up your
 * own style.
 *
 * To create a nicely formated PDF file for printing install the enscript 
 * command.  To create a PDF for "file.c" in landscape with 2 columns do:
 *     enscript file.c -G2rE -o - | ps2pdf - file.pdf
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//notice there is no semi-colon at the end of a #define
#define MAXLINE 100
#define MINNUM 10
#define MAXNUM 100
#define MINSIZE 6
#define MAXSIZE 12
#define MINKEY 1
#define MAXKEY 5


int main()
{
    int isPrime(int number);        //                  //
    void readMessage(void);         // Function Headers //
    void sendMessage(void);         //                  //

    char line[MAXLINE];         // the entire message
    int msgSize;                // amount of values per message
    int keyValue, id;           // key value is our prime checker and id the is last value in the array so
    int msgChecker = 0;         //
    int numOfPrimes;            //
    int sizeOfArr;              //

    fgets(line, MAXLINE, stdin);                    // This line gathers the entire input into the array line
    sscanf(line, "%d %d", &msgSize, &keyValue);     // This puts the first value in the line into msgSize

    sizeOfArr = sizeof(line) / sizeof(line[0]);  // Logic Check to make sure we have values to read
    if (sizeOfArr < 2 || NULL)
        exit(1);

    msgSize = line[0]-'0';
    

    //while (1) {



    //}


    printf("\nMsgsize is set to %d.\n", msgSize);
    printf("\nKeyValue is set to %d.\n", keyValue);

    exit(0);
}

int isPrime(int number) {
    if (number <= 1) return 0; // 0 implies non-prime
    if (number % 2 == 0 && number > 2) return 0;
    for(int i = 3; i < number / 2; i+= 2) {
        if (number % i == 0)
            return 0;
    }
    return 1; // 1 implies prime number
}

void readMessage (void) {


}

void sendMessage (void) {

    /* printf(">> No more message received. Program exit.\n\n\n");                             // if
    printf(">> Msg:  I'm safe, all good.\n");                                                  // numOfPrimes == 1
    printf(">> Msg:  Mission Success. Agent ID: %d.\n", id);                                   // numOfPrimes == 2
    printf(">> Msg:  Mission Failed. Agent ID: %d.\n", id);                                    // numOfPrimes == 4
    printf(">> Msg:  Don't contact me.\n");                                                    // numOfPrimes == 3
    printf(">> Msg:  No Content.    \n");                                                      // numOfPrimes > 4
    // or
    printf(">> Corrupted Message.\n");                                                         // if 11 > msg > 99
    printf("\n>> Agent Center is not safe now. Go find a safe place. Program exit.\n\n\n"); */ //-2

}