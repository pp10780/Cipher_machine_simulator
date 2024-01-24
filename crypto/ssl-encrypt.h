#ifndef SSL_ENCRYPT_H
#define SSL_ENCRYPT_H

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string.h>
#include "../common/defs.h"

#define KEY_LEN 64

void handleErrors(void);

// Function to encrypt plaintext using key and iv and store it in ciphertext using the AEX-XTS encryption mode
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext);

// Function to decrypt ciphertext using key and iv and store it in plaintext using the AEX-XTS encryption mode
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext);

// Function to generate a random key
void generate_key(unsigned char* key);

// Function to generate a random ID (client tests)
void generate_ID(unsigned char* ID);

// Function to encrypt data according to the AEX-XTS encryption mode implemented by Veracrypt
int encrypt_data(unsigned char* plaintext, unsigned char* key, uint64_t length, uint64_t start_data_unit_number);

// Function to decrypt data according to the AEX-XTS encryption mode implemented by Veracrypt
int decrypt_data(unsigned char* ciphertext, unsigned char* key, uint64_t length, uint64_t start_data_unit_number);

#endif // SSL_ENCRYPT_H