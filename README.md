# Secure Server-Client Communication

This project implements a secure server-client communication system using OpenSSL for encryption and decryption.

## Project Structure

The project is divided into three main directories:

- `client/`: Contains the source code, object file, and executable for the client.
- `server/`: Contains the source code, object files, and executable for the server. This includes the main server code as well as additional modules for handling connections and a container list.
- `crypto/`: Contains the source code, object file, and header for the OpenSSL encryption and decryption functions.

The root directory also contains a `Makefile` for building the project, a `run.sh` script for compiling and running the server and client, and a `.vscode/` directory with configuration files for Visual Studio Code.

## Building the Project

To build the project, run the following command in the root directory:

```sh
make
```
This will compile the source code and create the executables in the client/ and server/ directories.

# Debugging

To build the project with debug symbols, use the `deb´ target in the Makefile:
```sh
make deb
```