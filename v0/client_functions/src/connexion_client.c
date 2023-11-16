#include "../connexion_client.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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