#include <windows.h>

#include "../protocol/protocol.h"
#include "../client/client.h"
#include "../server/server.h"

#define SOCKET_IMPL "wsock32"

/******** CLIENT GLOBALS **********/

SOCKET* CLIENT_SOCKET = NULL;
SOCKADDR_IN* CLIENT_TARGET_ADDRESS = NULL;

/******** SERVER GLOBALS **********/

SOCKET* ACCEPT_SOCKET = NULL;

/******** PRIVATE *********/

int createSocket(SOCKET* s) {
	*s = socket(AF_INET, SOCK_STREAM, 0);
	if(*s == INVALID_SOCKET) {
		return -1;
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

int initSockets(void) {
	WSADATA wsa;
	int code = WSAStartup(MAKEWORD(2,0), &wsa);
	if(code == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}

/******** PUBLIC *********/

int sendMessage(YakMessage* msg) {
	return 0;
}

YakMessage* receiveMessage(void) {
	return NULL;
}

ClientError startClient(const char* ip, unsigned short port) {

	int code;
	code = initSockets();
	if(code != 0) {
		return INIT_SOCKET_ERROR;
	}

	CLIENT_TARGET_ADDRESS = createIpAddress(ip, port); // TODO addresse aufräumen (global?)
	CLIENT_SOCKET = (SOCKET*)malloc(sizeof(SOCKET));

	code = createSocket(CLIENT_SOCKET);
	if(code != 0) {
		return CREATE_SOCKET_ERROR;
	}

	code = connect(*CLIENT_SOCKET, (SOCKADDR*)CLIENT_TARGET_ADDRESS, sizeof(SOCKADDR));
	if(code == SOCKET_ERROR) {
		return CONNECT_SOCKET_ERROR;
	}

	return CLIENT_OK;
}

void shutDownClient(void) {
	if(CLIENT_SOCKET != NULL) {
		closesocket(*CLIENT_SOCKET);
		free(CLIENT_SOCKET);
	}
	if(CLIENT_TARGET_ADDRESS != NULL) {
		free(CLIENT_TARGET_ADDRESS);
	}
	WSACleanup();
}

ServerError startServer(unsigned short port, int maxConnections) {

	ACCEPT_SOCKET = (SOCKET*)malloc(sizeof(SOCKET));
	SOCKADDR_IN addr;

	if(initSockets() != 0) {
		return SERVER_INIT_SOCKET_ERROR;
	}

	int error = createSocket(ACCEPT_SOCKET);
	if(error != 0) {
		return SOCKET_CREATE_ERROR;
	}

	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ADDR_ANY;

	error = bind(*ACCEPT_SOCKET, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if(error == SOCKET_ERROR) {
		return SOCKET_BIND_ERROR;
	}

	error = listen(*ACCEPT_SOCKET, maxConnections);
	if(error == SOCKET_ERROR) {
		return SOCKET_LISTEN_ERROR;
	}

	return SERVER_OK;
}

ServerError waitForConnection(void) {
	SOCKET s = accept(*ACCEPT_SOCKET, NULL, NULL);
	if(s == INVALID_SOCKET) {
		return SOCKET_ACCEPT_ERROR;
	}
	return SERVER_OK;
}

void shutDownServer(void) {
	if(ACCEPT_SOCKET != NULL) {
		closesocket(*ACCEPT_SOCKET);
		free(ACCEPT_SOCKET);
	}
	WSACleanup();
}

int getErrorCode(void) {
	return WSAGetLastError();
}
