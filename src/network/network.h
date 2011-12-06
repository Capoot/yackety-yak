#ifndef NETWORK_H_
#define NETWORK_H_

#include <windows.h>
#include "../protocol/protocol.h"

#define IP_MASK1 0xff000000
#define IP_MASK2 0x00ff0000
#define IP_MASK3 0x0000ff00
#define IP_MASK4 0x000000ff

char* serializeMessage(YakMessage*, int);
YakHeader* deSerializeHeader(char*, int);
int sendMessage(YakMessage*, unsigned int, SOCKADDR_IN*);
int getMessage(YakMessage*, SOCKET, struct timeval, SOCKADDR_IN*);
int ipToString(unsigned long, char*);

enum network_errors {
	WSA_OK = 0,
	WSA_STARTUP_FAIL = 101,
	WSA_CREATE_SOCKET_FAIL = 102,
	WSA_BIND_FAIL = 103,
	WSA_SOCKET_ERROR = 104
};

#endif /* NETWORK_H_ */
