/* lab3.c
 * XXX  <<-- you must change to your name
 * XXX  <<-- you must change to your name
 * ECE 2220, Fall 2020 
 * MP3
 * Subject: ECE2220,MP3
 *
 * Purpose:  A template for MP3 -- you must replace this placeholder
 *
 * Assumptions:
 *  #1   The user is prompted to enter a pseudo arithmetic command.  The
 *       input must be verified to be grammatically correct.
 *
 *  #2:  The string and character type library cannot be used under
 *       any circumstances.  You are encouraged to develop your own
 *       functions to perform any similar operations that are needed.
 *
 *  #3   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs:
 *
 * Notes:
 *
 * See the ECE 2220 programming guide
 *
 * If your formatting is not consistent you must fix it.  You can easily
 * reformat (and automatically indent) your code using the astyle
 * command.  In a terminal on the command line do
 *
 *     astyle --style=kr lab3.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up
 * your own style.
 */

// do not include any additional libraries
#include <stdio.h>

// do not change these constants
#define MAXLINE 80
#define MAXOPER 13

// named constants and strings
enum operations {NOOP, ADD, MUL, DIV, POW};
const char *operation_str[] = {"Invalid", "+", "*", "/", "^"};

// function prototypes
int process_input(const char *input, char *op_left, char *op_right);
void calc_output(const char *op_l, int op, const char *op_r, char *result);

// do not change any code in main.  We are grading based on the format
// of the output as given in the printfs in main.
int main()
{
    char input_line[MAXLINE];
    char left_operand[MAXOPER];
    char right_operand[MAXOPER];
    char answer[MAXOPER];
    int  operator;

    printf("\nMP3: Arithmetic on GF(47) with + * / ^ using letters\n");
    printf("Commands:\n\tabc+bbc\n\tturtle/frog\n\ttiger^one");
    printf("\tOperands are no more than 12 letters and no spaces\n");
    printf("\tCtrl-d to quit\n\n");
    printf("> ");

    // each call to fgets collects one line of input and stores in input_line
    // BEWARE: fgets includes the end-of-line character '\n' in input_line
    while (fgets(input_line, sizeof input_line, stdin) != NULL) {

        // clear for next round
        left_operand[0] = right_operand[0] = answer[0] = '\0';

        // check for valid grammar
        operator = process_input(input_line, left_operand, right_operand);

        if (operator == ADD || operator == MUL
                || operator == DIV || operator == POW) {

            // print parsed input
            printf("'%s'", left_operand);
            printf(" %s ", operation_str[operator]);
            printf("'%s' => ", right_operand);

            // perform pseudo arithmetic
            calc_output(left_operand, operator, right_operand, answer);

            // print result
            printf("'%s'\n\n", answer);
        } else {
            printf("# %s", input_line);
        }
        printf("> ");
    }
    printf("\nGoodbye\n");
    return 0;
}

/* Parse input of the form SOS where S is a string and O is a character.
 *
 * A string S must consist of up to 12 valid symbols a-z and A-U.
 * The operand O must be one character from: + * / ^
 * Any other characters found in the input, including spaces, are
 * grammatically incorrect and invalidate the input.
 *
 * There must be no spaces anywhere in the input, including between
 * either SO, OS, or leading or trailing spaces.
 *
 * Input: The input string is collected using fgets.  Recall the end-of-line
 *        character is included in the input string and marks the end of
 *        the input.  This string must not be changed.
 *
 * Output: There are three outputs from this function.
 *
 *   The return value is one of NOOP, ADD, MUL, DIV, POW which are
 *      named constants.  If the input is invalid for any reason
 *      then the output must be NOOP.  Otherwise the return value
 *      corresponds to operand O.
 *
 *   If the input is grammatically correct, then two strings are also
 *   returned, one for each of the left and right operands.  If the input
 *   in invalid the two output strings are undefined.
 */
int process_input(const char *input, char *op_left, char *op_right)
{
    // replace the following with your code
    op_left[0] = 'A';
    op_left[1] = '\0';
    op_right[0] = 'A';
    op_right[1] = '\0';
    return ADD;
}

/* Pseudo mathematical opertions on the two operands work as follows.
 *
 * Each character is converted to an integer in the range 1...46, where a is 0,
 * b is 1, c is 2, ..., z is 25.  The operation is then performed using
 * math on a finite field with no carries.
 *
 * If the two input strings are not the same length, then each output character
 * beyond the length of the shorter string should be a copy of the character
 * from the longer string but with the opposite case.
 *
 * Input: The two operand strings and the operator are assumed to be valid (and
 *        are verified as valid in the parse_input function).
 *
 * Output: The final string generated by the above rules is stored in the
 *         output string named result.  The input strings must not be
 *         changed.
 */
void calc_output(const char *l_op, int op, const char *r_op, char *result)
{
    // replace the following with your code
    result[0] = '!';
    result[1] = '\0';
}
