#include "ssl-encrypt.h"

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;
  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_xts(), NULL, key, NULL))
	  handleErrors();

	/* Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits */
	if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, NULL, iv))
		handleErrors();

	/* Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
  char* temp = malloc(plaintext_len);
  memcpy(temp, plaintext, plaintext_len);
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();
	ciphertext_len = len;

  if (memcmp(ciphertext, temp, plaintext_len) == 0)
  {
    printf("Plaintext and ciphertext are the same\n");
  }
  else
  {
    //printf("Plaintext and ciphertext are different\n");
  }
  
	/* Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
	ciphertext_len += len;
  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_xts(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

void generate_key(unsigned char* key)
{
  memset(key, 0, KEY_LEN/2);
  memset(key + KEY_LEN/2, 1, KEY_LEN/2);
  return;
  if(!RAND_bytes(key, KEY_LEN))
    handleErrors();
}

void generate_ID(unsigned char* ID)
{
  if(!RAND_bytes(ID, ID_LEN))
    handleErrors();
}

int encrypt_data(unsigned char* plaintext, unsigned char* key, uint64_t length, uint64_t start_data_unit_number)
{
  unsigned char iv[16];
  memcpy(iv, &start_data_unit_number, 8);
  memset(iv + 8, 0, 8);
  uint64_t processed_data_len = 0;
  int curr_length = DATA_UNIT_LEN;
  if(length % BLOCK_LEN != 0)
  {
    printf("Invalid length\n");
    return -1;
  }
  while(processed_data_len < length)
  {
    if(length - processed_data_len < DATA_UNIT_LEN)
    {
      curr_length = length - processed_data_len;
    }
    else
    {
      curr_length = DATA_UNIT_LEN;
    }
    // printf("IV:\n");
    // BIO_dump_fp (stdout, (const char *)iv, 16);
    // printf("key:\n");
    // BIO_dump_fp (stdout, (const char *)key, 64);
    int ciphertext_len = encrypt(plaintext + processed_data_len, curr_length, key, iv, plaintext + processed_data_len);
    if(ciphertext_len <= 0)
    {
      printf("Encryption failed\n");
      return -1;
    }
    processed_data_len += curr_length;
    start_data_unit_number++;
    memcpy(iv, &start_data_unit_number, 8);
    memset(iv + 8, 0, 8);
  }

  return 0;
  
}

int decrypt_data(unsigned char* ciphertext, unsigned char* key, uint64_t length, uint64_t start_data_unit_number)
{
  unsigned char iv[16];
  memcpy(iv, &start_data_unit_number, 8);
  memset(iv + 8, 0, 8);
  uint64_t processed_data_len = 0;
  int curr_length = DATA_UNIT_LEN;
  if(length % BLOCK_LEN != 0)
  {
    printf("Invalid length\n");
    return -1;
  }
  while(processed_data_len < length)
  {
    if(length - processed_data_len < DATA_UNIT_LEN)
    {
      curr_length = length - processed_data_len;
    }
    else
    {
      curr_length = DATA_UNIT_LEN;
    }

    int plaintext_len = decrypt(ciphertext + processed_data_len, curr_length, key, iv, ciphertext + processed_data_len);
    if(plaintext_len <= 0)
    {
      printf("Decryption failed\n");
      return -1;
    }
    processed_data_len += curr_length;
    start_data_unit_number++;
    memcpy(iv, &start_data_unit_number, 8);
  }

  return 0;
  
}