//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////																			//////
/////		Author : Fourny Nathan												//////
/////		Date : 16/11/2023													//////
/////		Subject : Client main program										//////
/////																			//////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/* This program aims to manage all the main logic of the client. */

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

// External functions
#include "client_functions/connexion_client.h"

// Constant
#define LG_MESSAGE 256 // = Maximal lenght of the messages

int main(int argc, char *argv[]){

	int socket;
	struct sockaddr_in sockaddrDistant;
	socklen_t longueurAdresse;

	char buffer[LG_MESSAGE];
	int nb; /* number of bytes read and written */

	char ip_dest[16];
	int  port_dest;

	// Get the ip and the port of the server the client want to connect
    if (argc>1) {
		strncpy(ip_dest,argv[1],16);
		sscanf(argv[2],"%d",&port_dest);
	}else{
		printf("USAGE : %s server_ip server_port\n",argv[0]);
		exit(-1);
	}

	////////////////////////////// Socket Creation ////////////////////////////////////////////////


	// Creation of the communication socket
	socket = communication_socket_creation();

	// Get the lenght of the sockaddr_in struct
	longueurAdresse = sizeof(sockaddrDistant);
	// Initiliaze struc sockaddr_in to 0
	memset(&sockaddrDistant, 0x00, longueurAdresse);
	// Populates sockaddr_in structure with remote server information
	sockaddrDistant.sin_family = AF_INET;
	// Select the listen port of the server
	sockaddrDistant.sin_port = htons(port_dest);
	// Select the ipv4 address of the server
	inet_aton(ip_dest, &sockaddrDistant.sin_addr);


	////////////////////////////// Server Connexion ////////////////////////////////////////////////


	// Start of the connexion with the server
	if((connect(socket, (struct sockaddr *)&sockaddrDistant, longueurAdresse)) == -1){
		perror("Erreur de connection avec le serveur distant...");
		close(socket);
		exit(-2); // ERROR
	}

	printf("Connexion au serveur %s:%d réussie!\n",ip_dest,port_dest);

	

	// Close the socket and leave
	close(socket);
	return 0;
}


// Compilation
// gcc -o client PN_client_v0.c client_functions/src/connexion_client.c
// Execution
// ./client