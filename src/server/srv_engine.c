#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "server.h"
#include "../protocol/protocol.h"
#include "../network/network.h"
#include "../util/util.h"

typedef struct {
	SOCKET socket;
	ServerSettings settings;
	unsigned short running;
} YakServer;

int startServer(YakServer* server) {

	WSADATA wsa;
	int code = WSAStartup(MAKEWORD(2,0), &wsa);
	if(code == SOCKET_ERROR) {
		return WSA_STARTUP_FAIL;
	}

	server->socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(server->socket == INVALID_SOCKET) {
		return WSA_CREATE_SOCKET_FAIL;
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server->settings.listenPort);
	addr.sin_addr.s_addr = ADDR_ANY;
	code = bind(server->socket, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if(code == SOCKET_ERROR) {
		return WSA_BIND_FAIL;
	}

	server->running = 1;
	return WSA_OK;
}

void stopServer(YakServer* server) {
	printf("Stopping server... ");
	closesocket(server->socket);
	WSACleanup();
	printf("done!");
}

int getMessage(YakMessage* msg, YakServer* server) {

	FD_SET fdset;
	FD_ZERO(&fdset);
	FD_SET(server->socket, &fdset);
	struct timeval timeout = server->settings.timeout;

	int code = select(0, &fdset, NULL, NULL, &timeout);
	if(code == 0) {
		// timeout - socket not ready yet
		return 0;
	}else if(code == SOCKET_ERROR) {
		return code;
	}

	if(!FD_ISSET(server->socket, &fdset)) {
		return 0;
	}

	SOCKADDR_IN remoteAddr;
	int size = sizeof(remoteAddr);

	// TODO do something useful with remote addy
	return receiveMessage(msg, server->socket, &remoteAddr, &size);
}

int handleHelloMessage(YakMessage* msg) {

	char* name;
	char* password;
	readHelloParams(msg, &name, &password);

	// TODO hello msg...
	printf("received hello from %s // %s...\n", name, password);

	return 0;
}

void dispatchMessage(YakMessage* msg, YakServer* server) {

	int error = 0;
	switch(msg->header.type) {
	case HELLO: {
		error = handleHelloMessage(msg);
		break;
	}
	default: {
		// unrecognized type of message: ignored...
		printf("discarding message of unknown type %d\n", msg->header.type);
	}
	}

	if(error != 0) {
		// TODO error...
	}
}

void serverLoop(YakServer* server) {

	int code;
	YakMessage msg;

	code = getMessage(&msg, server);
	if(code == 0) {
		// no message received. no need to handle.
		return;
	} else if(code == SOCKET_ERROR) {
		printError(WSA_SOCKET_ERROR);
		server->running = 0;
		return;
	}

	dispatchMessage(&msg, server);
	if(msg.header.dataSize > 0) {
		free(msg.data);
	}
}

void runServer(ServerSettings* settings) {

	YakServer server;
	server.settings = *settings;

	printf("Launching Yak server at port %d... ", settings->listenPort);
	int error = startServer(&server);
	if(error != 0) {
		printError(error);
		stopServer(&server);
		exit(error);
	} else {
		printf("success!\n");
	}

	printf("Waiting for transmission... ");
	while(server.running) {
		serverLoop(&server);
	}

	stopServer(&server);
}

