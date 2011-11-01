
#ifndef NETWORK_H_
#define NETWORK_H_
#endif /* NETWORK_H_ */

int initSockets(void);
void cleanupSockets(void);
int connectToServer(const char*, unsigned short);
int disconnectFromServer(void);
