#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "server_functions/connection_server.h"
#include "server_functions/letter.h"

#define MAX_CONNECTION 2
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
#define CODE_OPPONENT_LOST 211
#define CODE_OPPONENT_WON 212
#define CODE_ALLOW_CLIENT_TO_PLAY 213

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
gcc -o Server.run PN_server_v1.c server_functions/src/letter.c server_functions/src/connection_server.c -I server_functions
*/

int main(int argc, char *argv[]){
    unsigned char error_message[ERROR_LEN],  // Message sent if a critical error happened
                  client_message[MESSAGE_LEN], // Message sent by client
                  server_message[MESSAGE_LEN], // Message will be send by server
                  all_letter_in_word_client1[strlen(WORD)], // All unique letters in researched words
                  all_letter_in_word_client2[strlen(WORD)],
                  all_letters_tried_client1[NB_LETTERS_ALPHA], // All letters tried by client1
                  all_letters_tried_client2[NB_LETTERS_ALPHA], // All letters tried by client2
                  word_received[strlen(WORD)], // Word sent by the client
                  letter_received; // Simple unsigned char

    int y; // Loop indice
    int socket_listen, nb_client;
    int retry = 0;
    int socket_client1[1], socket_client2[1];
    int port = IPPORT_RESERVED;
    int try_error_client1 = TRY_ERROR; // Number of try remaining for client
    int try_error_client2 = TRY_ERROR;
    int number_letter = strlen(WORD);
    int buffer_positions[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // Buffer for letters positions in word
 
    if (argc > 1){ 
		sscanf(argv[1],"%d",&port);
    }

    //Creating main socket
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
            memset(error_message, 0, ERROR_LEN);
            memset(client_message, 0, MESSAGE_LEN);
            memset(server_message, 0, MESSAGE_LEN);
            memset(all_letters_tried_client1, 0, NB_LETTERS_ALPHA);
            memset(all_letters_tried_client2, 0, NB_LETTERS_ALPHA);
            all_letters_in_word(WORD, all_letter_in_word_client1);
            all_letters_in_word(WORD, all_letter_in_word_client2);

            printf("Initialisation\n");

            // Send to both clients len of word
            server_message[0] = CODE_NUMBER_LETTER;
            server_message[1] = strlen(WORD);
            if((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                strcpy(error_message, ERROR_SENDING);
                goto error;
            }

            if((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                strcpy(error_message, ERROR_SENDING);
                goto error;
            }

            while (1){
                //Start of game loop
                //Reset buffers

                // Switch between clients
                if (retry != 1){
                    if (nb_client == 0){
                        nb_client = 1;
                    }
                    else{
                        nb_client =  0;
                    }
                }
                printf("boucle\n");
                retry = 0;
                memset(client_message, 0, MESSAGE_LEN);
                memset(server_message, 0, MESSAGE_LEN);
                y = 0;
                
                if (nb_client == 0){ // Client 1
                    server_message[0] = CODE_ALLOW_CLIENT_TO_PLAY;
                    if((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                        strcpy(error_message, ERROR_SENDING);
                        goto error;
                    }
                    memset(server_message, 0, MESSAGE_LEN);

                    recv(socket_client1[0], client_message, MESSAGE_LEN, 0);


                    for(int i = 0; i < 10; i++) {
                        printf("%d ", client_message[i]);
                    }
                    printf("\n");

                }

                else{ // Client 2
                    server_message[0] = CODE_ALLOW_CLIENT_TO_PLAY;
                    if((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                        strcpy(error_message, ERROR_SENDING);
                        goto error;
                    }
                    memset(server_message, 0, MESSAGE_LEN);
                    recv(socket_client2[0], client_message, MESSAGE_LEN, 0);

                    for(int i = 0; i < 10; i++) {
                        printf("%d ", client_message[i]);
                    }
                    printf("\n");    
                }


                if (nb_client == 0){ // CLient 1

                    switch (client_message[0]){

                    case CODE_LETTER_RECEIVED: //Client is sending a letter
                        //Letter verification
                        if ((letter_received = upper_letter(client_message[1])) == 0){
                            server_message[0] = CODE_NOT_A_LETTER;
                            if((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                                }
                            retry = 1;
                            break; // Leave CODE_LETTER_RECEIVED and switch statements
                            }

                        if (letter_in_word(WORD, letter_received, strlen(WORD))){ //Letter is in word
                            if (letter_in_word(all_letters_tried_client1, letter_received, NB_LETTERS_ALPHA) == 0){ //Letter is not in all letter already tried
                                // Adding the new letter found to list of all letter tried
                                all_letters_tried_client1[(letter_in_word(all_letters_tried_client1, 0, NB_LETTERS_ALPHA)-1)] = letter_received;
                                printf("all letters tried: \n");
                                for(int i = 0; i <= 26; i++){
                                    printf("%d", all_letter_in_word_client1[i]);
                                    }
                                printf("\n");
                                // Suppr letter found of all unique letters in word
                                all_letter_in_word_client1[letter_in_word(all_letter_in_word_client1, letter_received, strlen(WORD))-1] = 0;
                                printf("all letter in word: \n");
                                for (int i = 0; i <= strlen(WORD); i++){
                                    printf("%d ", all_letter_in_word_client1[i]);
                                }
                                printf("\n");
                                // Verify is client found all laters one by one
                                printf("verif only 0: %d\n", verif_only_zero(all_letter_in_word_client1, strlen(WORD)));
                                if (verif_only_zero(all_letter_in_word_client1, strlen(WORD)) == 1){
                                    server_message[0] = CODE_CLIENT_WON;
                                    if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                        strcpy(error_message, ERROR_SENDING);
                                        goto error;
                                    }
                                    memset(server_message, 0, MESSAGE_LEN);
                                    server_message[0] = CODE_OPPONENT_WON;
                                    goto game_end;
                                }

                                // There is still letters to find
                                else{
                                    server_message[0] = CODE_LETTER_FOUND_IN_WORD;

                                    position_letter(WORD, buffer_positions, letter_received, strlen(WORD));
                                    do{
                                        y++;
                                        printf("y = %d\n", y);
                                        server_message[y] = buffer_positions[y-1]-1; 
                                    } while (buffer_positions[y] > 0); 

                                    server_message[y+1] = 255;

                                    if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                        strcpy(error_message, ERROR_SENDING);
                                        goto error;
                                    }
                                }
                            }
                            else if (letter_in_word(all_letters_tried_client1, letter_received, NB_LETTERS_ALPHA) > 0){ // Letter already sent by client
                                server_message[0] = CODE_LETTER_ALREADY_SENT;
                                if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                            }
                            retry = 1;
                        }

                        else{
                            // Not implemented statement
                            strcpy(error_message, ERROR_WRONG_CODE_LETTER_RECEIVED);
                            goto error;
                        }
                        }
                        else { //Letter is not in word
                            if (letter_in_word(all_letters_tried_client1, letter_received, NB_LETTERS_ALPHA)){ //Letter already sent by client
                                server_message[0] = CODE_LETTER_ALREADY_SENT;

                                if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                                retry = 1;
                            }
                            else { //Letter is tried by client for the first time

                                try_error_client1--; //Life remaining -1

                                if (try_error_client1 <= 0){ //If client has no life remaining,
                                    server_message[0] = CODE_CLIENT_LOST; //send code then end the game
                            
                                    if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                        strcpy(error_message, ERROR_SENDING);
                                        goto error;
                                }
                                    memset(server_message, 0, MESSAGE_LEN);
                                    server_message[0] = CODE_OPPONENT_LOST;
                                    goto game_end;
                            
                            
                            }
                                else { //Client has still life remaining
                                    all_letters_tried_client1[letter_in_word(all_letters_tried_client1, 0, NB_LETTERS_ALPHA)-1] = letter_received;

                                    server_message[0] = CODE_LETTER_NOT_IN_WORD;
                                    server_message[1] = try_error_client1;

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
                        if (strcmp(word_received, WORD) == 0){ // Client sent good word
                            server_message[0] = CODE_CLIENT_FOUND_WORD; // Client won

                            if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }
                            memset(server_message, 0, MESSAGE_LEN);
                            server_message[0] = CODE_OPPONENT_WON;
                            goto game_end; 
                            }

                        else{
                            // Word tried is false
                            try_error_client1--;

                            if (try_error_client1 <= 0){ // Client has no life remaining, lost
                                server_message[0] = CODE_CLIENT_LOST; // Client lost
                            
                                if ((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                                memset(server_message, 0, MESSAGE_LEN);
                                server_message[0] = CODE_OPPONENT_LOST;
                                goto game_end;
                            }
                            else{
                                server_message[0] = CODE_CLIENT_NOT_FOUND_WORD;
                                server_message[1] = try_error_client1;

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

                else{ // Client 2
                    switch (client_message[0]){

                    case CODE_LETTER_RECEIVED: //Client is sending a letter
                        //Letter verification
                        if ((letter_received = upper_letter(client_message[1])) == 0){
                            server_message[0] = CODE_NOT_A_LETTER;
                            if((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                                }
                            retry = 1;
                            break; // Leave CODE_LETTER_RECEIVED and switch statements
                            }

                        if (letter_in_word(WORD, letter_received, strlen(WORD))){ //Letter is in word
                            if (letter_in_word(all_letters_tried_client2, letter_received, NB_LETTERS_ALPHA) == 0){ //Letter is not in all letter already tried
                                // Adding the new letter found to list of all letter tried
                                all_letters_tried_client2[(letter_in_word(all_letters_tried_client2, 0, NB_LETTERS_ALPHA)-1)] = letter_received;
                                printf("all letters tried: \n");
                                for(int i = 0; i <= 26; i++){
                                    printf("%d", all_letter_in_word_client2[i]);
                                    }
                                printf("\n");
                                // Suppr letter found of all unique letters in word
                                all_letter_in_word_client2[letter_in_word(all_letter_in_word_client2, letter_received, strlen(WORD))-1] = 0;
                                printf("all letter in word: \n");
                                for (int i = 0; i <= strlen(WORD); i++){
                                    printf("%d ", all_letter_in_word_client2[i]);
                                }
                                printf("\n");
                                // Verify is client found all laters one by one
                                printf("verif only 0: %d\n", verif_only_zero(all_letter_in_word_client2, strlen(WORD)));
                                if (verif_only_zero(all_letter_in_word_client2, strlen(WORD)) == 1){
                                    server_message[0] = CODE_CLIENT_WON;
                                    if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                        strcpy(error_message, ERROR_SENDING);
                                        goto error;
                                    }
                                    memset(server_message, 0, MESSAGE_LEN);
                                    server_message[0] = CODE_OPPONENT_WON;
                                    goto game_end;
                                }

                                // There is still letters to find
                                else{
                                    server_message[0] = CODE_LETTER_FOUND_IN_WORD;

                                    position_letter(WORD, buffer_positions, letter_received, strlen(WORD));
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
                                }
                            }
                            else if (letter_in_word(all_letters_tried_client2, letter_received, NB_LETTERS_ALPHA) > 0){ // Letter already sent by client
                                server_message[0] = CODE_LETTER_ALREADY_SENT;
                                if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                            }
                            retry = 1;
                        }

                        else{
                            // Not implemented statement
                            strcpy(error_message, ERROR_WRONG_CODE_LETTER_RECEIVED);
                            goto error;
                        }
                        }
                        else { //Letter is not in word
                            if (letter_in_word(all_letters_tried_client2, letter_received, NB_LETTERS_ALPHA)){ //Letter already sent by client
                                server_message[0] = CODE_LETTER_ALREADY_SENT;

                                if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                                retry = 1;
                            }
                            else { //Letter is tried by client for the first time

                                try_error_client2--; //Life remaining -1

                                if (try_error_client2 <= 0){ //If client has no life remaining,
                                    server_message[0] = CODE_CLIENT_LOST; //send code then end the game
                            
                                    if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                        strcpy(error_message, ERROR_SENDING);
                                        goto error;
                                }
                                    memset(server_message, 0, MESSAGE_LEN);
                                    server_message[0] = CODE_OPPONENT_LOST;
                                    goto game_end;
                            
                            
                            }
                                else { //Client has still life remaining
                                    all_letters_tried_client2[letter_in_word(all_letters_tried_client2, 0, NB_LETTERS_ALPHA)-1] = letter_received;

                                    server_message[0] = CODE_LETTER_NOT_IN_WORD;
                                    server_message[1] = try_error_client2;

                                    if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
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

                            if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                strcpy(error_message, ERROR_SENDING);
                                goto error;
                            }
                            memset(server_message, 0, MESSAGE_LEN);
                            server_message[0] = CODE_OPPONENT_WON;
                            goto game_end; 
                            }

                        else{
                            // Word tried is false
                            try_error_client2--;

                            if (try_error_client2 <= 0){ // Client has no life remaining, lost
                                server_message[0] = CODE_CLIENT_LOST; // Client lost
                            
                                if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                                    strcpy(error_message, ERROR_SENDING);
                                    goto error;
                                }
                                memset(server_message, 0, MESSAGE_LEN);
                                server_message[0] = CODE_OPPONENT_LOST;
                                goto game_end;
                            }
                            else{
                                server_message[0] = CODE_CLIENT_NOT_FOUND_WORD;
                                server_message[1] = try_error_client2;

                                if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
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
        }

    error:
        // Error display
        perror(error_message);
        close(socket_client1[0]);
        close(socket_client2[0]);
        close(socket_listen);

        _Exit(EXIT_FAILURE);

    game_end:

        // Send to client who didn't play this turn if an event happened (opponent won or lost)
        if (nb_client == 0){
            if ((send(socket_client2[0], server_message, MESSAGE_LEN, 0)) <= 0){
                perror(ERROR_SENDING);
                goto error;
            }
        }
        else{
            if((send(socket_client1[0], server_message, MESSAGE_LEN, 0)) <= 0){
                perror(ERROR_SENDING);
                goto error;
            }
        }

        // Sockets closing
        close(socket_client1[0]);
        close(socket_client2[0]);

        // Return to client acceptation
        goto setup_game;   
}