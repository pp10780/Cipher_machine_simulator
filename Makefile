# Compiler options
CC := gcc
CFLAGS := -Wall -Wextra

# Debug flag
ifdef deb
CFLAGS += -g
endif

# OpenSSL library
OPENSSL_LIB := -lssl -lcrypto

# Source files
CLIENT_SRC := client/client.c crypto/ssl-encrypt.c
SERVER_SRC := server/server.c server/connection.c server/container_list.c crypto/ssl-encrypt.c

# Object files
CLIENT_OBJ := $(CLIENT_SRC:.c=.o)
SERVER_OBJ := $(SERVER_SRC:.c=.o)

# Executables
CLIENT := client/client
SERVER := server/server

all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(OPENSSL_LIB)

$(SERVER): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(OPENSSL_LIB)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(OPENSSL_LIB)

deb:
	@echo "Building with debug symbols..."
	$(MAKE) all deb=1

clean:
	rm -f $(CLIENT) $(SERVER) $(CLIENT_OBJ) $(SERVER_OBJ)