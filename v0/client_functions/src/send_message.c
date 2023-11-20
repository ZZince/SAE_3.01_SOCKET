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


// Depedencies
#include "../client_functions.h"

////////////////////////////// FUNCTIONS ////////////////////////////////////////////////


int send_message_to_server(int socket, char character, int size) {

    char message[size];
	int nb;

    memset(message, 0x00, size);

	message[0] = character;

    printf("%s\n", message);

	switch(nb = write(socket, message, size)){
		case -1 : /* une erreur ! */
			perror("Erreur en écriture...");
			close(socket);
			return -1;
		case 0 : /* la socket est fermée */
			fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			return 0;
		default: /* envoi de n octets */
			printf("Message %s envoyé! (%d octets)\n\n", message, nb);
	}

	return 0;
}