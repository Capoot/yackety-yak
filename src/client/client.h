#ifndef CLIENT_H_
#define CLIENT_H_
#endif

#include "..\network\network.h"

typedef struct {
	IpAddress remoteAddress;
	char* name;
} ClientSettings;

typedef enum {
	OK = 0,
	NAME_NOT_SPECIFIED = -1,
	REMOTE_ADDRESS_NOT_SPECIFIED = -2,
	PORT_NOT_SPECIFIED = -3
} ClientError;
