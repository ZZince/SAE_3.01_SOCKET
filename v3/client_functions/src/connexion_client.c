//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////																			//////
/////		Author : Fourny Nathan												//////
/////		Date : 16/11/2023													//////
/////		Subject : connexion to client function								//////
/////																			//////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/* Utils for the connexion to a server and the creation of the socket. */

////////////////////////////// IMPORT ////////////////////////////////////////////////

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <string.h> // memset()

// Depedencies
#include "../client_functions.h"


////////////////////////////// FUNCTIONS ////////////////////////////////////////////////


/*Create a communication socket
    Return:
        - [int] Communication socket
*/
int communication_socket_creation() {
    int communicationSocket;

    // Create the communication socket
	if((communicationSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Erreur en création de la socket...");
		return -1; // ERROR
	}

    printf("Socket créée! (%d)\n", communicationSocket); // Socket created successfuly

    return communicationSocket;
}