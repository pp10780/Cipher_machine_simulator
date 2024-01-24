#include "connection.h"
#include "../common/defs.h"
#include "container_list.h"
#include "../crypto/ssl-encrypt.h"

#include <signal.h>

// Server acts as the external module

int server_fd, client_fd;

container *container_list = NULL;

void INThandler(int sig)
{
    signal(sig, SIG_IGN);
    printf("\nClosing connection\n");
    close(client_fd);
    close(server_fd);
    free_container_list(container_list);
    unlink(SOCKET_PATH);
    /* Clean up */
    EVP_cleanup();
    ERR_free_strings();
    exit(0);
}

int main() 
{
    signal(SIGINT, INThandler);
    container *container = NULL;

    unsigned char response[RESPONSE_LEN];

    unsigned char buffer[MESSAGE_LEN];

    uint64_t start_data_unit_number;

    uint64_t length;

    int total_bytes = 0;
    
    // Set up the connection
    server_fd = setup_connection();

    // Connect to the client
    client_fd = connect_client(server_fd);

    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    while(1)
    {
        // printf("\nWaiting for message...\n");
        ssize_t num_bytes = read(client_fd, buffer, MESSAGE_LEN);
        // printf("Received %ld\n", num_bytes);
        if (num_bytes == -1) {
            //perror("read");
            // client_fd = connect_client(server_fd);
            continue;
        }
        else if (num_bytes == 0) {
            //printf("Client disconnected\n");
            client_fd = connect_client(server_fd);
            continue;
        }

        length = *((uint64_t*)(buffer + 1 + ID_LEN + DATA_UNIT_NUMBER_LEN));
        while((uint64_t) num_bytes < (length + 1 + ID_LEN + DATA_UNIT_NUMBER_LEN + U_INT_64_LEN))
        {
            num_bytes += read(client_fd, buffer + num_bytes, length - num_bytes + 1 + ID_LEN + DATA_UNIT_NUMBER_LEN + U_INT_64_LEN);
        }

        // Get ID from message
        unsigned char ID[ID_LEN];
        memcpy(ID, buffer + 1, ID_LEN);
        container = get_container_info(container_list, ID);
        if(container == NULL)
        {
            // printf("Container not Found! \nAdding new key...\n");
            container_list = add_container(container_list, ID);
            container = get_container_info(container_list, ID);
        }
        else
        {
            // printf("Container found!\n");
        }

        start_data_unit_number = *((uint64_t*)(buffer + 1 + ID_LEN));
        memcpy(response, buffer + 1 + ID_LEN + DATA_UNIT_NUMBER_LEN + U_INT_64_LEN, length);
        // printf("Data unit number: %lu\n", start_data_unit_number);
        // printf("Data length: %lu\n", length);

        if(buffer[0] == ENCRYPT_MODE)
        {
            total_bytes += num_bytes;
            printf("Encrypting\n");
            if(encrypt_data(response, container->key, length, start_data_unit_number) < 0)
            {
                printf("Encryption failed\n");
                continue;
            }
            // printf("Total bytes: %d\n", total_bytes);
        }
        else if(buffer[0] == DECRYPT_MODE)
        {
            printf("Decrypting\n");
            if(decrypt_data(response, container->key, length, start_data_unit_number) < 0)
            {
                printf("Decryption failed\n");
                continue;
            }
        }
        else
        {
            printf("Invalid mode\n");
            continue;
        }

        num_bytes = write(client_fd, response, length);
        if (num_bytes == -1) {
            perror("write");
            continue;
        }
        while ((uint64_t) num_bytes < length)
        {
            num_bytes += write(client_fd, response + num_bytes, length - num_bytes);
        }
        
        printf("Sent %ld\n", num_bytes);
    }

    printf("Closing connection\n");

    // Close the client and server sockets
    close(client_fd);
    close(server_fd);

    free_container_list(container_list);

    // Remove the socket file
    unlink(SOCKET_PATH);

    /* Clean up */
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
