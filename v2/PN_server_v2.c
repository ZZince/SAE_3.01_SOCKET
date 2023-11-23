//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////																			//////
/////		Author : Artu Florient												//////
/////		Date : 24/11/2023													//////
/////		Subject : Server main program										//////
/////																			//////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/* This program aims to manage all the main logic of the server. */

////////////////////////////// IMPORT ////////////////////////////////////////////////

// Library
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "server_functions/connection_server.h"
#include "server_functions/letter.h"

// Constants
#define MAX_CONNECTION 2
#define MESSAGE_LEN 50
#define ERROR_LEN 256
#define TRY_ERROR 6
#define NB_LETTERS_ALPHA 26

// Codes
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
#define CODE_OPPONENT_LOST 211
#define CODE_OPPONENT_WON 212
#define CODE_ALLOW_CLIENT_TO_PLAY 213
#define CODE_CLIENT_CHOOSE_CUSTOM_WORD 214
#define CODE_CLIENT_SENT_CUSTOM_WORD 215
#define CODE_CLIENT2_FOUND_WORD 216
#define CODE_CLIENT2_LOST_GAME 217
#define CODE_CLIENT2_FOUND_LETTER 218
#define CODE_CLIENT2_LOST_TRY 219

#define CODE_NOT_A_LETTER 101
#define CODE_CRITICAL_ERROR 199

// Errors
#define ERROR_SOCKET "Error during socket creation: "
#define ERROR_ACCEPTATION "Error during client acceptation: "
#define ERROR_SENDING "Error during sending informations: "
#define ERROR_WRONG_CODE_LETTER_RECEIVED "CODE LETTER RECEIVED part: Not implementated"
#define ERROR_WRONG_CODE_RECEIVED "Unexpected code received"
#define ERROR_CLOSING_CLIENT_SOCKET "Error during closing client socket: "

int main(int argc, char *argv[]){

    char word[MESSAGE_LEN]; // The word client 2 have to find (choosed by client 1)

    unsigned char error_message[ERROR_LEN],  // Message sent if a critical error happened
                  client_message[MESSAGE_LEN], // Message sent by client
                  server_message[MESSAGE_LEN], // Message will be send by server
                  all_letter_in_word_client[strlen(word)], // All unique letters in researched words
                  all_letters_tried_client[NB_LETTERS_ALPHA], // All letters tried by client1
                  word_received[strlen(word)], // Word sent by the client
                  letter_received; // Simple unsigned char

    int y; // Loop indice
    int word_choiced = 0;
    int socket_listen, nb_client;
    int socket_client1[1], socket_client2[1];
    int port = IPPORT_RESERVED;
    int try_error_client = TRY_ERROR; // Number of try remaining for client
    int number_letter = strlen(word);
    int buffer_positions[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // Buffer for letters positions in word

    if (argc > 1){ 
		sscanf(argv[1],"%d",&port);
    }

    // Creating main socket
    if((socket_listen = socket_creation(port, MAX_CONNECTION)) <= 0){
                perror("Error during creating main socket_listen: ");
                _Exit(EXIT_FAILURE);
            }

    printf("Server is On...\n");

    while(1){
        setup_game:
            // Waiting for 2 clients
            if (wait_for_two_clients(socket_listen, socket_client1, socket_client2) != 0){
                perror(ERROR_ACCEPTATION);
                _Exit(EXIT_FAILURE);
            }

            printf("Sockets created\n");

            // Buffers reset
            memset(error_message, 0x00, ERROR_LEN);
            memset(client_message, 0x00, MESSAGE_LEN);
            memset(server_message, 0x00, MESSAGE_LEN);
            memset(word, 0x00, MESSAGE_LEN);
            memset(all_letters_tried_client, 0x00, NB_LETTERS_ALPHA);

            printf("Initialisation\n");

            while(word_choiced == 0) {
                
                server_message[0] = CODE_CLIENT_CHOOSE_CUSTOM_WORD;
                if((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                    strcpy(error_message, ERROR_SENDING);
                    goto error;
                }

                recv(socket_client1[0], client_message, MESSAGE_LEN, 0);

                if (client_message[0] == CODE_CLIENT_SENT_CUSTOM_WORD) {
                    for (int j = 0; j < MESSAGE_LEN; j++) {
                        word[j] = client_message[j + 1];
                    }
                    all_letters_in_word(word, all_letter_in_word_client );
                    word_choiced = 1;
                }
            }

            start:
            // Send to client2 len of word
            server_message[0] = CODE_NUMBER_LETTER;
            server_message[1] = strlen(word);


            if((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                strcpy(error_message, ERROR_SENDING);
                goto error;
            }

            while (1){
                //Start of game loop
                //Reset buffers

                printf("boucle\n");
                memset(client_message, 0, MESSAGE_LEN);
                memset(server_message, 0, MESSAGE_LEN);
                memset(buffer_positions, 0, 9);
                y = 0;
                
                recv(socket_client2[0], client_message, MESSAGE_LEN, 0);

                for(int i = 0; i < 10; i++) {
                    printf("%d ", client_message[i]);
                }
                printf("\n");


                switch (client_message[0]){

                case CODE_LETTER_RECEIVED: //Client is sending a letter
                    //Letter verification
                    if ((letter_received = upper_letter(client_message[1])) == 0){
                        server_message[0] = CODE_NOT_A_LETTER;
                        if((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                            strcpy(error_message, ERROR_SENDING);
                            goto error;
                            }
                        break; // Leave CODE_LETTER_RECEIVED and switch statements
                        }

                    if (letter_in_word(word, letter_received, strlen(word))){ //Letter is in word
                        if (letter_in_word(all_letters_tried_client, letter_received, NB_LETTERS_ALPHA) == 0){ //Letter is not in all letter already tried
                            // Adding the new letter found to list of all letter tried
                            all_letters_tried_client[(letter_in_word(all_letters_tried_client, 0, NB_LETTERS_ALPHA)-1)] = letter_received;
                            
                            printf("all letters tried: \n");
                            for(int i = 0; i <= 26; i++){
                                printf("%d", all_letters_tried_client[i]);
                            }
                            
                            printf("\n");
                            // Suppr letter found of all unique letters in word
                            
                            all_letter_in_word_client[letter_in_word(all_letter_in_word_client, letter_received, strlen(word))-1] = 0;
                            
                            printf("all letter in word: \n");
                            for (int i = 0; i <= strlen(word); i++){
                                printf("%d ", all_letter_in_word_client[i]);
                            }
                            printf("\n");
                            
                            // Verify is client found all letters one by one
                            printf("verif only 0: %d\n", verif_only_zero(all_letter_in_word_client, strlen(word)));
                            if (verif_only_zero(all_letter_in_word_client, strlen(word)) == 1){
                                server_message[0] = CODE_CLIENT_WON;
                                if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                                
                                memset(server_message, 0, MESSAGE_LEN);
                                server_message[0] = CODE_CLIENT2_FOUND_WORD;
                                if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                                goto game_end;
                            }

                            // There is still letters to find
                            else{
                                server_message[0] = CODE_LETTER_FOUND_IN_WORD;
                                position_letter(word, buffer_positions, letter_received, strlen(word));
                                do{
                                    y++;
                                    printf("y = %d\n", y);
                                    server_message[y] = buffer_positions[y-1]-1; 
                                } while (buffer_positions[y] > 0); 

                                server_message[y+1] = 255;

                                if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }

                                memset(server_message, 0, MESSAGE_LEN);
                                server_message[0] = CODE_CLIENT2_FOUND_LETTER;
                                server_message[1] = letter_received;

                                if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                            }
                        }
                        else if (letter_in_word(all_letters_tried_client, letter_received, NB_LETTERS_ALPHA) > 0){ // Letter already sent by client
                            server_message[0] = CODE_LETTER_ALREADY_SENT;
                            if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
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
                        if (letter_in_word(all_letters_tried_client, letter_received, NB_LETTERS_ALPHA)){ //Letter already sent by client
                            server_message[0] = CODE_LETTER_ALREADY_SENT;
                            if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }
                        }
                        else { //Letter is tried by client for the first time
                            try_error_client--; //Life remaining -1

                            if (try_error_client <= 0){ //If client has no life remaining,
                                server_message[0] = CODE_CLIENT_LOST; //send code then end the game
                            
                                if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                            }
                                memset(server_message, 0, MESSAGE_LEN);
                                server_message[0] = CODE_CLIENT2_LOST_GAME;

                                if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                                goto game_end;
                            
                            
                        }
                            else { //Client has still life remaining
                                all_letters_tried_client[letter_in_word(all_letters_tried_client, 0, NB_LETTERS_ALPHA)-1] = letter_received;

                                server_message[0] = CODE_LETTER_NOT_IN_WORD;
                                server_message[1] = try_error_client;

                                if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }

                                memset(server_message, 0, MESSAGE_LEN);
                                server_message[0] = CODE_CLIENT2_LOST_TRY;
                                server_message[1] = try_error_client;

                                if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                            }
                        }
                    }
                    
                    break; //Leave CODE_LETTER_RECEIVED and switch statements

                case CODE_CLIENT_SENT_A_WORD: //Client sent a word
                    extract_word(word_received, client_message); //Recovery word tried without code
                    if (strcmp(word_received, word) == 0){ // Client sent good word
                        server_message[0] = CODE_CLIENT_FOUND_WORD; // Client won

                        if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                            strcpy(error_message, ERROR_SENDING);
                            goto error;
                        }
                        memset(server_message, 0, MESSAGE_LEN);
                        server_message[0] = CODE_CLIENT2_FOUND_WORD;

                        if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                            strcpy(error_message, ERROR_SENDING);
                            goto error;
                        }
                        goto game_end; 
                        }

                    else{
                        // Word tried is false
                        try_error_client--;

                        if (try_error_client <= 0){ // Client has no life remaining, lost
                            server_message[0] = CODE_CLIENT_LOST; // Client lost
                            
                            if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }
                            memset(server_message, 0, MESSAGE_LEN);
                            server_message[0] = CODE_CLIENT2_LOST_GAME;

                            if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }
                            goto game_end;
                        }
                        else{
                            server_message[0] = CODE_CLIENT_NOT_FOUND_WORD;
                            server_message[1] = try_error_client;

                            if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }

                            memset(server_message, 0, MESSAGE_LEN);
                            server_message[0] = CODE_CLIENT2_LOST_GAME;

                            if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }
                            
                        }

                    break; // Leave CODE_CLIENT_SENT_A_WORD and switch statements
                    }
                
                default:
                    printf("test\n");
                    // If the code isn't recognized
                    strcpy(error_message, ERROR_WRONG_CODE_RECEIVED);
                    goto error;
                    // break;
                }
            }

       }

    error:
        // Error display
        perror(error_message);
        close(socket_client1[0]);
        close(socket_client2[0]);
        close(socket_listen);

        _Exit(EXIT_FAILURE);

    game_end:
        // Sockets closing
        close(socket_client1[0]);
        close(socket_client2[0]);
        word_choiced = 0;

        // Return to client acceptation
        goto setup_game;   
}


/*
Compilation:
gcc -o Server.run PN_server_v2.c server_functions/src/letter.c server_functions/src/connection_server.c -I server_functions
*/