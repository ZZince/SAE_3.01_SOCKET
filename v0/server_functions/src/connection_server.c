//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////																			//////
/////		Author : Artu Florient												//////
/////		Date : 18/11/2023													//////
/////		Subject : connexion to client          								//////
/////																			//////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/* Utils to make the connexion between server and clients */

////////////////////////////// IMPORT ////////////////////////////////////////////////


// Library
#include "../connection_server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


////////////////////////////// FUNCTIONS /////////////////////////////////////////////


/*Create, bind and return a socket
    Parameters:
        - [int] port (const): Port used for listening
        - [int] max_connection (const): Number of max connection possible to the server
    Return:
        - [int] Socket binded/listened
*/
int socket_creation(const int port, const int max_connection) {
    int listen_socket;
    struct sockaddr_in addr_inLocal;

    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error during socket creation: ");
        return -1;
    }

	memset(&addr_inLocal, 0x00, sizeof(struct sockaddr_in));
    addr_inLocal.sin_family = AF_INET;
    addr_inLocal.sin_addr.s_addr = htonl(INADDR_ANY); 
    addr_inLocal.sin_port = htons(port);

    if (bind(listen_socket, (struct sockaddr *)&addr_inLocal, sizeof(struct sockaddr_in)) < 0) {
        perror("Error during socket binding: ");
        return -2;
    }

    if (listen(listen_socket, max_connection) < 0) {
        perror("Error during socket listening: ");
        return -3;
    }

    return listen_socket;  
}

/*Wait for a client connection 
    Parameters:
        - [int] Socket ready to be used
    Return:
        - [int] New socket between client and server
*/
int client_acceptation(const int listen_socket) {
    int dialog_socket;
    struct sockaddr_in addr_inDistant;
	socklen_t longueurAdresse;

	if ((dialog_socket = accept(listen_socket, (struct sockaddr *)&addr_inDistant, &longueurAdresse)) < 0) {
		perror("Error during client acceptation: ");
        return -1;
	}

    return dialog_socket;

}




