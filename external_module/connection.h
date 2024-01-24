#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../common/defs.h"

// Function that sets up socket connection on the server side
int setup_connection();

// Function that establishes the connection to the client on the server side
int connect_client(int server_fd);

#endif // CONNECTION_H