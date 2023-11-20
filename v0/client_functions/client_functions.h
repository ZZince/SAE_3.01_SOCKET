# include <stdbool.h> // get bool type

// connexion_client.c
int communication_socket_creation();

// character_selection.c
char select_letter();
bool is_letter(char character);

// message_client.c
int send_character_to_server(int socket, char character, int size);
char *get_message_from_server(int socket, int size);
void translate_message(unsigned char *buffer, char *word);

// game_client.c
int set_word_lenght(int number_of_letter, char *word);
void show_word_in_cmd(int number_of_letter, char *word);