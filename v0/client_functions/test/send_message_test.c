/*

    // Saisie du message
	printf("Votre message (%d caractères au maximum) ? \n",LG_MESSAGE);
	fgets(buffer,LG_MESSAGE,stdin);

 	// Envoi du message
	switch(nb = write(socket, buffer, strlen(buffer))){
		case -1 : // une erreur ! 
     	perror("Erreur en écriture...");
     	close(socket);
     	exit(-3);
		case 0 : // la socket est fermée
			fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			return 0;
		default: // envoi de n octets
			printf("Message %s envoyé! (%d octets)\n\n", buffer, nb);
	}

	// Reception des données du serveur
	switch(nb = read(socket, buffer, LG_MESSAGE)) {
		case -1 : // une erreur !
			perror("Erreur de lecture...");
			close(socket);
			exit(-4);
		case 0 : // la socket est fermée
   		fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			return 0;
		default: // réception de n octets
		  buffer[nb]='\0';
			printf("Message reçu du serveur : %s (%d octets)\n\n", buffer, nb);
	}

*/