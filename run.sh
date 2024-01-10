#!/bin/bash

# Compile the code
make

# Run the server in a new terminal
gnome-terminal -- bash -c "./server/server; exec bash"

# Run the client in a new terminal
gnome-terminal -- bash -c "./client/client; exec bash"