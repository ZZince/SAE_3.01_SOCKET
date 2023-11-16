#include <stdio.h>

// Function to display the hangman based on the number of errors
void showHangman(int errors) {
    
    // Check if the number of errors is outside the valid range
    if (errors < 0 || errors > 6) {
        perror("Error: Number must be between 0 and 6");
        return;
    }

    // Display the basic structure of the hangman
    printf(" +---+\n");
    printf(" |   |\n");

    // Display the hangman's head if at least 1 error
    if (errors >= 1) {
        printf(" |   O\n");
    } else {
        printf(" |    \n");
    }

    // Display the left arm if 2 errors, the right arm if there are 3 errors,
    // and both arms if there are 4 errors or more
    if (errors == 2) {
        printf(" |   |\n");
    } else if (errors == 3) {
        printf(" |  /| \n");
    }
    else if (errors >= 4) {
        printf(" |  /|\\\n");
    } else {
        printf(" |    \n");
    }

    // Display the left leg if 5 errors, both legs if there are 6 errors or more
    if (errors == 5) {
        printf(" |  /\n");
    } else if (errors >= 6) {
        printf(" |  / \\\n");
    } else {
        printf(" |    \n");
    }

    // Display the base of the hangman
    printf(" |_____\n");
}

int main() {
    int errors = 6; 

    showHangman(errors);

    return 0;
}
