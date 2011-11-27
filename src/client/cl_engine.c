#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "client.h"
#include "../protocol/protocol.h"
#include "../network/network.h"
#include "../util/util.h"

typedef struct {
	unsigned int socket;
	unsigned char running;
	ClientSettings settings;
	SOCKADDR_IN serverAddress;
	unsigned char isLoggedIn;
} YakClient;

int handleWelcome(YakMessage* msg, YakClient* client) {
	client->isLoggedIn = 1;
	return 0;
}

int handleRejected(YakMessage* msg, YakClient* client) {
	int reason;
	readRejectedParams(msg, &reason);
	printf("Server rejected connection request. Reason: ");
	switch(reason) {
	case 1: {
		printf("wrong password");
		break;
	}
	case 2: {
		printf("user name already taken");
		break;
	}
	case 3: {
		printf("server is full");
		break;
	}
	default: {
		printf("unknown reason");
	}
	}
	printf("\n");
	client->running = 0;
	client->isLoggedIn = 0;
	return 0;
}

void dispatchMessage(YakMessage* msg, YakClient* client, SOCKADDR_IN* remoteAddr) {

	// TODO check if the message came from the correct server

	int error = 0;
	switch(msg->header.type) {
	case WELCOME: {
		error = handleWelcome(msg, client);
		break;
	}
	case REJECTED: {
		error = handleRejected(msg, client);
		break;
	}
	case SAYS: {
		break;
	}
	case WHISPER: {
		break;
	}
	case BYE: {
		break;
	}
	default: {
		// unrecognized type of message: ignored...
		error = UNRECOGNIZED_MESSAGE;
		printf("discarding message of unknown type %d\n", msg->header.type);
	}
	}

	if(error != 0) {
		// TODO handle error...
	}
}

int logIn(char* password, YakClient* client) {

	int try = 0;
	int code = 0;

	while(try < 3) {

		YakMessage* helloMsg = createHelloMessage(client->settings.userName, password);
		sendMessage(helloMsg, client->socket, &client->serverAddress);

		YakMessage receivedMessage;
		SOCKADDR_IN remoteAddr;
		code = getMessage(&receivedMessage, client->socket, client->settings.timeout, &remoteAddr);
		if(code == 0) {
			// no reply. request may have got lost. try again
			printf("\nconnection request failed. ");
			try++;
			if(try < 3) {
				printf("retrying... ");
			}
			continue;
		} else if(code == SOCKET_ERROR) {
			client->running = 0;
			return WSA_SOCKET_ERROR;
		}

		if(receivedMessage.header.type != WELCOME && receivedMessage.header.type != REJECTED) {
			return INVALID_RESPONSE;
		}
		dispatchMessage(&receivedMessage, client, &remoteAddr);
		return 0;
	}
	return -1;
}

int startClient(YakClient* client) { // TODO duplicated in serv_engine.c; find a way to make code reusable

	WSADATA wsa;
	int code = WSAStartup(MAKEWORD(2,0), &wsa);
	if(code == SOCKET_ERROR) {
		return WSA_STARTUP_FAIL;
	}

	client->socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(client->socket == INVALID_SOCKET) {
		return WSA_CREATE_SOCKET_FAIL;
	}

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(client->settings.clientPort);
	addr.sin_addr.s_addr = ADDR_ANY;
	code = bind(client->socket, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if(code == SOCKET_ERROR) {
		return WSA_BIND_FAIL;
	}

	code = logIn("yak123", client); // TODO debug! instead password should be read from console
	if(code == -1) {
		return CONNECTION_REQUEST_FAIL;
	}

	// TODO wait for reply, resend after timeout, retry some times or give up
	// TODO evaluate reply

	client->running = 1;
	return 0;
}

void stopClient(YakClient* client) {

}

void clientLoop(YakClient* client) {

	int code;
	YakMessage msg;
	SOCKADDR_IN remoteAddress;

	code = getMessage(&msg, client->socket, client->settings.timeout, &remoteAddress);
	if(code == 0) {
		// no message received, no need to handle
		return;
	}
	if(code == SOCKET_ERROR) {
		printError(WSA_SOCKET_ERROR);
		client->running = 0;
	}

	dispatchMessage(&msg, client, &remoteAddress);
	if(msg.header.dataSize > 0) {
		free(msg.data);
	}

	// TODO read input
	// TODO handle user events
}

void initClient(YakClient* client) {
	client->running = 0;
	client->isLoggedIn = 0;
	memset(&client->serverAddress, 0, sizeof(SOCKADDR_IN));
	client->serverAddress.sin_family = AF_INET;
	client->serverAddress.sin_port = htons(client->settings.serverPort);
	client->serverAddress.sin_addr.s_addr = inet_addr(client->settings.serverIp);
}

void runClient(ClientSettings* settings) {

	int error;
	YakClient client;
	client.settings = *settings;
	initClient(&client);

	printf("connecting to host %s:%u... ", settings->serverIp, settings->serverPort);
	error = startClient(&client);
	if(error != 0) {
		printError(error);
		stopClient(&client);
		exit(error);
	} else {
		printf("success!\n");
	}

	while(client.running) {
		clientLoop(&client);
	}

	printf("Shutting down... ");
	stopClient(&client);
	printf("done!\n");
}
