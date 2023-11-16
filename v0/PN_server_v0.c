#include <stdio.h>
#include <stdlib.h> /* pour exit */
#include <string.h> /* pour memset */
#include <unistd.h> /* pour sleep */

#define PORT IPPORT_USERRESERVED // = 5000

#define LG_MESSAGE 256 // = Taille maximale des messages de la couche Application

int main(int argc, char *argv[]){

    int socketDialogue;
	char messageEnvoi[LG_MESSAGE]; /* le message de la couche Application ! */
	char messageRecu[LG_MESSAGE]; /* le message de la couche Application ! */
	int  nb; /* nb d’octets ecrits et lus */

    // boucle d’attente de connexion : en théorie, un serveur attend indéfiniment !
	while(1){
		printf("Attente d’une demande de connexion (quitter avec Ctrl-C)\n\n");

		

		// Initialise à 0 la structure sockaddr_in
		memset(&messageRecu, 0x00, LG_MESSAGE);

    // On réception les données du client (cf. protocole)
    nb = read(socketDialogue, messageRecu, LG_MESSAGE*sizeof(char)); // ici appel bloquant
    switch(nb) {
        case -1 : /* une erreur ! */
                perror("read");
                close(socketDialogue);
                exit(-5);
        case 0  : /* la socket est fermée */
                fprintf(stderr, "La socket a été fermée par le client !\n\n");
                close(socketDialogue);
                return 0;
        default:  /* réception de n octets */
                printf("Message reçu : %s (%d octets)\n\n", messageRecu, nb);
	}

	// Ecriture du message à envoyer au client
	sprintf(messageEnvoi, "Hello client! Veux-tu jouer au pendu avec moi ?\n");

	// On envoie les données vers le client
	nb = write(socketDialogue, messageEnvoi, strlen(messageEnvoi));
		switch(nb){
			case -1 : /* une erreur ! */
				  perror("write");
   				  close(socketDialogue);
   				  exit(-6);
			case 0 :  /* la socket est fermée */
				  fprintf(stderr, "La socket a été fermée par le client !\n\n");
				  close(socketDialogue);
				  return 0;
			default:  /* envoi de n octets */
   				  printf("Message %s envoyé (%d octets)\n\n", messageEnvoi, nb);
				  // On ferme la socket de dialogue et on se replace en attente ...
   				  close(socketDialogue);
		}

	}


	// On ferme la ressource avant de quitter
   	close(socketEcoute);
    return 0;
}