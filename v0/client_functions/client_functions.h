# include <stdbool.h> // get bool type

// show_hangman.c
void show_hangman(int errors);

// connexion_client.c
int communication_socket_creation();

// character_selection.c
char select_letter();
bool is_letter(char character);

// message_client.c
int send_character_to_server(int socket, char character, int size);
char *get_message_from_server(int socket, int size);
void translate_message(unsigned char *buffer, char *word, char character);

// game_client.c
int set_word_lenght(int number_of_letter, char *word);
void show_word_in_cmd(char *word);
void character_is_good(char *buffer, char *word, char character);