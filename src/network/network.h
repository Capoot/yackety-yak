#ifndef NETWORK_H_
#define NETWORK_H_

#include "../protocol/protocol.h"
#include "connection.h"

int startClient(const char*, unsigned short, Connection*);
int startServer(unsigned short, int);
void shutDownClient(void);
void shutDownServer(void);
int waitForConnection(Connection*);
int getErrorCode(void);
int sendMessage(YakMessage*, Connection*);
int receiveMessage(Connection*);

#endif /* NETWORK_H_ */
