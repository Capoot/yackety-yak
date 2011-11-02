#ifndef SERVER_H_
#define SERVER_H_
#endif

typedef enum {
	SERVER_OK = 0,
	SOCKET_BIND_ERROR = -201,
	SOCKET_LISTEN_ERROR = -202,
	SOCKET_CREATE_ERROR = -203,
	SOCKET_ACCEPT_ERROR = -204,
	LISTEN_PORT_NOT_SPECIFIED = -205,
	MAX_CONNECTIONS_NOT_SPECIFIED = -206
} ServerError;

typedef struct {
	unsigned short listenPort;
	int maxConnections;
} ServerSettings;
