//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////																			//////
/////		Author : Fourny Nathan												//////
/////		Date : 17/11/2023													//////
/////		Subject : Send messages to the server   							//////
/////																			//////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/* Utils to send messages to a server. */

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
#include <stdbool.h> // type boolean


// Constants
#define CODE_NUMBER_OF_LETTER 201 // Means that the server sends the number of letters in the word
#define CODE_LETTER_RECEIVED 202 // Send letter validation code
#define LETTER_FOUND_IN_WORD 204 // A letter is found in the word validation code
#define LETTER_NOT_FOUND_IN_WORD 205 // A letter is not found in the word validation code
#define CODE_SEND_WORD 206 // Send word validation code
#define CODE_WORD_FOUND 207 // Word find validation code
#define CODE_WORD_NOT_FOUND 208 // Word not find validation code
#define CODE_PLAYER_LOST 209 // Player loose the game validation code
#define CODE_PLAYER_WON 2010 // Player win the game (he get all letters)
 


// Depedencies
#include "../client_functions.h"

////////////////////////////// FUNCTIONS ////////////////////////////////////////////////


/* Allow a client to send a character to the server
    Parameter:
        socket [int]:
        character [char]: The letter we send to the server
        size [int]: The maximum size (number of bytes) of the message 

    Return:
        - [int]: 0 if resolved, -1 if error
*/
int send_character_to_server(int socket, char character, int size) {
    unsigned char message[size]; // set in unsigned to allow values bigger than 127 in a bytes
    int nb;

    memset(message, 0x00, size); // Initialize message

    message[0] = CODE_LETTER_RECEIVED; // 202
    message[1] = character;

    

	// Show message in decimal  ////////////////////
    //printf("Message formé : ");
    //for (int i = 0; i < size; ++i) {
    //    printf("%d ", message[i]);
    //}
    //printf("\n");


	// Try sending the message
    switch(nb = write(socket, message, size)){
		case -1 : 
			perror("Erreur en écriture...");
			close(socket); 
			return -1; // ERROR
		case 0 : // Socket is closed
			fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			return 0;
		default: // message send successfuly
			printf("Lettre %c envoyée! (%d octets)\n\n", character, nb);
	}

    return 0;
}


int send_word_to_server(int socket, char *word, int size) {
    unsigned char message[size]; // set in unsigned to allow values bigger than 127 in a bytes
    int nb;

    memset(message, 0x00, size); // Initialize message

    message[0] = CODE_SEND_WORD;

    for (int index = 0; index < size; index++) {
        message[index + 1] = word[index];
    }

    // Try sending the message
    switch(nb = write(socket, message, size)){
		case -1 : 
			perror("Erreur en écriture...");
			close(socket); 
			return -1; // ERROR
		case 0 : // Socket is closed
			fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			return 0;
		default: // message send successfuly
			printf("Mot : %s envoyée! (%d octets)\n\n", word, nb);
	}

    return 0;
}


/* Allow a client to get a message from the server
    Parameters:
        socket [int]:
        size [int]: The maximum size (number of bytes) that we get

    Return:
        - [char *]: The message from the server

*/
char *get_message_from_server(int socket, int size) {
    unsigned char *message = (char *)malloc(size * sizeof(char)); // Allocate message memory
    memset(message, 0x00, size);

    if (message == NULL) {
        perror("Allocation de mémoire impossible");
        exit(-1); // ERROR
    }
    int nb;

    // Try getting the message
    switch(nb = read(socket, message, size)) {
		case -1 :
			perror("Erreur de lecture...");
			close(socket);
			exit(-1); // Error
		case 0 : // Socket is closed
   		    fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			return 0;
		default: // Get the message
		    message[nb]='\0';
            // Show message
            printf("Message reçu du serveur (%d octets) : ", nb);
            for(int i = 0; i < size; i++) {
                printf("%d ", message[i]);
            }
            printf("\n");
	}


    return message;
}

/* Translate the message the server send to the client. (Show word, correct or wrong letter, etc...)
    Parameters:
        *buffer [unsigned char]: the message from the server
        *word [char]: the word the client have to find
*/
int translate_message(unsigned char *buffer, char *word, char character) {
    static bool firstTime = true;
    int index = 0;
    int code = buffer[index];
    int value;
    int error;

    loop:
    switch(code){
        case CODE_NUMBER_OF_LETTER: // Number of letter in the word
            index = 1;
            value = buffer[index+1];
            if (!value) { // If value Index + 1 is 0 it means end of message
                if (firstTime) { // If it's the first time we get the word lenght
                    set_word_lenght(buffer[index], word); // Reallocate word's memory
                    firstTime = false;
                }   
                show_word_in_cmd(word); // Show message in the cmd with the number of letter (index)
                goto end; // Then go to end
            } else
                index++; 
                goto loop; // Then loop again

        case LETTER_FOUND_IN_WORD:  // Letter food, be add it in the word to show it in the cmd
            character_is_good(buffer, word, character);
            goto end;

        case LETTER_NOT_FOUND_IN_WORD:  // Letter not find so we show the hangman
            error = buffer[1];
            error = abs(error - 6);
            show_hangman(error);
            goto end;

        case CODE_WORD_FOUND: // Player find the word and win the game
            printf("Vous avez trouvé le bon mot\n");
            return 1;

        case CODE_WORD_NOT_FOUND: // Player find the wrong word, show the hangman
            printf("Le mot est incorrect\n");
            error = buffer[1];
            error = abs(error - 6);
            show_hangman(error);
            goto end;

        case CODE_PLAYER_LOST: // Player reach the maximum error (6 errors)
            printf("Vous avez atteint le nombre maximum d'erreur\n");
            return 3;

        case CODE_PLAYER_WON: // Player won the game (he gess all the letters)
            printf("Vous avez trouvé le bon mot\n");
            return 2;

        default:
            printf("Message inconnu\n");
            goto end;
    }
    end:
    return 0;
}