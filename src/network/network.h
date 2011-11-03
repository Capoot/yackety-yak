#ifndef NETWORK_H_
#define NETWORK_H_

#include "../protocol/protocol.h"
#include "connection.h"

int startClient(const char*, unsigned short, Connection*);
int startServer(unsigned short, int, Connection*);
void shutDownClient(Connection*);
void shutDownServer(Connection*);
int waitForConnection(Connection*, Connection*);
int getErrorCode(void);
int sendMessage(YakMessage*, Connection*);
int receiveMessage(YakMessage*, Connection*);

#endif /* NETWORK_H_ */
