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
    memset(buffer, 0, strlen(buffer));
    memset(all_letters, 0, strlen(wrong_word));

    printf("Tests for letter.c:\n");

    printf("Test with a wrong letter: \n");
    printf("Result for letter %c in %s: %d\n", letter, wrong_word, letter_in_word(wrong_word, letter));
    printf("Result for letter %c in %s: %d\n", letter, right_word, letter_in_word(right_word, letter));

    nb_letters_return = all_letters_in_word(wrong_word, all_letters);

    printf("Result for all_letters_in_word with %s: %s\n", wrong_word, all_letters);
    printf("Return of all_letters_in_word: %d\n", nb_letters_return);

    printf("Test for upper_letter: capital letter (Z)\n: %c", upper_letter(capital_letter));
    printf("Test for upper_letter: letter (i): %c\n", upper_letter(letter));
    printf("Test for upper_letter: ({): %c\n", upper_letter(173));


    extract_word(buffer, "\0test\0\0");
    buffer[4] = '\0';
    printf("Test for extract_word (test): %s", buffer);

    //memset(bufferint,0,40);
    position_letter(wrong_word,bufferint, 'o');


    printf("Test for position_letter: %s\n", wrong_word);
        for (int i = 0; i<= (10); i++){
            printf("\t %d", bufferint[i]);
        }

    return 0;
}
/*
Compilation
    gcc -o tests_letter.run script_test_letter.c server_functions/src/letter.c -I server_functions
Running
    ./tests_letter.run
*/