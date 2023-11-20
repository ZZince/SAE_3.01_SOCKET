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
# include <stdbool.h> // get bool type
# include <ctype.h>   // get isAlpha()

// Depedencies
#include "../client_functions.h"


////////////////////////////// FUNCTIONS ////////////////////////////////////////////////


/* Made the client select a letter
    Return:
        - [char]: The letter choosed by the client 
*/
char select_letter() {

    char character;

    choice:
        printf("Entrez une lettre :\n");
        character = getchar();
        // Clear the input buffer to prevent any remaining characters
        while (getchar() != '\n');

        if (!is_letter(character)){
            goto choice;
        }

    return toupper(character); // Return the character in upper format
}



/* Check if the character is a letter
    Return:
        - [char]: The checked character
*/
bool is_letter(char character) {
    return isalpha(character);
}

