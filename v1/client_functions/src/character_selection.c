//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////																			//////
/////		Author : Fourny Nathan												//////
/////		Date : 16/11/2023													//////
/////		Subject : Character selection function								//////
/////																			//////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/* Utils to character selection for the main client program. */

////////////////////////////// IMPORT ////////////////////////////////////////////////


// Library
# include <stdio.h>
# include <stdlib.h>  // get malloc()
# include <stdbool.h> // get bool type
# include <ctype.h>   // get isAlpha()
# include <string.h>  // get strlen()

// Depedencies
#include "../client_functions.h"

// Constant
#define MAX_WORD_SIZE 256 // Maximum lenght of the word


////////////////////////////// FUNCTIONS ////////////////////////////////////////////////


/* Made the client select a letter
    Return:
        - [char]: The letter choosed by the client 
*/
char select_letter() {

    char character;

    // Clear the input buffer to prevent any remaining characters
    while (getchar() != '\n');

    choice:
        printf("Entrez une lettre :\n");
        scanf("%c", &character);
        
        // FIX - Repetition ici
        printf("%c\n", character);
        if (!is_not_letter(character)){
            goto choice;
        }

    return toupper(character); // Return the character in upper format
}


char *select_word() {
    char *word = (char *)malloc(MAX_WORD_SIZE * sizeof(char));;

    selection:
        printf("Entrez un mot (max %d caractères) :\n", MAX_WORD_SIZE);

        if (scanf("%s", word) == 1){
            // Delete the \n if there is one
            size_t len = strlen(word);
            if (len > 0 && word[len - 1] == '\n') {
                word[len - 1] = '\0';
            }
        }

        // Check if there is invalid character
        for (size_t i = 0; i < strlen(word); ++i) {
            if (!isalpha(word[i])) {
                printf("Le mot contient des caractères non valides. Veuillez entrer un mot valide.\n");
                goto selection;
            }
        }

        // Set the word in uppercase
        for (size_t i = 0; i < sizeof(word); ++i) {
        word[i] = toupper(word[i]);
        }

    return word;
}



/* Check if the character is a letter
    Return:
        - [char]: The checked character
*/
bool is_not_letter(char character) {
    return isalpha(character);
}

