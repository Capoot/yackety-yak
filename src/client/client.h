#ifndef CLIENT_H_
#define CLIENT_H_
#endif

#include "../network/network.h"

typedef enum {
	CLIENT_OK = 0,
	NAME_NOT_SPECIFIED = -1,
	REMOTE_ADDRESS_NOT_SPECIFIED = -2,
	PORT_NOT_SPECIFIED = -3,
	NO_ARGS_SPECIFIED = -4,
	CONNECTION_ERROR = -5
} ClientError;

typedef struct {
	IpAddress remoteAddress;
	char* name;
} ClientSettings;

typedef enum {
	CHAT,
	WHISPER,
	QUIT
} ActionType;

typedef struct {
	ActionType type;
	char** params;
} Action;

ClientError runClient(ClientSettings*);
