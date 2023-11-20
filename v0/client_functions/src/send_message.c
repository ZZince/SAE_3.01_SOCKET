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


// Constants
#define CODE_LETTER_RECEIVED 202 // Send letter validation code


// Depedencies
#include "../client_functions.h"

////////////////////////////// FUNCTIONS ////////////////////////////////////////////////


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
