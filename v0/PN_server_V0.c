#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "server_functions/connection_server.h"
#include "server_functions/letter.h"

#define MAX_CONNECTION 1
#define MESSAGE_LEN 10
#define ERROR_LEN 256
#define TRY_ERROR 6
#define NB_LETTERS_ALPHA 26

#define CODE_NUMBER_LETTER 201
#define CODE_LETTER_RECEIVED 202
#define CODE_LETTER_ALREADY_SENT 203
#define CODE_LETTER_FOUND_IN_WORD 204
#define CODE_LETTER_NOT_IN_WORD 205
#define CODE_CLIENT_SENT_A_WORD 206
#define CODE_CLIENT_FOUND_WORD 207
#define CODE_CLIENT_NOT_FOUND_WORD 208

#define CODE_NOT_A_LETTER 101
#define CODE_CRITICAL_ERROR 199

#define WORD "socket"

#define ERROR_SOCKET "Error during socket creation: "
#define ERROR_ACCEPTATION "Error during client acceptation: "
#define ERROR_SENDING "Error during sending informations: "
#define ERROR_WRONG_CODE_LETTER_RECEIVED "CODE LETTER RECEIVED part: Not implementated"
#define ERROR_CLOSING_CLIENT_SOCKET "Error during closing client socket: "

int main(int argc, int *argv){
    unsigned char error_message[ERROR_LEN],  //Message sent if a critical error happened
                  client_message[MESSAGE_LEN], //Message sent by client
                  server_message[MESSAGE_LEN], //Message will be send by server
                  all_letters_in_word[strlen(WORD)], //All unique letters in researched words
                  all_letters_tried[NB_LETTERS_ALPHA], //All letters tried by client
                  word_received[strlen(WORD)], //Word sent by the client
                  letter_received; //Simple unsigned char


    int socket_listen, socket_client;
    int port = IPPORT_RESERVED;
    int try_error = TRY_ERROR;
    int number_letter = strlen(WORD);

    if (argc > 0){ 
        port = IPPORT_RESERVED;
    }
    while (1){
        start_loop:
            memset(error_message, 0, ERROR_LEN);
            memset(client_message, 0, MESSAGE_LEN);
            memset(server_message, 0, MESSAGE_LEN);
            memset(all_letters_tried, 0, NB_LETTERS_ALPHA);
            

            if((socket_listen = socket_creation(port, MAX_CONNECTION)) <= 0){
                strcpy(error_message, ERROR_SOCKET);
                goto error;
            }

            if((socket_client = client_acceptation(socket_listen)) <= 0){
                strcpy(error_message, ERROR_ACCEPTATION);
                goto error;
            }

            server_message[0] = CODE_NUMBER_LETTER;
            server_message[1] = number_letter;
            if((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                strcpy(error_message, ERROR_SENDING);
                goto error;
            }

            while (1){
                memset(client_message, 0, MESSAGE_LEN);
                memset(server_message, 0, MESSAGE_LEN);
                recv(socket_client, client_message, MESSAGE_LEN, 0);
                switch (client_message[0]){

                case CODE_LETTER_RECEIVED: //Client is sending a letter
                    if ((letter_received = upper_letter(client_message[1])) == 0){
                        server_message[0] = CODE_NOT_A_LETTER;
                        if((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                            strcpy(error_message, ERROR_SENDING);
                            goto error;
                        }
                        break; //Leave CODE_LETTER_RECEIVED statement
                    }
                    if (letter_in_word(WORD, client_message[1])){ //Letter is in word
                        if (letter_in_word(all_letters_in_word, client_message[1])){ //Letter is in all letter buffer
                            //Adding the new letter found to list of all letter tried
                            all_letters_tried[letter_in_word(all_letters_tried, 0)-1] = client_message[1];
                            //Suppr letter found of all unique letters in word
                            all_letters_in_word[letter_in_word(all_letters_in_word, client_message[1])-1] = 0;

                            server_message[0] = CODE_LETTER_FOUND_IN_WORD;

                            //TODO - Implémenter l'envois des positions
                            if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }
                        }
                        else if (letter_in_word(all_letters_tried, client_message[1])){ //Letter already sent by client
                           server_message[0] = CODE_LETTER_ALREADY_SENT;

                           if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                           }
                        }

                        else{
                            strcpy(error_message, ERROR_WRONG_CODE_LETTER_RECEIVED);
                            goto error;
                        }
                    }
                    else { //Letter is not in word
                        if (letter_in_word(all_letters_tried, client_message[1])){ //Letter already sent by client
                           server_message[0] = CODE_LETTER_ALREADY_SENT;

                           if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                           }
                        }
                        else { //Letter is tried by client for the first time

                            try_error--; //Life remaining -1
                            server_message[0] = CODE_LETTER_NOT_IN_WORD;
                            server_message[1] = try_error;

                            if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;

                            }
                        }
                    }
                    
                    break;

                case CODE_CLIENT_SENT_A_WORD:
                    extract_word(word_received, client_message);
                    if (strcmp(word_received, WORD) == 0){ // Client sent the client
                        server_message[0] = CODE_CLIENT_FOUND_WORD;

                        if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                            strcpy(error_message, ERROR_SENDING);
                            goto error;
                        }
                        
                        goto game_end;
                        }

                    else{
                        try_error--;
                        server_message[0] = CODE_CLIENT_NOT_FOUND_WORD;
                        server_message[1] = try_error;

                        if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                            strcpy(error_message, ERROR_SENDING);
                            goto error;
                        }

                    break;


                    }
                
                default:
                    strcpy(error_message, ERROR_WRONG_CODE_LETTER_RECEIVED);
                    goto error;
                    //break;
                }
            }


    error:
        //Error display
        perror(error_message);
        memset(server_message, 0, MESSAGE_LEN); //Reset server message buffer

        server_message[0] = CODE_CRITICAL_ERROR;
        send(socket_client, server_message, MESSAGE_LEN, 0);

        if(close(socket_client) > 0){
            printf("Client socket closed");
        }
        else{
            perror(ERROR_CLOSING_CLIENT_SOCKET);
            _Exit(EXIT_FAILURE);
        }
        memset(error_message, 0, sizeof(error_message));
        goto start_loop;

    game_end:
        if(close(socket_client) > 0){
            printf("Client socket closed properly");
            goto start_loop;
        }
        else{
            perror("Client socket cannot be closed: ");
            _Exit(EXIT_FAILURE);
        }
    }
}