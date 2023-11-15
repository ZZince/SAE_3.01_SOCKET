#include <stdio.h>
#include <stdlib.h> /* pour exit */
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> /* pour memset */
#include <netinet/in.h> /* pour struct sockaddr_in */
#include <arpa/inet.h> /* pour htons et inet_aton */
#include <unistd.h> /* pour sleep */

#define PORT IPPORT_USERRESERVED // = 5000

#define LG_MESSAGE 256 // = Taille maximale des messages de la couche Application

int main(int argc, char *argv[]){
    int socketEcoute;

    struct sockaddr_in addr_inLocal;
	socklen_t longueurAdresse;

    int socketDialogue;
	struct sockaddr_in addr_inDistant;
	char messageEnvoi[LG_MESSAGE]; /* le message de la couche Application ! */
	char messageRecu[LG_MESSAGE]; /* le message de la couche Application ! */
	int  nb; /* nb d’octets ecrits et lus */

    // Crée un socket de communication
	if ((socketEcoute = socket(AF_INET, SOCK_STREAM, 0))<0) exit(-1);

	addr_inLocal.sin_family = AF_INET;
	addr_inLocal.sin_addr.s_addr = htonl(INADDR_ANY); // toutes les interfaces locales disponibles
	addr_inLocal.sin_port = htons(PORT); // = 5000

	// On demande l’attachement local de la socket
	if((bind(socketEcoute, (struct sockaddr *)&addr_inLocal, sizeof(struct sockaddr_in))) < 0) exit(-2);

	// On fixe la taille de la file d’attente à 5 (pour les demandes de connexion non encore traitées)
	if(listen(socketEcoute, 5) < 0) exit(-3);

    // boucle d’attente de connexion : en théorie, un serveur attend indéfiniment !
	while(1){
		printf("Attente d’une demande de connexion (quitter avec Ctrl-C)\n\n");

		// c’est un appel bloquant
		if ((socketDialogue = accept(socketEcoute, (struct sockaddr *)&addr_inDistant, &longueurAdresse)) < 0) {
			  close(socketDialogue);
   			close(socketEcoute);
   			exit(-4);
		}

		printf("Connexion avec le client effectué\n");

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