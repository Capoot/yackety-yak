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
} YakClient;

int startClient(YakClient* client) {
	client->running = 1;
	return 0;
}

void stopClient(YakClient* client) {

}

int getMessage(YakMessage* msg, YakClient* client) {
	return 0;
}

void dispatchMessage(YakMessage* msg, YakClient* client) {
	int error = 0;
	switch(msg->header.type) {
	case WELCOME: {
		break;
	}
	case REJECTED: {
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
		printf("discarding message of unknown type %d\n", msg->header.type);
	}
	}

	if(error != 0) {
		// TODO handle error...
	}
}

void clientLoop(YakClient* client) {

	int code;
	YakMessage msg;

	code = getMessage(&msg, client);
	if(code == 0) {
		// no message received, no need to handle
		return;
	}
	if(code == SOCKET_ERROR) {
		printError(WSA_SOCKET_ERROR);
		client->running = 0;
	}

	dispatchMessage(&msg, client);
	if(msg.header.dataSize > 0) {
		free(msg.data);
	}
}

void initClient(YakClient* client) {
	client->running = 0;
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
