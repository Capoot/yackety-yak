#ifndef CONNECTION_H_
#define CONNECTION_H_

typedef enum {
	CLIENT,
	SERVER,
	ACCEPT
} ConnectionType;

typedef struct {
	unsigned int socketId;
	char* userName;
	ConnectionType type;
	char* address;
	unsigned short port;
} Connection;

#endif
