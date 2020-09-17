/* lab1.c
 * Aaron Bruner
 * ajbrune
 * ECE 2220, Fall 2020
 * MP1
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
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 100
#define MINNUM 10
#define MAXNUM 100
#define MINSIZE 6
#define MAXSIZE 12
#define MINKEY 1
#define MAXKEY 5

/* The main function starts off by initializing our two variables msgSize and keyValue
 * Once we initialize these values we can start to gather our messages from the user
 * The infinite while loop will continue to run until the -2 value is read in; or
 * if a value is read in that does not meet the specifications.
 *
 * inputs none
 *
 * outputs the exit code; good exit is 0 bad is 1
 */
int main() {
    int isPrime(int number);
    void sendMessage(int msgSize, int key, int arr[]);
    char line[MAXLINE];
    int msgSize;
    int keyValue;
    int message[MAXLINE];
    int counter = 1; // Used to determine what message # we're on

    for (int j = 0; j < 2; j++) {
        if (j == 0) {
            printf(">> Please enter a message size: ");
            fgets(line, MAXLINE, stdin);
            sscanf(line, "%d", &msgSize);
            msgSize = line[0]-'0';  // Convert from ASCII character to integer
            if (msgSize < MINSIZE || msgSize >= MAXSIZE) { // Check to see if the msgSize is not between 6 and 12
                printf("\n>> The message size is not valid...\n");
                exit(1);
            }
        }
        else {
            printf("\n>> Please enter a key value: ");
            fgets(line, MAXLINE, stdin);
            sscanf(line, "%d", &keyValue);
            keyValue = line[0]-'0'; // Convert from ASCII character to integer
            if (keyValue <= MINKEY || keyValue >= MAXKEY) { // Check to see if the keyValue is not between 2 and 4
                printf("\n>> The key value is not valid...\n");
                exit(1);
            }
        }
    }
    while (1) {
        printf("\n>> Please enter message number %d: \n", counter);
        for (int e = 0; e < msgSize; e++){ // Start to fill up our integers array with msgSize amount of values
            char integers[MAXLINE];
            fgets(integers,MAXLINE,stdin);
            sscanf(integers,"%d",message+e);
            if (message[e] == -2) { // As soon as we see -2 we exit
                printf("\n>> Agent Center is not safe now. Go find a safe place. Program exit.\n\n\n");
                exit(1);
            }
            if(message[e] == 0 || feof(stdin)) { // Make sure we don't keep getting values after the end
                printf("\n>> Failed to gather valid input... Program exit.\n");
                exit(1);
            }
        }
        sendMessage(msgSize, keyValue, message);
        counter++;
    }
    exit(0);

}

/* The isPrime function determines if the input value is a prime number
 *
 * inputs a single positive integer value
 *
 * outputs a 1 for true it is a prime number or 0 for false it is not a prime number
 */
int isPrime(int number) {

    if (number <= 1) return 0; // 0 implies non-prime
    if (number % 2 == 0 && number > 2) return 0;
    for(int i = 3; i < number / 2; i+= 2) {
        if (number % i == 0)
            return 0;
    }
    return 1; // 1 implies prime number
}

/* The sendMessage function is what determines what message should
 * be displayed based on the amount of primes values that we decoded
 *
 * inputs an integer msgSize so we can loop through exactly how many
 * values we have in our array. The integer key which is used to check
 * our prime numbers. And lastly the message array which is also used for
 * the prime number array.
 *
 * output void
 */
void sendMessage (int msgSize, int key, int message[]) {
    int numOfPrimes = 0; // Make sure that we don't keep any old values
    int id = 0; // This is for when we have a case 2 or 4 and we need the agent's id (the last value in the message)
    for (int q = 0; q < msgSize; q++) {
        if(message[q] <= MINNUM || message[q] >= MAXNUM) { // Ensure that our message is the correct size (not < our msgSize and not > our msgSize)
            numOfPrimes = MAXNUM; // Another way of setting off a flag for the switch
        }
        if (isPrime(message[q]-key) && numOfPrimes != MAXNUM) // Our prime number counter
            numOfPrimes++;
    }
    if (numOfPrimes == 2 || numOfPrimes == 4) { // If we need the agent ID we get it here
        id = message[msgSize-1];
    }
    switch (numOfPrimes) {
        case 1:
            printf(">> Msg:  I'm safe, all good.\n");
            break;
        case 2:
            printf(">> Msg:  Mission Success. Agent ID: %d.\n", id);
            break;
        case 3:
            printf(">> Msg:  Don't contact me.\n");
            break;
        case 4:
            printf(">> Msg:  Mission Failed. Agent ID: %d.\n", id);
            break;
        case 5:
            printf(">> Msg:  No Content.    \n");
            break;
        case 100:
            printf(">> Corrupted Message.\n");
            break;
        default:
            break;
    }
}