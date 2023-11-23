//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////																			//////
/////		Author : Artu Florient												//////
/////		Date : 18/11/2023													//////
/////		Subject : game functions            								//////
/////																			//////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/* Utils to make the game work properly */

////////////////////////////// IMPORT ////////////////////////////////////////////////


// Library
#include "../letter.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


////////////////////////////// FUNCTIONS /////////////////////////////////////////////


/*Return i+1 if the letter in found in word, 0 else
    Parameters:
        - [char*] word: Pointer to the word
        - [char] letter: ASCII character
        - [int] sizeof_buffer: Number of caracters in buffer
    Return:
        - [int]: Letter position + 1 in char if found, 0 else
*/
int letter_in_word(char* word, char letter, int sizeof_word){
    for (int i = 0; i < sizeof_word; i++){
        if (word[i] == letter){
            return i+1;
        }
    }

    return 0;
}

/*Add all unique letters found in word to buffer
    Parameters:
        - [char*] word: Pointer to the word
        - [char*] buffer: Pointer to buffer
    Return:
        - [int]: Number of unique letter added to buffer
*/
int all_letters_in_word(char* word, char* buffer){
    int index_buffer = 0;
    for (int i = 0; i < strlen(word); i++){
        if (letter_in_word(buffer, word[i], strlen(word)) == 0 && word[i] != '-'){
            buffer[index_buffer] = word[i];
            index_buffer++;
        }
    }
    return index_buffer;
}
/*Test if the char is a valid letter and return it in capital
    Parameters:
        - [char] letter: simple char
    Return:
        - [unsigned char]: 0 if the char isn't a latin letter, capital letter ASCII code else
*/
unsigned char upper_letter(char letter){
    if (65 <= letter && letter <= 90){
        return letter;
    }
    else if (97 <= letter && letter <= 122){
        return letter - 32;
    }
    else{
        return 0;
    }
}

/*Look if the letter is in the word. If yes, returns all the position of the letter in the word. 
Stock the position in the buffer, buffer start at 0
    Parameters:
        - [char*] word: Pointer to the word
        - [char*] buffer: Buffer will contain all letter positions
        - [char] letter: simple char
        - [int] sizeof_buffer: Number of caracters in buffer               
*/
void position_letter(char* word, int* buffer, char letter, int sizeof_word) {
    int j = 0;

    for (int i = 0; i < sizeof_word; i++) {
        if (word[i] == letter) {
            buffer[j] = i + 1;
            j++;
        }
    }
}


/*Extract the word throught of the code sent by the client WHITHOUT \0 at the end
    Parameters:
        - [char*] buffer: Buffer for the extracted word
        - [char*] string: String with the code and the word sent by the client
*/
void extract_word(char* buffer, char* string){
    int i = 1;
    while (string[i] != 0){
        buffer[i-1] = string[i];
        i++;
    }
}


/*Return 1 if the buffer contain only \0 characters
    Parameters:
        - [char*] buffer: Character list
        - [int] sizeof_buffer: Number of caracters in buffer
    Return
        - [int]: 1 if buffer countain only \0 equivalent, 0 else*/
int verif_only_zero(char* buffer, int sizeof_buffer){
    for (int i = 0; i <= sizeof_buffer; i++){ // DO NOT USE STRLEN
        if (buffer[i] != 0){
            return 0;
        }
    }
    return 1;
}