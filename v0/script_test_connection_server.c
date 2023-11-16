#include <stdio.h>
#include "connection_server.h"
#include <arpa/inet.h>

/*Some tests of connection_server.c functions*/
int main(){
    int socket;
    printf("Tests for connection_server.c:\n");

    printf("Creating a valid socket: \n");

    if (socket = socket_creation(IPPORT_USERRESERVED, 2) > 0){
        printf("Socket correctly created\n");
    }

    printf("End of tests");

    return 0;
}
//Compilation
//gcc -o tests_connection_server script_test_connection_server.c connection_server.c connection_server.h
//Running
//./tests_connection_server