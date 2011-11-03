#ifndef CONNECTION_H_
#define CONNECTION_H_

typedef enum {
	CLIENT,
	SERVER
} ConnectionType;

typedef struct {
	unsigned int socketId;
	char* userName;
	ConnectionType type;
} Connection;

#endif
