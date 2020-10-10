/* lab3.c
 * Aaron Bruner
 * ajbrune
 * ECE 2220, Fall 2020
 * MP3
 * Subject: ECE2220,MP3
 *
 * Purpose:  The purpose of MP3 is to take two ASCII inputs and apply the provided operation to each individual character and return the result in ASCII characters.
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
enum operations { NOOP, ADD, MUL, DIV, POW};
const char *operation_str[] = {"Invalid", "+", "*", "/", "^"};

// function prototypes
int process_input(const char *input, char *op_left, char *op_right);
void calc_output(const char *op_l, int op, const char *op_r, char *result);
int stringLen(const char *op);
int charInvert(int letter);
char resultConversion(char result);

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
int process_input(const char *input, char *op_left, char *op_right) {

    int operator, inputSize;
    int i = 0, k =0;

    // We want to check MAXLINE-2 because we cannot have more than 78 characters. Then, we would not have room for the operator and null character
    inputSize = stringLen(input);
    if (inputSize > MAXLINE-2)
        return NOOP;

    //read the rest of the left side
    while (input[i] != 0) {
        if ((input[i] > 96 && input[i] < 123) || (input[i] > 64 && input[i] != 94 && input[i] < 86)) {
            op_left[i] = input[i];
            if (i > MAXOPER-2)
                return NOOP;
        }
        //check for operator
        else if(input[i] == 43 || input[i] == 42 || input[i] == 47 || input[i] == 94) {
            operator = input[i];
            op_left[i] = 0;
            break;
        }
        else
            return NOOP;
        i++;

    }

    //read the rest of the right side after the operator is found
    while (input[i+1] != 0) {
        if ((input[i+1] > 96 && input[i+1] < 123) || (input[i+1] > 64 && input[i+1] < 86)) {
            op_right[k] = input[i+1];
            k++;
            if (k > MAXOPER-1)
                return NOOP;
        }
        else if(input[i+1] == 10 || input[i+1] == 0) {
            op_right[k] = 0;
            break;
        }
        else
            return NOOP;
        i++;
    }

    switch (operator) {
        case 43:
            return ADD;
            break;
        case 42:
            return MUL;
            break;
        case 47:
            return DIV;
            break;
        case 94:
            return POW;
            break;
        default:
            return NOOP;
            break;
    }
    return NOOP;
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
    int x = 0, w, temp, remainder, i = 0, j = 0;
    int rhsLength, lhsLength;
    char left_op[MAXOPER]; //translated left side
    char right_op[MAXOPER]; //translated right side
    int z, y; //Z & Y value for inversion
    int stringDiff; //check to see if the strings are different

    //translate letters
    while (l_op[i] != 0) {
        if (l_op[i] < 86) {
            left_op[i] = l_op[i] - 39;
            i++;
        }
        else {
            left_op[i] = l_op[i] - 97;
            i++;
        }
    }
    left_op[i] = 0; // set last character to null character

    while (r_op[j] != 0) {
        if (r_op[j] < 86) {
            right_op[j] = r_op[j] - 39;
            j++;
        }
        else {
            right_op[j] = r_op[j] - 97;
            j++;
        }
    }
    right_op[j] = 0; // set last character to null character

    //Calculate string lengths
    rhsLength = stringLen(r_op);
    lhsLength = stringLen(l_op);

    //check to see if rhs > lhs || rhs < lhs || or rhs == lhs
    if (rhsLength > lhsLength) {
        x+=0;
        remainder = rhsLength - lhsLength;
        stringDiff = rhsLength;
    } else if (rhsLength < lhsLength) {
        x+=1;
        remainder = lhsLength - rhsLength;
        stringDiff = lhsLength;
    } else {
        x+=2;
        stringDiff = lhsLength;
    }

    if (op == 1) { // op 1 is ADD
        switch (x) {
            case 2:// same size strings
                for (w = 0; w < rhsLength; w++)
                    result[w] = (left_op[w] + right_op[w]) % 47;
                result[rhsLength] = 0;
                break;
            case 1: // rhs < lhs
                for (w = 0; w < rhsLength; w++)
                    result[w] = (left_op[w] + right_op[w]) % 47;
                for (w = 0; w <= remainder; w++)
                    result[rhsLength + w] = charInvert(left_op[rhsLength + w]);
                result[lhsLength] = 0;
                break;
            case 0: // rhs > lhs
                for (w = 0; w < lhsLength; w++)
                    result[w] = (left_op[w] + right_op[w]) % 47;
                for (w = 0; w <= remainder; w++)
                    result[lhsLength + w] = charInvert(right_op[lhsLength + w]);
                result[rhsLength] = 0;
                break;
        }
        //convert result
        for(i = 0; i < stringDiff; i++)
            result[i] = resultConversion(result[i]);
        result[i+1] = 0;
    }

    if (op == 2) { // op 2 is MUL
        switch (x) {
            case 2:// same size strings
                for (w = 0; w < rhsLength; w++)
                    result[w] = (left_op[w] * right_op[w]) % 47;
                result[rhsLength] = 0;
                break;
            case 1: // rhs < lhs
                for (w = 0; w < rhsLength; w++)
                    result[w] = (left_op[w] * right_op[w]) % 47;
                for (w = 0; w <= remainder; w++)
                    result[rhsLength+w] = charInvert(left_op[rhsLength+w]);
                result[lhsLength] = 0;
                break;
            case 0: // rhs > lhs
                for (w = 0; w < lhsLength; w++)
                    result[w] = (left_op[w] * right_op[w]) % 47;
                for (w = 0; w <= remainder; w++)
                    result[lhsLength+w] = charInvert(right_op[lhsLength+w]);
                result[rhsLength] = 0;
                break;
        }
        //convert result
        for(i = 0; i < stringDiff; i++)
            result[i] = resultConversion(result[i]);
        result[i+1] = 0;
    }

    if (op == 3) { // op 3 is inversion
        switch (x) {
            case 2:// same size strings
                for (w = 0; w < rhsLength; w++) {
                    if (right_op[w] == 0)
                        result[w] = 0;
                    for (z = 0; z < 47; z++)
                        if ((right_op[w] * z) % 47 == left_op[w])
                            result[w] = z;
                }
                result[rhsLength] = 0;
                break;
            case 1: // rhs < lhs
                for (w = 0; w < rhsLength; w++) {
                    if (right_op[w] == 0)
                        result[w] = 0;
                    for (z = 0; z < 47; z++)
                        if ((right_op[w] * z) % 47 == left_op[w])
                            result[w] = z;
                }
                for (w = 0; w <= remainder; w++)
                    result[rhsLength+w] = charInvert(left_op[rhsLength+w]);
                result[lhsLength] = 0;
                break;
            case 0: // rhs > lhs
                for (w = 0; w < lhsLength; w++) {
                    if (right_op[w] == 0)
                        result[w] = 0;
                    for (z = 0; z < 47; z++)
                        if ((right_op[w] * z) % 47 == left_op[w])
                            result[w] = z;
                }
                for (w = 0; w <= remainder; w++)
                    result[lhsLength+w] = charInvert(right_op[lhsLength+w]);
                result[rhsLength] = 0;
                break;
        }
        //convert result
        for(i = 0; i < stringDiff; i++)
            result[i] = resultConversion(result[i]);
        result[i+1] = 0;
    }

    if (op == 4) { // op 4 is DIV
        switch(x) {
            case 2:// same size strings
                for (w = 0; w < rhsLength; w++) {
                    temp = 1;
                    for (y = 0; y < right_op[w]; y++) {
                        temp *= left_op[w];
                        temp = temp % 47;
                    }
                    result[w] =  temp % 47;
                    if (right_op[w] == 1)
                        result[w] = left_op[w];
                    if (right_op[w] == 0)
                        result[w] = 1;
                }
                result[rhsLength] = 0;
                break;
            case 1:
                for (w = 0; w < rhsLength; w++) {
                    temp = 1;
                    for (y = 0; y < right_op[w]; y++) {
                        temp *= left_op[w];
                        temp = temp % 47;
                    }
                    result[w] =  temp % 47;
                    if (right_op[w] == 1)
                        result[w] = left_op[w];
                    if (right_op[w] == 0)
                        result[w] = 1;
                }
                for (w = 0; w <= remainder; w++)
                    result[rhsLength+w] = charInvert(left_op[rhsLength+w]);
                result[lhsLength] = 0;
                break;
            case 0: // rhs > lhs
                for (w = 0; w < rhsLength; w++) {
                    temp = 1;
                    for (y = 0; y < right_op[w]; y++) {
                        temp *= left_op[w];
                        temp = temp % 47;
                    }
                    result[w] =  temp % 47;
                    if (right_op[w] == 1)
                        result[w] = left_op[w];
                    if (right_op[w] == 0)
                        result[w] = 1;
                }
                for (w = 0; w <= remainder; w++)
                    result[lhsLength+w] = charInvert(right_op[lhsLength+w]);
                result[rhsLength] = 0;
        }
        //convert result
        for(i = 0; i < stringDiff; i++)
            result[i] = resultConversion(result[i]);
        result[i+1] = 0;
    }
}

/* The stringLen function determines how long a string is. This is pretty much strlen function from string.h
 *
 * inputs the pointer to a character array
 *
 * outputs an integer value with the length of the string
 */
int stringLen(const char *op) {
    int len = 0;
    int i;
    for(i=0; op[i] != 0; i++) {
        len++;
    }
    return (len);
}

/* The charInvert function converts converts the case of an ASCII character. So, lowercase to uppercase or uppercase to lowercase
 *
 * inputs a single integer value (an ASCII character but in int form)
 *
 * outputs a single integer value (an ASCII character but in int form)
 */
int charInvert(int letter) {

    if (letter >= 0 && letter <= 25)
        letter = letter + 26;
    else if (letter >= 25 && letter < 47)
        letter = letter - 26;
    else
        return letter;

    return letter;
}

/* The resultConversion function converts the character decimal value into ASCII characters for printing the answer
 *
 * inputs a single character
 *
 * outputs a single character
 */
char resultConversion(char result) {

    if (result < 26 && result >= 0)
        result = result + 97;
    else if (result > 25 && result < 52)
        result = result + 39;
    else
        result = result;

    return result;
}