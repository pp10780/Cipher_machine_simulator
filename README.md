## Rules

# SERVER POV
1. The first byte specifies if the incoming packet should be encrypted ('0') or decrypted ('1')
2. The second byte contains the ID of the container
3. The ID 0 is saved from new containers don't have an ID yet.

# CLIENT POV
1. The first byte of the incoming packet specifies the ID of the client's container
2. The following bytes correspond to the rest of the data of the container


## Next Steps

1. Change the size of the buffer to a bigger value so that it can afford every type of buffer requests and use the return from read (number of bytes) to do the following operations

In the real implementation the length and the startdataunitNo must both be sent in a first connection

In this test implementation the buffer is being received and the length is received as it is always the length of the message