#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../crypto/ssl-encrypt.h"

// Struct that where the ID and encrypttion key of a container are stored
typedef struct container_t
{
    unsigned char ID[ID_LEN];
    unsigned char key[KEY_LEN];
    struct container_t *next;

} container;

// Function that creates a new container
void create_new_container(container* new_container, unsigned char* ID);

// Function that adds a container to the container list
container *add_container(container *container_list, unsigned char* ID) ;

// Function that removes a container from the container list
container *get_container_info(container *container_list, unsigned char* ID);

// Function that removes the containers from a list
void free_container_list(container *container_list);

#endif //CONTAINER_LIST_H