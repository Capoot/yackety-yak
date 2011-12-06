#ifndef CLIENT_H_
#define CLIENT_H_
#endif

#include <windows.h>

typedef enum { // TODO rework this... most of it isnt needed
	CLIENT_OK = 0,
	NAME_NOT_SPECIFIED = -101,
	REMOTE_ADDRESS_NOT_SPECIFIED = -102,
	PORT_NOT_SPECIFIED = -103,
	NO_ARGS_SPECIFIED = -104,
	CONNECTION_ERROR = -105,
	NETWORK_ERROR = -106,
	CREATE_SOCKET_ERROR = -107,
	CONNECT_SOCKET_ERROR = -108,
	INIT_SOCKET_ERROR = -109
} ClientError;

typedef struct {
	char* serverIp;
	unsigned short clientPort;
	unsigned short serverPort;
	char* userName;
	/** timeout for select statement */
	struct timeval timeout;
} ClientSettings;

enum client_errors {
	UNRECOGNIZED_MESSAGE = 200,
	INVALID_RESPONSE = 201,
	CONNECTION_REQUEST_FAIL = 202,
	REJECTED_SERVER_FULL = 203,
	REJECTED_WRONG_PASSWORD = 204,
	REJECTED_NAME_TAKEN = 205
};

void runClient(ClientSettings*);
