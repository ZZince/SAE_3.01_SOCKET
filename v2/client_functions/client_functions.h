# include <stdbool.h> // get bool type

// show_hangman.c
void show_hangman(int errors);

// connexion_client.c
int communication_socket_creation();

// character_selection.c
char select_letter();
bool is_not_letter(char character);
char *select_word();

// message_client.c
int send_character_to_server(int socket, char character, int size);
int send_word_to_server(int socket, char *word, int size);
int send_custom_word_to_server(int socket, char *word, int size);
char *get_message_from_server(int socket, int size);
int translate_message(unsigned char *buffer, char *word, char character);

// game_client.c
int set_word_lenght(int number_of_letter, char *word);
void show_word_in_cmd(char *word);
void character_is_good(unsigned char *buffer, unsigned char *word, char character);