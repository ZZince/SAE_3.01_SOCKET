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
# include <ctype.h>   // get isDigit()
#include <string.h>	// memset()
#include <netinet/in.h>	// struct sockaddr_in
#include <arpa/inet.h> // htons() and inet_aton() 

// External functions
#include "client_functions/client_functions.h"

// Constant
#define MESSAGE_LEN 50 // = Maximal lenght of the messages
#define MAX_WORD_SIZE 256 // = Maximal lenght of the word
#define CODE_CLIENT_CHOOSE_WORD 214 // = Client choose the custom word
#define CODE_CLIENT2_FOUND_WORD 216
#define CODE_CLIENT2_LOST_GAME 217
#define CODE_CLIENT2_FOUND_LETTER 218
#define CODE_CLIENT2_LOST_TRY 219

int main(int argc, char *argv[]){

	int socket;
	struct sockaddr_in sockaddrDistant;
	socklen_t longueurAdresse;

	unsigned char buffer[MESSAGE_LEN];
	int nb; /* number of bytes read and written */

	char ip_dest[16];
	int  port_dest;

	// Game attributes
	char letter; // The character to send to the server
	int send; // Verify if the message is correctly send
	unsigned char *received_message; // Variable that stores the server message
	char word[MAX_WORD_SIZE]; // The word the client have to find
	int word_not_found = 0; // Check if the word is found or not (victory condition)
	char choice; // Choice of the client between send character or send word
	char *client_word; // The word the client choosed
	
	memset(buffer, 0x00, MESSAGE_LEN); // Initialize the buffer
	memset(word, 0x00, MAX_WORD_SIZE); // Initialize the word

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
	printf("Création\n");

	// Start of the connexion with the server
	if((connect(socket, (struct sockaddr *)&sockaddrDistant, longueurAdresse)) == -1){
		perror("Erreur de connection avec le serveur distant...");
		close(socket);
		exit(-2); // ERROR
	}

	printf("Connexion au serveur %s:%d réussie!\n",ip_dest,port_dest);

	client1_loop:
	//memset(received_message, 0, MESSAGE_LEN);
	printf("En attente du serveur de jeu..\n");
	// Get a message from the server
	received_message = get_message_from_server(socket, MESSAGE_LEN);
	if (received_message == NULL) {
		perror("Erreur lors de la réception du message");
		exit(-1); // ERROR
	}

	// Client 1 Choose a custom word, client 2 have to find it
    if (received_message[0] == CODE_CLIENT_CHOOSE_WORD) {
        // Client choose the custom word of the game

		printf("Choisissez le mot pour le second joueur\n");
        client_word = select_word();
        if (send = send_custom_word_to_server(socket, client_word, MESSAGE_LEN) == -1) {
            perror("Envoi du message impossible");
        }

        goto client1_loop;
    }
    else if (received_message[0] == CODE_CLIENT2_FOUND_LETTER)
    {
        printf("Le joueur 2 à trouvé la lettre: %c\n", received_message[1]);
        goto client1_loop;
    }

    else if (received_message[0] == CODE_CLIENT2_FOUND_WORD)
    {
        printf("Le joueur 2 à deviné votre mot !\n");
        goto end_observer;
    }
    

    else if (received_message[0] == CODE_CLIENT2_LOST_GAME)
    {
        printf("Le joueur 2 à échoué à trouvr votre mot !\n");
        goto end_observer;
    }

    else if (received_message[0] == CODE_CLIENT2_LOST_TRY)
    {
        printf("Le joueur 2 à commit une erreur, il lui reste %d tentative(s)\n", received_message[1]);
		goto client1_loop;
    }
	
	else {
		// Translate the message that the server send
		word_not_found = translate_message(received_message, word, letter);
	}

	memset(received_message, 0x00, MESSAGE_LEN); // refresh the message

	// Game Loop
	while(!word_not_found){

		// Get a message from the server (check if client can play or not)
		received_message = get_message_from_server(socket, MESSAGE_LEN);
		if (received_message == NULL) {
			perror("Erreur lors de la réception du message");
			exit(-1); // ERROR
		}

		// Translate the message that the server send
		word_not_found = translate_message(received_message, word, letter);

		if (word_not_found) {
			goto end;
		}

		// Ask client what his action
		printf("1: Send a letter\n2: Send the word\n");
		ask:
			scanf("%c", &choice);
			if (choice != 49 && choice != 50)
				goto ask;

		switch(choice) {
			case '1':
				// Choose a letter
				letter = select_letter();

				// Send the letter to the server
				if (send = send_character_to_server(socket, letter, MESSAGE_LEN) == -1) {
					perror("Envoi du message impossible");
				}
				break;
			
			case '2':
				// Choose the word
				client_word = select_word();

				// Send the word to the server
				if (send = send_word_to_server(socket, client_word, MESSAGE_LEN) == -1) {
					perror("Envoi du message impossible");
				}
				break;
		}


		// Get a message from the server
		received_message = get_message_from_server(socket, MESSAGE_LEN);
		if (received_message == NULL) {
			perror("Erreur lors de la réception du message");
			exit(-1); // ERROR
		}

		// Translate the message that the server send
		word_not_found = translate_message(received_message, word, letter);

		memset(received_message, 0x00, MESSAGE_LEN); // refresh the message
	}

	end:

		// If player have won (find the good word)
		if (word_not_found == 1) {
			printf("Le mot était : ");
			for(int i = 0; i < 10; i++) {
				printf("%c", client_word[i]);
			}
			printf("\n");
		}

		if (word_not_found == 2) {
			printf("Le mot était : SOCKET\n");
		}
		
		// Free memory allocated
		free(received_message);

		// Close the socket and leave
		close(socket);
		return 0;

	end_observer:

		close(socket);
		return 0;
}


/* Compilation:
gcc -o Client.run PN_client_v2.c client_functions/src/show_hangman.c client_functions/src/connexion_client.c client_functions/src/character_selection.c client_functions/src/message_client.c client_functions/src/game_client.c
Run: 
./Client.run
*/