#ifndef CONNECTION_H_
#define CONNECTION_H_

#define CONN_LOST (-1)
#define CONN_ERROR (-2)

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
