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

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext);

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext);

void generate_key(unsigned char* key);

void generate_ID(unsigned char* ID);

int encrypt_data(unsigned char* plaintext, unsigned char* key, uint64_t length, uint64_t start_data_unit_number);

int decrypt_data(unsigned char* ciphertext, unsigned char* key, uint64_t length, uint64_t start_data_unit_number);

#endif // SSL_ENCRYPT_H