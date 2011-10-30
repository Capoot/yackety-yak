
#ifndef NETWORK_H_
#define NETWORK_H_
#endif /* NETWORK_H_ */

typedef struct {
	unsigned long int address;
	int port;
} IpAddress;

typedef enum {
	NETWORK_OK,
	HOST_NOT_FOUND,
	INVALID_ADDRESS,
	CONNECTTION_TIMEOUT
} NetworkError;

int initSockets(void);
