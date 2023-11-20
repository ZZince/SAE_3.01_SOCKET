# include <stdbool.h> // get bool type

// connexion_client.c
int communication_socket_creation();

// character_selection.c
char select_letter();
bool is_letter(char character);

// send_message.c
int send_character_to_server(int socket, char character, int size);