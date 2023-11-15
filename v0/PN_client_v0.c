#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LG_MESSAGE 256 // = Taille maximale des messages

int main(int argc, char *argv[]){
	int descripteurSocket;
	struct sockaddr_in sockaddrDistant;
	socklen_t longueurAdresse;

	char buffer[LG_MESSAGE];
	int nb; /* nb d’octets ecrits et lus */

	char ip_dest[16];
	int  port_dest;

    if (argc>1) {
		strncpy(ip_dest,argv[1],16);
		sscanf(argv[2],"%d",&port_dest);
	}else{
		printf("USAGE : %s ip port\n",argv[0]);
		exit(-1);
	}

    // Crée un socket de communication
	descripteurSocket = socket(AF_INET, SOCK_STREAM, 0);

	// Teste la valeur renvoyée par l’appel système socket()
	if(descripteurSocket < 0){
		perror("Erreur en création de la socket..."); // Affiche le message d’erreur
		exit(-1); // On sort en indiquant un code erreur
	}

	printf("Socket créée! (%d)\n", descripteurSocket);

	// Obtient la longueur en octets de la structure sockaddr_in
	longueurAdresse = sizeof(sockaddrDistant);

	// Initialise à 0 la structure sockaddr_in
	memset(&sockaddrDistant, 0x00, longueurAdresse);

	// Renseigne la structure sockaddr_in avec les informations du serveur distant
	sockaddrDistant.sin_family = AF_INET;

	// On choisit le numéro de port d’écoute du serveur
	sockaddrDistant.sin_port = htons(port_dest);

	// On choisit l’adresse IPv4 du serveur
	inet_aton(ip_dest, &sockaddrDistant.sin_addr);

	// Débute la connexion vers le processus serveur distant
	if((connect(descripteurSocket, (struct sockaddr *)&sockaddrDistant,longueurAdresse)) == -1){
		perror("Erreur de connection avec le serveur distant...");
		close(descripteurSocket);
		exit(-2); // On sort en indiquant un code erreur
	}

	printf("Connexion au serveur %s:%d réussie!\n",ip_dest,port_dest);

	// Saisie du message
	printf("Votre message (%d caractères au maximum) ? \n",LG_MESSAGE);
	fgets(buffer,LG_MESSAGE,stdin);

 	// Envoi du message
	switch(nb = write(descripteurSocket, buffer, strlen(buffer))){
		case -1 : /* une erreur ! */
     	perror("Erreur en écriture...");
     	close(descripteurSocket);
     	exit(-3);
		case 0 : /* la socket est fermée */
			fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			return 0;
		default: /* envoi de n octets */
			printf("Message %s envoyé! (%d octets)\n\n", buffer, nb);
	}

	/* Reception des données du serveur */
	switch(nb = read(descripteurSocket, buffer, LG_MESSAGE)) {
		case -1 : /* une erreur ! */
			perror("Erreur de lecture...");
			close(descripteurSocket);
			exit(-4);
		case 0 : /* la socket est fermée */
   		fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			return 0;
		default: /* réception de n octets */
		  buffer[nb]='\0';
			printf("Message reçu du serveur : %s (%d octets)\n\n", buffer, nb);
	}

	// On ferme la ressource avant de quitter
	close(descripteurSocket);

	return 0;
}