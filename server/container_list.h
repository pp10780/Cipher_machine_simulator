#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../crypto/ssl-encrypt.h"

typedef struct container_t
{
    unsigned char ID[ID_LEN];
    unsigned char key[KEY_LEN];
    struct container_t *next;

} container;

void create_new_container(container* new_container, unsigned char* ID);

container *add_container(container *container_list, unsigned char* ID) ;

container *get_container_info(container *container_list, unsigned char* ID);

void free_container_list(container *container_list);

#endif //CONTAINER_LIST_H