#include <stdio.h>
#include <arpa/inet.h>
#include "connection_server.h"

/*Some tests of connection_server.c functions*/
int main(){
    int socket;
    printf("Tests for connection_server.c:\n");

    printf("Creating a valid socket: \n");

    if ((socket = socket_creation(IPPORT_USERRESERVED, 2)) > 0){
        printf("Socket correctly created : %d\n", socket);
        printf("Waiting for client acceptation: ... (CTRL + C to quit)\n");
        client_acceptation(socket);
    }

    printf("End of tests");

    return 0;
}
/*
Compilation
    gcc -o tests_connection_server.run script_test_connection_server.c server_functions/src/connection_server.c -I server_functions
Running
    ./tests_connection_server.run
*/