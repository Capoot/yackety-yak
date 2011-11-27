#ifndef NETWORK_H_
#define NETWORK_H_

#include <windows.h>
#include "../protocol/protocol.h"

char* serializeMessage(YakMessage*, int);
YakHeader* deSerializeHeader(char*, int);
int sendMessage(YakMessage*, unsigned int, SOCKADDR_IN*);
int getMessage(YakMessage*, SOCKET, struct timeval, SOCKADDR_IN*);

enum network_errors {
	WSA_OK = 0,
	WSA_STARTUP_FAIL = 101,
	WSA_CREATE_SOCKET_FAIL = 102,
	WSA_BIND_FAIL = 103,
	WSA_SOCKET_ERROR = 104
};

#endif /* NETWORK_H_ */
