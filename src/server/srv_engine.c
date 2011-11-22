#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "server.h"
#include "../protocol/protocol.h"
#include "../network/network.h"
#include "../util/util.h"

typedef struct {
	char* userName;
	SOCKADDR_IN address;
	unsigned long lastMessage;
} ClientSession;

typedef struct {
	SOCKET socket;
	ServerSettings settings;
	unsigned char running;
	ClientSession* clients;
	int numClients;
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
	closesocket(server->socket);
	WSACleanup();
}

int getMessage(YakMessage* msg, YakServer* server, SOCKADDR_IN* remoteAddr) {

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
		// nothing to read
		return 0;
	}

	int size = sizeof(remoteAddr);
	code = receiveMessage(msg, server->socket, remoteAddr, &size);
	return code;
}

int handleHelloMessage(YakMessage* msg, SOCKADDR_IN* remoteAddr) {

	char* name;
	char* password;
	readHelloParams(msg, &name, &password);

	// TODO hello msg...
	// TODO if accepted, save address
	printf("received hello from %s // %s...\n", name, password);

	return 0;
}

void dispatchMessage(YakMessage* msg, YakServer* server, SOCKADDR_IN* remoteAddr) {

	int error = 0;
	switch(msg->header.type) {
	case HELLO: {
		error = handleHelloMessage(msg, remoteAddr);
		break;
	}
	// TODO proof of live...
	default: {
		// unrecognized type of message: ignored...
		printf("discarding message of unknown type %d\n", msg->header.type);
	}
	}

	if(error != 0) {
		// TODO error... (probably non-fatal)
	}
}

void serverLoop(YakServer* server) {

	int code;
	YakMessage msg;
	SOCKADDR_IN remoteAddr;

	code = getMessage(&msg, server, &remoteAddr);
	if(code == 0) {
		// no message received. no need to handle.
		return;
	} else if(code == SOCKET_ERROR) {
		printError(WSA_SOCKET_ERROR);
		server->running = 0;
		return;
	}

	dispatchMessage(&msg, server, &remoteAddr);
	if(msg.header.dataSize > 0) {
		free(msg.data);
	}
}

void initServer(YakServer* server) {
	server->clients = malloc(server->settings.maxConnections * sizeof(ClientSession));
	server->numClients = 0;
	server->running = 0;
}

void destroyServer(YakServer* server) {
	int count = server->settings.maxConnections;
	for(int i=0; i<count; i++) {
		if(&server->clients[i] != NULL) {
			free(server->clients[i].userName);
			free(&server->clients[i]);
		}
	}
}

void runServer(ServerSettings* settings) {

	YakServer server;
	server.settings = *settings;
	initServer(&server);

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

	printf("Stopping server... ");
	stopServer(&server);
	destroyServer(&server);
	printf("done!");
}

