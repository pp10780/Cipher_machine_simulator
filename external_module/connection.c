#include "connection.h"

int setup_connection() {
    
    int server_fd;
    struct sockaddr_un server_addr;

    // Create a socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Bind the socket to the server address
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // printf("Server is listening on %s\n", SOCKET_PATH);

    return server_fd;
}

int connect_client(int server_fd) {
    int client_fd;
    struct sockaddr_un client_addr;
    socklen_t client_len;

    // Accept incoming connections
    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // printf("Client connected\n");

    return client_fd;
}