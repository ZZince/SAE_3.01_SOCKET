#include "../letter.h"
#include <string.h>

/*Return i+1 if the letter in found in word, 0 else
    Parameters:
        - [char*] word: Pointer to the word
        - [char] letter: ASCII character
    Return:
        - [int]: Letter position + 1 in char if found, 0 else
*/
int letter_in_word(char* word, char letter){
    for (int i = 0; i < strlen(word); i++){
        if (word[i] == letter){
            return i+1;
        }
    }

    return 0;
}

/*Add all unique letters found in word to buffer
    Parameters:
        - [char*] word: Pointer to the word
        - [char*] letter: Pointer to buffer
    Return:
        - [int]: Number of unique letter added to buffer
*/
int all_letters_in_word(char* word, char* buffer){
    int index_buffer = 0;
    for (int i = 0; i < strlen(word); i++){
        if (letter_in_word(buffer, word[i]) == 0 && word[i] != '-'){
            buffer[index_buffer] = word[i];
            index_buffer++;
        }
    }
    return index_buffer;
}
/*Test if the char is a valid letter and return it in capital
    Parameters:
        -[char] letter: simple char
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
