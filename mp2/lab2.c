/* lab2.c
 * Your Name    <<-- you must change to your name and id
 * Clemson ID
 * ECE 222, Fall 2020
 * MP2 (2020)
 *
 * Purpose:  A template for MP2 -- replace
 *
 * Assumptions:
 *  #1:  The menu driven input was provided and must be used exactly
 *       as written.  A user can enter commands:
 *            enc CUt
 *            dec 0E8A549C
 *            quit
 *       Encoding takes three printable ASCII letters
 *       Decoding takes up to eight HEX digits. If exactly eight digits are
 *            entered, the first digit must be 0 or 1.
 *            Leading zeros can be dropped.
 *
 *  #2:  The string and character type libraries cannot be used except as
 *       already provided.  These libraries are for checking inputs in main
 *       and in printing after decoding is complete.  They cannot be used
 *       for anyother purpose.
 *
 *  #3:  No arrays can be used (excpet as already provided for collecting
 *       keyboard input).  You must use bitwise operators for all encoding
 *       and decoding.  If you want to use an array as a lookup table you
 *       must first propose your design and get it approved.  Designs that
 *       use tables to avoid bitwise operators will not be approved.  There
 *       are many good and simple designs that do not require tables.
 *
 *  #4   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs:
 *
 * See the ECE 222 programming guide
 *
 * If your formatting is not consistent you must fix it.  You can easily
 * reformat (and automatically indent) your code using the astyle
 * command.  If it is not installed use the Ubuntu Software Center to
 * install astyle.  Then in a terminal on the command line do
 *
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up
 * your own style.
 *
 * To create a nicely formated PDF file for printing install the enscript
 * command.  To create a PDF for "file.c" in landscape with 2 columns do:
 *     enscript file.c -G2rE -o - | ps2pdf - file.pdf
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 100

// function prototypes
void encode(unsigned char first_letter, unsigned char second_letter,
        unsigned char third_letter);
void decode(unsigned int codeword);

int main()
{
    char line[MAXLINE];
    char command[MAXLINE];
    char inputcs[MAXLINE];
    int  items;
    int i, invalid;
    unsigned int codeword;

    printf("\nMP2: encoding and decoding (29, 24) Hamming code.\n");
    printf("Commands:\n\tenc 3-letters\n\tdec 8-hex-digits\n\tquit\n");

    // each call to fgets, collects one line of input and stores in line
    while (fgets(line, MAXLINE, stdin) != NULL) {
        items = sscanf(line, "%s%s", command, inputcs);
        if (items == 1 && strcmp(command, "quit") == 0) {
            break;
        } else if (items == 2 && strcmp(command, "enc") == 0) {
            // encoding
            if (strlen(inputcs) != 3 || !isprint(inputcs[0]) ||
                    !isprint(inputcs[1]) || !isprint(inputcs[2])) {
                printf("Invalid input to encoder: %s\n", inputcs);
            } else {
                encode(inputcs[0], inputcs[1], inputcs[2]);
            }
        } else if (items == 2 && strcmp(command, "dec") == 0) {
            // decoding: convert hex digits to integer
            items = sscanf(inputcs, "%x", &codeword);
            if (items != 1 || strlen(inputcs) > 8) {
                printf("Invalid input to decoder: %s\n", inputcs);
            } else {
                // verify all digits are hex characters because
                // scanf does not reject invalid letters
                for (i=0, invalid=0; i < strlen(inputcs) && !invalid; i++) {
                    if (!isxdigit(inputcs[i]))
                        invalid = 1;
                }
                // if 8 digits, leading digit must be 1 or 0
                if (invalid) {
                    printf("Invalid decoder digits: %s\n", inputcs);
                } else if (strlen(inputcs) == 8 && inputcs[0] != '1'
                        && inputcs[0] != '0') {
                    printf("Invalid decoder leading digit: %s\n", inputcs);
                } else {
                    decode(codeword);
                }
            }
        } else {
            printf("# :%s", line);
        }
    }
    printf("Goodbye\n");
    return 0;
}

/* encode: calculates parity bits and prints codeword
 *
 * input: three ASCII characters
 * assumptions: input is valid
 *
 * Example: if input letters are is 'C', 'U', and 't'
 * the final print must be:
 *  ---01110 10001010 01010100 10011100
 *  Codeword: 0x0E8A549C
 */
void encode(unsigned char first_letter, unsigned char second_letter,
        unsigned char third_letter)
{
    // you must construct the codeword
    unsigned int codeword = 0;
    int i = -1;  // dummy variable

    printf("%9s%9c%9c%9c\n", "Encoding:", third_letter, second_letter, first_letter);
    printf(" 0x    00%9x%9x%9x\n", third_letter, second_letter, first_letter);

    //
    // ADD your code here
    //

    // print the information word in binary form with spaces

    // print the parity bits, one bit per line.  Do not change
    // the format, but you must change the dummy variable i
    // to match your design
    printf("P1 : %d\n", i);
    printf("P2 : %d\n", i);
    printf("P4 : %d\n", i);
    printf("P8 : %d\n", i);
    printf("P16: %d\n", i);

    // print the codeword bits in binary form with spaces

    // print the codeword in hex format
    printf(" Codeword: 0x%.8X\n", codeword);
    printf("\n");
}

/* decode: checks parity bits and prints information characters
 *
 * input: A 29-bit codeword
 * assumptions: the codeword has either no or only one error.
 *
 *              the information characters may not be printable
 *
 * FYI: when a codeword has more than one error the decoding algorithm
 * may generate incorrect information bits.  In a practical system
 * inforamtion is grouped into multiple codewords and a final CRC
 * code verifies if all codewords are correct.  We will not
 * implement all of the details of the system in this project.
 *
 * Example: if the codeword is 0x0E8A549C
 * the final print must be:
 *  No error
 *  -------- 01110100 01010101 01000011
 *  Information Word: 0x745543 (CUt)
 *
 * Example with one error in codeword bit 21: 0x0E9A549C
 * Notice the 8 in the previous example has been changed to a 9
 * the final print must be:
 *  Corrected bit: 21
 *  -------- 01110100 01010101 01000011
 *  Information Word: 0x745543 (CUt)
 */
void decode(unsigned int codeword)
{
    // you must determine these values:
    unsigned int info_word = 0;
    unsigned char first_letter = 0x42;
    unsigned char second_letter = 0x61;
    unsigned char third_letter = 0x64;
    int bit_error_location = 21;

    // dummy variable
    int i = -1;

    printf("Decoding: 0x%.8X\n", codeword);

    //
    // ADD your code here to fix a bit error if one exists
    // and determine the three characters
    //

    // print the error location bits, one bit per line.  Do not change
    // the format, but you must change the dummy variable i
    // to match your design
    printf("E1 : %d\n", i);
    printf("E2 : %d\n", i);
    printf("E4 : %d\n", i);
    printf("E8 : %d\n", i);
    printf("E16: %d\n", i);

    // here is the required format for the prints.  Do not
    // change the format but update the variables to match
    // your design
    if (bit_error_location == 0)
        printf(" No error\n");
    else if (bit_error_location > 0 && bit_error_location <= 29) {
        printf(" Corrected bit: %d\n", bit_error_location);
    } else
        printf(" Decoding failure: %d\n", bit_error_location);

    // you must print the info_word in binary format

    // print the information word in hex:
    printf(" Information Word: 0x%.6X", info_word);

    // You must convert the info_word into three characters for printing
    // only print information word as letters if 7-bit printable ASCII
    // otherwise print a space for non-printable information bits
    if ((first_letter & 0x80) == 0 && isprint(first_letter))
        printf(" (%c", first_letter);
    else
        printf(" ( ");
    if ((second_letter & 0x80) == 0 && isprint(second_letter))
        printf("%c", second_letter);
    else
        printf(" ");
    if ((third_letter & 0x80) == 0 && isprint(third_letter))
        printf("%c)\n", third_letter);
    else
        printf(" )\n");
    printf("\n");
}
