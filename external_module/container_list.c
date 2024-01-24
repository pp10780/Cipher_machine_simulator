#include "container_list.h"

void create_new_container(container* new_container, unsigned char* ID)
{
    memcpy(new_container->ID, ID, ID_LEN);
    generate_key(new_container->key);
    new_container->next = NULL;
}

container *add_container(container *container_list, unsigned char* ID) {
    container *current = container_list;

    // Allocate memory for a new node
    container *new_node = malloc(sizeof(container));
    if (new_node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    create_new_container(new_node, ID);
    new_node->next = NULL;

    // If the list is empty, set the new node as the head
    if (container_list == NULL) {
        container_list = new_node;
    } else {
        // Traverse to the end of the list and append the new node
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    return container_list; // Return the updated list
}

container *get_container_info(container *container_list, unsigned char* ID)
{
    container *current = container_list;
    while(current != NULL)
    {
        if(memcmp(current->ID, ID, ID_LEN) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_container_list(container *container_list)
{
    container *current = container_list;
    container *next = NULL;
    while(current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}