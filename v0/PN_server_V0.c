#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
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
#define CODE_CLIENT_LOST 209
#define CODE_CLIENT_WON 210

#define CODE_NOT_A_LETTER 101
#define CODE_CRITICAL_ERROR 199

#define WORD "SOCKET"

#define ERROR_SOCKET "Error during socket creation: "
#define ERROR_ACCEPTATION "Error during client acceptation: "
#define ERROR_SENDING "Error during sending informations: "
#define ERROR_WRONG_CODE_LETTER_RECEIVED "CODE LETTER RECEIVED part: Not implementated"
#define ERROR_WRONG_CODE_RECEIVED "Unexpected code received"
#define ERROR_CLOSING_CLIENT_SOCKET "Error during closing client socket: "

/*
Compilation:
    gcc -o Server.run PN_server_V0.c server_functions/src/letter.c server_functions/src/connection_server.c -I server_functions
*/

int main(int argc, int *argv){
    unsigned char error_message[ERROR_LEN],  // Message sent if a critical error happened
                  client_message[MESSAGE_LEN], // Message sent by client
                  server_message[MESSAGE_LEN], // Message will be send by server
                  all_letter_in_word[strlen(WORD)], // All unique letters in researched words
                  all_letters_tried[NB_LETTERS_ALPHA], // All letters tried by client
                  word_received[strlen(WORD)], // Word sent by the client
                  letter_received; // Simple unsigned char

    int i; // Loop indice
    int socket_listen, socket_client; 
    int port = IPPORT_RESERVED;
    int try_error = TRY_ERROR; // Number of try remaining for client
    int number_letter = strlen(WORD);
    int buffer_positions[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // Buffer for letters positions in word
 
    if (argc > 0){ 
        port = 5000;
    }


    //Creating main socket
    if((socket_listen = socket_creation(port, MAX_CONNECTION)) <= 0){
                perror("Error during creating main socket_listen: ");
                _Exit(EXIT_FAILURE);
            }

    printf("Server is On...\n");
    

    while (1){
        start_loop: //Start initialisation for the game
            //Reset buffers
            memset(error_message, 0, ERROR_LEN);
            memset(client_message, 0, MESSAGE_LEN);
            memset(server_message, 0, MESSAGE_LEN);
            memset(all_letters_tried, 0, NB_LETTERS_ALPHA);
            i = 0;
            
            //Accept the client
            if((socket_client = client_acceptation(socket_listen)) <= 0){
                strcpy(error_message, ERROR_ACCEPTATION);
                goto error;
            }

            server_message[0] = CODE_NUMBER_LETTER;
            server_message[1] = number_letter;
            //Send to client how much letter are in word
            if((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                strcpy(error_message, ERROR_SENDING);
                goto error;
            }

            while (1){
                //Start of game loop
                //Reset buffers
                memset(client_message, 0, MESSAGE_LEN);
                memset(server_message, 0, MESSAGE_LEN);
                recv(socket_client, client_message, MESSAGE_LEN, 0);

                for(int i = 0; i < 10; i++) {
                    printf("%d ", client_message[i]);
                }
                printf("\n");

                all_letters_in_word(WORD, all_letter_in_word);

                switch (client_message[0]){

                case CODE_LETTER_RECEIVED: //Client is sending a letterx
                    //Letter verification
                    if ((letter_received = upper_letter(client_message[1])) == 0){
                        server_message[0] = CODE_NOT_A_LETTER;
                        if((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                            strcpy(error_message, ERROR_SENDING);
                            goto error;
                        }
                        break; // Leave CODE_LETTER_RECEIVED and switch statements
                    }
                    if (letter_in_word(WORD, letter_received)){ //Letter is in word
                        if (letter_in_word(all_letter_in_word, letter_received)){ //Letter is in all letter buffer
                            // Adding the new letter found to list of all letter tried
                            all_letters_tried[letter_in_word(all_letters_tried, 0)-1] = letter_received;
                            // Suppr letter found of all unique letters in word
                            all_letter_in_word[letter_in_word(all_letter_in_word, letter_received)-1] = 0;

                            // Verify is client found all laters one by one
                            if (verif_only_zero(all_letter_in_word)){
                                server_message[0] = CODE_CLIENT_WON;

                                if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                            }

                            // There is still letters to find
                            else{
                                server_message[0] = CODE_LETTER_FOUND_IN_WORD;

                                position_letter(WORD, buffer_positions, letter_received);
                                do{
                                    server_message[i+1] = buffer_positions[i]; 
                                } while (buffer_positions[i++] > 0); // i++ evalued after comparaison
                            
                                if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                            }
                        }
                        else if (letter_in_word(all_letters_tried, letter_received)){ // Letter already sent by client
                            server_message[0] = CODE_LETTER_ALREADY_SENT;
                            if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                           }
                        }

                        else{
                            // Not implemented statement
                            strcpy(error_message, ERROR_WRONG_CODE_LETTER_RECEIVED);
                            goto error;
                        }
                    }
                    else { //Letter is not in word
                        if (letter_in_word(all_letters_tried, letter_received)){ //Letter already sent by client
                           server_message[0] = CODE_LETTER_ALREADY_SENT;

                           if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                           }
                        }
                        else { //Letter is tried by client for the first time

                            try_error--; //Life remaining -1

                            if (try_error <= 0){ //If client has no life remaining,
                            server_message[0] = CODE_CLIENT_LOST; //send code then end the game
                            
                                if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }

                                goto game_end;
                        }
                            else { //Client has still life remaining
                                server_message[0] = CODE_LETTER_NOT_IN_WORD;
                                server_message[1] = try_error;

                                if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;

                                }
                            }
                        }
                    }
                    
                    break; //Leave CODE_LETTER_RECEIVED and switch statements

                case CODE_CLIENT_SENT_A_WORD: //Client sent a word
                    extract_word(word_received, client_message); //Recovery word tried without code
                    if (strcmp(word_received, WORD) == 0){ // Client sent good word
                        server_message[0] = CODE_CLIENT_FOUND_WORD; // Client won

                        if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                            strcpy(error_message, ERROR_SENDING);
                            goto error;
                        }
                        
                        goto game_end; 
                        }

                    else{
                        // Word tried is false
                        try_error--;

                        if (try_error <= 0){ // Client has no life remaining, lost
                            server_message[0] = CODE_CLIENT_LOST; // Client lost
                            
                            if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }

                            goto game_end;
                        }
                        else{
                            server_message[0] = CODE_CLIENT_NOT_FOUND_WORD;
                            server_message[1] = try_error;

                            if ((send(socket_client, server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }
                        }

                    break; // Leave CODE_CLIENT_SENT_A_WORD and switch statements
                    }
                
                default:
                    // If the code isn't recognized
                    strcpy(error_message, ERROR_WRONG_CODE_RECEIVED);
                    goto error;
                    // break;
                }
            }


    error:
        // Cancel client's game
        // Error display
        perror(error_message);
        memset(server_message, 0, MESSAGE_LEN); // Reset server message buffer

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
        goto start_loop; // Return to client acceptation

    game_end:
        // Client's game is ended
        if(close(socket_client) > 0){
            printf("Client socket closed properly");
            goto start_loop; // Return to client acceptation
        }
        else{
            perror("Client socket cannot be closed: ");
            _Exit(EXIT_FAILURE);
        }
    }
}