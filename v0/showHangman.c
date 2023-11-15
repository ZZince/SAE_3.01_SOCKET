#include <stdio.h>

void showHangman(int errors) {

    if (errors < 0 || errors > 6) {
        perror("Erreur : Le nombre d'erreurs doit être compris entre 0 et 6");
        return;
    }

    printf(" +---+\n");
    printf(" |   |\n");

    if (errors >= 1) {
        printf(" |   O\n");
    } else {
        printf(" |    \n");
    }

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

    if (errors == 5) {
        printf(" |  /\n");
    } else if (errors >= 6) {
        printf(" |  / \\\n");
    } else {
        printf(" |    \n");
    }

    printf(" |_____\n");

}

int main() {
    int errors = 6; 

    showHangman(errors);

    return 0;
}
