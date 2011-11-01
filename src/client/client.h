#ifndef CLIENT_H_
#define CLIENT_H_
#endif

typedef enum {
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
	unsigned short serverPort;
	char* userName;
} ClientSettings;

ClientError runClient(ClientSettings*);
