#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include "../common/defs.h"
#include "../crypto/ssl-encrypt.h"

// Client acts as VeraCrypt

int main() {
    int sockfd;
    struct sockaddr_un server_addr;
    unsigned char* ID = malloc(ID_LEN);
    generate_ID(ID);

    char message[MESSAGE_LEN];
    char response[RESPONSE_LEN];
    uint64_t start_data_unit_number = 1;
    uint64_t length = DATA_LEN;

    // for(int i = 0; i < 100; i++)
    // {

        // Create a socket
        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // Set up the server address
        server_addr.sun_family = AF_UNIX;
        strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

        // Connect to the server
        if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("connect");
            exit(EXIT_FAILURE);
        }

        printf("Connected to server\n");

        char plaintext[DATA_LEN];
        char ciphertext[DATA_LEN];
        memset(plaintext, 0, MESSAGE_LEN);

        message[0] = ENCRYPT_MODE;
        memcpy(message + 1, ID, ID_LEN);
        // Store data unit number
        memcpy(message + 1 + ID_LEN, &start_data_unit_number, DATA_UNIT_NUMBER_LEN);
        // Store data length
        memcpy(message + 1 + ID_LEN + DATA_UNIT_NUMBER_LEN, &length, U_INT_64_LEN);
        
        memcpy(message + 1 + ID_LEN + DATA_UNIT_NUMBER_LEN + U_INT_64_LEN, plaintext, DATA_LEN);

        // BIO_dump_fp (stdout, (const char *)message, MESSAGE_LEN);
        
        printf("Plaintext is:\n");
        BIO_dump_fp (stdout, (const char *)plaintext, DATA_LEN);


        if (write(sockfd, message, MESSAGE_LEN) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        // Receive the ciphertext
        ssize_t num_bytes = read(sockfd, response, RESPONSE_LEN);
        if (num_bytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        memcpy(ciphertext, response, DATA_LEN);
        printf("Ciphertext is:\n");
        BIO_dump_fp (stdout, (const char *)ciphertext, DATA_LEN);

        close(sockfd);
    // }

    // memcpy(ciphertext, response, DATA_LEN);
    // printf("Ciphertext is:\n");
    // BIO_dump_fp (stdout, (const char *)ciphertext, DATA_LEN);

    // message[0] = DECRYPT_MODE;
    // memcpy(message + 1 + ID_LEN + DATA_UNIT_NUMBER_LEN + U_INT_64_LEN, ciphertext, DATA_LEN);

    // // Send the ciphertext to the external module
    // if (write(sockfd, message, MESSAGE_LEN) == -1) {
    //     perror("send");
    //     exit(EXIT_FAILURE);
    // }

    // // Receive the plaintext
    // num_bytes = read(sockfd, response, RESPONSE_LEN);
    // if (num_bytes == -1) {
    //     perror("recv");
    //     exit(EXIT_FAILURE);
    // }

    // memcpy(plaintext, response + 1, DATA_LEN);

    // printf("Plaintext is:\n");
    // BIO_dump_fp (stdout, (const char *)plaintext, DATA_LEN);
    
    // Close the socket
    close(sockfd);

    return 0;
}
