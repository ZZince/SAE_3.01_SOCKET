//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////																			//////
/////		Author : Fourny Nathan												//////
/////		Date : 20/11/2023													//////
/////		Subject : Contains the game's logical functions   					//////
/////																			//////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/* Utils to make the game work properly. */

////////////////////////////// IMPORT ////////////////////////////////////////////////

// Library
#include <stdio.h>
#include <stdlib.h> // exit()
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>	// memset()
#include <netinet/in.h>	// struct sockaddr_in
#include <arpa/inet.h> // htons() and inet_aton() 

#define MAX_WORD_SIZE 256 // = Maximum lenght of a word

// Depedencies
#include "../client_functions.h"

////////////////////////////// FUNCTIONS ////////////////////////////////////////////////

/* Set a -1 where at the maximum lenght of the word
    Parameters:
        number_of_letter [int]: The number of letter in the word
        *word [char]: The word the player have to find
*/
int set_word_lenght(int number_of_letter, char *word) {
    if (number_of_letter >= 0 && number_of_letter < MAX_WORD_SIZE) {
        word[number_of_letter] = -1; // Set -1 at the end of the word
    } else {
        printf("Nombre de lettres invalide.\n");
        return -1;
    }
    return 0;
}

/* Show the word that the player have to find on the terminal
    Parameters:
        number_of_letter [int]: The number of letter in the word
        *word [char]: The word the player have to find
*/
void show_word_in_cmd(int number_of_letter, char *word) {
    char character;
    printf("\nVotre mot :\n");
    for (int i = 0; i < number_of_letter; i++) {
        if (!word[i]) { // When it's 0, letter is not found yet
            printf(" _");
        } else { // Show letter
            character = word[i]; 
            printf(" %c", character);
        }
    }
    printf("\n\n");
}