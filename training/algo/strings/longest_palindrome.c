#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 *  Finds the length of a palindrome inside a string starting from its middle.
 *  Stops until the beginning of the string is reached (left_bound) on the left
 *  or until the end of the string ('0\').
 *  Left is a pointer to the char immediately on the left of the middle char.
 *  Right is a pointer to the char immediately on the right of the middle chars.
 *  If the palindrome has an even number of chars, the middle is not a char.
 *
 *  For example:
 *                  abba        radar
 *                   ^^          ^ ^
 *                   LR          L R
 *
 *  Runs in O(len(palindrome)/2), worst O(len(string)/2) if string is palindrome.
 */
int len_palindrome(char* left, char* right, char* left_bound) {
    int len = 0;
    while( *right != '\0' ) {
        if ( *left != *right )  /* left and right chars are different */
            break;
        len += 2;               /*    otherwise we found 2 new matching chars */
        
        if (left == left_bound) /* we were checking the left bound -> stop here */
            break;
        left--;                 /*    otherwise update pointers */
        right++;
    };
    return len;
}


/*
 *  Find the longest palindrome inside the chain of characters char* string.
 *  Ignores single character palindromes (length 1).
 *
 *  The string is looked character by character starting from the beginning
 *  until the end.
 *
 *  We look for palindromes starting from their middle:
 *      if current char = next char -> beginning of even length palindrome
 *      if previous char = next char -> beginning of odd length palindrome
 *
 *  we then call len_palindrome() to check the length and store its length and
 *  start point if it is longer than the longest palindrome we have found so far.
 *
 *  Returns the length to the caller. A pointer to the beginning is written at
 *  the memory location given by the caller. If no palindrome was found, returns
 *  -1. In this case the memory location is set to NULL.
 *
 *  Expected run time is proportional to N the length of the string plus
 *  a constant C times the sum of the length of all palindromes found in the
 *  string.
 *
 *      O( N + C sum_i len(palindrome[i]) ) 
 *
 *  Average times in N depends on the nature of the text and should be close to
 *  O(N) for normal readable text.
 *
 */
int longest_palindrome(char* string, char** palindrome_start) {
    int longest_len = -1;
    *palindrome_start = NULL;

    if (!string) return -1;     /* guard against null input */

    char* index = string;       /* pointers used for iterating over the string */

    char  current_char = *string;
    char  prev_char = '\0';

    while( *index != '\0') {
        char next_char = *(index+1);
        if ( current_char == next_char ) {     /* middle of even len palindrome */
            int len = len_palindrome(index, index + 1, string);
            if ( len > longest_len ) {
                longest_len = len;
                *palindrome_start = index - (len-1)/2;
            }
        }
        if ( prev_char == next_char) {   /* middle of oddlen palindrome */
            int len = 1 + len_palindrome(index - 1, index + 1, string);
            if ( len > longest_len ) {
                longest_len = len;
                *palindrome_start = index - len/2;
            }
        }

        /*  We could add here a shortcut to stop the loop if there is less 
         *  remaining chars in the string than half the length of the longest
         *  palindrome we have found so far.
         */

        prev_char = current_char;
        current_char = next_char;
        index++;
    }

    return longest_len;
}


int main(void) {

    char *text = "abcababtbatabtcddc";

    char *palindrome_start;

    int len = longest_palindrome(text, &palindrome_start);

    if (len > 0) {

        char* palindrome = strndup(palindrome_start, len);

        if (palindrome) {
            printf("found palindrome of length %i:\n", len); /* state length */

            printf("%s\n", text);                            /* print string */

            while (palindrome_start != text) {               /* print offsets " " */
                printf(" ");
                palindrome_start--;
            }
            printf("%s\n", palindrome);                      /* print palindrome */
            free(palindrome);
        }

    }

    exit(0);
}
