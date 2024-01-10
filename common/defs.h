#define SOCKET_PATH "/tmp/my_socket"

#define DATA_LEN 1000000

#define ID_LEN 64

#define DATA_UNIT_NUMBER_LEN 8

#define U_INT_64_LEN 8

#define DATA_UNIT_LEN 512

#define BLOCK_LEN 16

#define IV_LEN 16

#define MESSAGE_LEN 1 + ID_LEN + DATA_UNIT_NUMBER_LEN + U_INT_64_LEN + DATA_LEN

#define RESPONSE_LEN DATA_LEN

#define ENCRYPT_MODE '0'

#define DECRYPT_MODE '1'