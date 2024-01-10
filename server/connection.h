#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../common/defs.h"

int setup_connection();

int connect_client(int server_fd);

#endif // CONNECTION_H