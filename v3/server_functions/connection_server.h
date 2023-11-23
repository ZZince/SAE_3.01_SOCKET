int socket_creation(const int port, const int max_connection);
int client_acceptation(const int listen_socket);
int wait_for_two_clients(int listen_socket, int* socket_client1, int* socket_client2);