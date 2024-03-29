#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include "server.h"
#include "../protocol/protocol.h"
#include "../network/network.h"
#include "../util/util.h"

typedef struct {
	char* userName;
	SOCKADDR_IN address;
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
	addr.sin_port = htons(server->settings.serverPort);
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

int putClient(char* name, char* password, SOCKADDR_IN* remoteAdrr, YakServer* server) {
	if(strcmp(server->settings.password, password) != 0) {
		return 2;
	}
	if(server->numClients >= server->settings.maxConnections) {
		return 1;
	}
	// check if user name is available
	for(int i=0; i<server->settings.maxConnections; i++) {
		if(server->clients[i].userName == NULL) {
			continue;
		}
		if(strcmp(server->clients[i].userName, name) == 0) {
			return 3;
		}
	}
	server->clients[server->numClients].address = *remoteAdrr;
	server->clients[server->numClients].userName = name;
	server->numClients++;
	return 0;
}

int handleHelloMessage(YakMessage* msg, SOCKADDR_IN* remoteAddr, YakServer* server) {

	char* name;
	char* password;
	char ipString[16];
	readHelloParams(msg, &name, &password);
	YakMessage* reply;

	int code = putClient(name, password, remoteAddr, server);
	if(code != 0) {
		reply = createRejectedMessage(code);
	} else {
		reply = createWelcomeMessage();
	}

	int error = 0;
	error = sendMessage(reply, server->socket, remoteAddr);
	if(error == SOCKET_ERROR) {
		// TODO this doesn't have to be fatal. might be connection reset by peer or something...
		return WSA_SOCKET_ERROR;
	}

	ipToString(remoteAddr->sin_addr.s_addr, ipString);
	if(code == 0) {
		printf("User %s connected from remote address %s\n", name, ipString);
	} else {
		printf("Connection request from remote address %s was rejected (code %d)\n", ipString, code);
	}
	return 0;
}

char* findUser(SOCKADDR_IN* addr, YakServer* server) {
	for(int i=0; i<server->settings.maxConnections; i++) {
		if(server->clients[i].userName == NULL) {
			continue;
		}
		if(server->clients[i].address.sin_addr.s_addr == addr->sin_addr.s_addr) {
			return server->clients[i].userName;
		}
	}
	return NULL;
}

int broadcast(YakMessage* msg, YakServer* server) {
	for(int i=0; i<server->settings.maxConnections; i++) {
		if(server->clients[i].userName == NULL) { // FIXME I dont like this... this really should be a collection
			continue;
		}
		char ip[16];
		ipToString(server->clients[i].address.sin_addr.s_addr, ip);
		if(sendMessage(msg, server->socket, &server->clients[i].address) == SOCKET_ERROR) {
			return WSA_SOCKET_ERROR;
		}
	}
	return 0;
}

int handleSayMessage(YakMessage* msg, SOCKADDR_IN* remoteAddr, YakServer* server) {
	char* user = findUser(remoteAddr, server);
	if(user == NULL) {
		// no such user: drop msg
		char ipString[16];
		ipToString(remoteAddr->sin_addr.s_addr, ipString);
		printf("Warning: received message from unauthorized host %s. This could be an attempt of attack\n", ipString);
	}
	char* text;
	readSayParams(msg, &text);
	printf("%s says:\"%s\"\n", user, text);
	YakMessage* says = createSaysMessage(user, text, 0);
	int error = broadcast(says, server);
	free(text);
	deleteMessage(says);
	return error;
}

void removeClient(char* name, YakServer* server) {
	for(int i=0; i<server->settings.maxConnections; i++) {
		if(server->clients[i].userName == NULL) {
			continue;
		}
		if(strcmp(server->clients[i].userName, name) == 0) {
			server->clients[i].userName = NULL;
			memset(&server->clients[i].address, 0, sizeof(SOCKADDR_IN));
			server->numClients--;
			printf("User %s disconnected\n", name);
		}
	}
}

int handleBye(YakMessage* msg, SOCKADDR_IN* remoteAddr, YakServer* server) {
	char* user = findUser(remoteAddr, server);
	if(user == NULL) { // TODO duplicated code
		// no such user: drop msg
		char ipString[16];
		ipToString(remoteAddr->sin_addr.s_addr, ipString);
		printf("Warning: received message from unauthorized host %s. This could be an attempt of attack\n", ipString);
	}
	removeClient(user, server);
	return 0;
}

void dispatchMessage(YakMessage* msg, YakServer* server, SOCKADDR_IN* remoteAddr) {

	int error = 0;
	switch(msg->header.type) {
	case HELLO: {
		error = handleHelloMessage(msg, remoteAddr, server);
		break;
	}
	case SAY: {
		error = handleSayMessage(msg, remoteAddr, server);
		break;
	}
	case BYE: {
		error = handleBye(msg, remoteAddr, server);
		break;
	}
	// TODO proof of live...
	default: {
		// unrecognized type of message: ignored...
		printf("discarding message of unknown type %d\n", msg->header.type);
	}
	}

	if(error == WSA_SOCKET_ERROR) {
		// server socket broken
		printError(WSA_SOCKET_ERROR);
		server->running = 0;
	}
}

void serverLoop(YakServer* server) {

	int code;
	YakMessage msg;
	SOCKADDR_IN remoteAddr;
	memset(&remoteAddr, 0, sizeof(SOCKADDR_IN));

	code = getMessage(&msg, server->socket, server->settings.timeout, &remoteAddr);
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

	printf("Launching Yak server at port %d... ", settings->serverPort);
	int error = startServer(&server);
	if(error != 0) {
		printError(error);
		stopServer(&server);
		exit(error);
	} else {
		printf("success!\n");
	}

	while(server.running) {
		serverLoop(&server);
	}

	printf("Stopping server... ");
	stopServer(&server);
	destroyServer(&server);
	printf("done!\n");
}

