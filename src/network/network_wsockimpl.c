#include <windows.h>
#include "../protocol/protocol.h"
#include "../client/client.h"

#include <stdio.h> // FIXME DEBUG

#define SOCKET_IMPL "wsock32"

/******** GLOBALS **********/

SOCKET* CLIENT_SOCKET = NULL;
SOCKADDR_IN* REMOTE_ADDRESS = NULL;

/******** PRIVATE *********/

int createClientSocket(SOCKET* s) {
	*s = socket(AF_INET, SOCK_STREAM, 0);
	int code = WSAGetLastError();
	if(code != 0) {
		return code;
	}
	return 0;
}

SOCKADDR_IN* createIpAddress(const char* ip, unsigned short port) {
	SOCKADDR_IN* addr = (SOCKADDR_IN*)malloc(sizeof(SOCKADDR_IN));
	memset(addr, 0, sizeof(SOCKADDR_IN));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = inet_addr(ip);
	return addr;
}

/******** PUBLIC *********/

ClientError initSockets(void) {
	WSADATA wsa;
	int code = WSAStartup(MAKEWORD(2,0), &wsa);
	if(code == SOCKET_ERROR) {
		return INIT_SOCKET_ERROR;
	}
	return CLIENT_OK;
}

void cleanupSockets(void) {
	if(CLIENT_SOCKET != NULL) {
		free(CLIENT_SOCKET);
	}
	if(REMOTE_ADDRESS != NULL) {
		free(REMOTE_ADDRESS);
	}
	WSACleanup();
}

int sendMessage(YakMessage* msg) {
	return 0;
}

YakMessage* receiveMessage(void) {
	return NULL;
}

ClientError connectToServer(const char* ip, unsigned short port) {

	int code;
	REMOTE_ADDRESS = createIpAddress(ip, port); // TODO addresse aufräumen (global?)
	CLIENT_SOCKET = (SOCKET*)malloc(sizeof(SOCKET));

	code = createClientSocket(CLIENT_SOCKET);
	if(code != 0) {
		return CREATE_SOCKET_ERROR;
	}

	code = connect(*CLIENT_SOCKET, (SOCKADDR*)REMOTE_ADDRESS, sizeof(SOCKADDR));
	if(code != 0) {
		return CONNECT_SOCKET_ERROR;
	}
	return CLIENT_OK;
}

ClientError disconnectFromServer(void) {
	int code = closesocket(*CLIENT_SOCKET);
	if(code != 0) {
		return NETWORK_ERROR;
	}
	return CLIENT_OK;
}
