#include <stdio.h>
#include <string.h>
#include "server_functions/letter.h"

/*Some tests of letter.c functions*/
int main(){
    char* wrong_word = "wrong-word";
    char* right_word = "right_word";
    char all_letters[strlen(wrong_word)];
    char letter = 'i';
    char capital_letter = 'Z';
    int nb_letters_return;
    char buffer[10];
    int* positions;
    int bufferint[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char* only_zero = "\0\0\0";
    char* not_only_zero = "\0i0d";
    int len_not_only_zero = 3;

    memset(buffer, 0, strlen(buffer));
    memset(all_letters, 0, strlen(wrong_word));

    printf("Tests for letter.c:\n");

    printf("Test with a wrong letter: \n");
    printf("Result for letter %c in %s: %d\n", letter, wrong_word, letter_in_word(wrong_word, letter, strlen(wrong_word)));
    printf("Result for letter %c in %s: %d\n", letter, right_word, letter_in_word(right_word, letter, strlen(wrong_word)));

    nb_letters_return = all_letters_in_word(wrong_word, all_letters);

    printf("Result for all_letters_in_word with %s: %s\n", wrong_word, all_letters);
    printf("Return of all_letters_in_word: %d\n", nb_letters_return);

    printf("Test for upper_letter: capital letter (Z)\n: %c", upper_letter(capital_letter));
    printf("Test for upper_letter: letter (i): %c\n", upper_letter(letter));
    printf("Test for upper_letter: ({): %c\n", upper_letter(173));


    extract_word(buffer, "\0test\0\0");
    buffer[4] = '\0';
    printf("Test for extract_word (test): %s\n", buffer);

    position_letter(wrong_word, bufferint, 'o', strlen(wrong_word));


    printf("Test for position_letter: %s\n", wrong_word);
        for (int i = 0; i<= (10); i++){
            printf("\t %d", bufferint[i]);
        }

    printf("\n");
    printf("Test for verif_only_zero: %d\n", verif_only_zero(only_zero, strlen(only_zero)));
    printf("Test for verif_only_zero: %d\n", verif_only_zero(not_only_zero, len_not_only_zero));
}
/*
Compilation
    gcc -o tests_letter.run script_test_letter.c server_functions/src/letter.c -I server_functions
Running
    ./tests_letter.run
*/