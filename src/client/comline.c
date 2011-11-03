#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "../network/network.h"

ClientError dispatchMessage(YakMessage*);
int handleIdentify(short);
void printErrorMessage();

Connection SERVER_CONNECTION;
ClientSettings* SETTINGS;

ClientError runClient(ClientSettings* settings) {

	ClientError error;
	SETTINGS = settings;

	printf("connecting to host %s:%u...", settings->serverIp, settings->serverPort);
	error = startClient(settings->serverIp, settings->serverPort, &SERVER_CONNECTION);
	if(error != CLIENT_OK) {
		printf("failed!\n");
	} else {
		printf("success!\n");
	}

	int code;
	while(1) {

		YakMessage msg;

		printf("*** receiving ***\n");
		code = receiveMessage(&msg, &SERVER_CONNECTION);
		if(code != 0) {
			printErrorMessage();
			error = CONNECTION_ERROR;
			break;
		}

		printf("*** dispatching ***\n");
		error = dispatchMessage(&msg);
		if(error != CLIENT_OK) {
			break;
		}

		if(msg.header.dataSize > 0) {
			free(msg.data);
		}
	}

	printf("Shutting down... ");
	shutDownClient(&SERVER_CONNECTION);
	free(SERVER_CONNECTION.address);
	printf("done!\n");

	return error;
}

ClientError dispatchMessage(YakMessage* msg) {
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
	case GOTO: {
		break;
	}
	case IDENTIFY: {
		return handleIdentify((short)msg->data[0]);
	}
	default: {
		return -1;
	}
	}
	return CLIENT_OK;
}

int handleIdentify(short pwRequired) {
	printf("*** Received IDENTIFY, ");
	if(pwRequired) {
		printf("password authorization is REQUIRED ***\n");
	} else {
		printf("a password is not required ***\n");
	}
	YakMessage* msg = createHelloMessage(SETTINGS->userName, "abc"); // TODO read PW from console
	int code = sendMessage(msg, &SERVER_CONNECTION);
	if(code != 0) {
		printErrorMessage();
		return -1;
	}
	return 0;
}

void printErrorMessage() {
	int code = getErrorCode();
	printf("An error occured: ");
	switch(code) {
	case 10054: {
		// Winsock32 Error
		printf("connection reset by remote host. The server may have gone offline.");
		break;
	}
	default: {
		printf("unknown error (code %d)", code);
	}
	}
	printf("\n");
}
