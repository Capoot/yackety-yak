#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "client.h"

void handleError(ClientError);

void parseIp(char* ipString, IpAddress* ipAddress) {

	char* delimiter = ".";
	char* substr;
	int byte = 3;

	substr = strtok(ipString, delimiter);
	while(substr != NULL) {
		unsigned long int value = strtol(substr, NULL, 10);
		unsigned long int shifted = value << (byte-- * 8);
		ipAddress->address |= shifted;
		substr = strtok(NULL, delimiter);
	}
}

ClientError readArgs(int argc, char* argv[], ClientSettings* settings) {
	for(int i=1; i<argc-1; i++) {
		if(strcmp(argv[i], "-n") == 0) {
			if(argv[i+1] == NULL) {
				return NAME_NOT_SPECIFIED;
			}
			settings->name = argv[i+1];
			continue;
		}
		if(strcmp(argv[i], "-a") == 0) {
			if(argv[i+1] == NULL) {
				return REMOTE_ADDRESS_NOT_SPECIFIED;
			}
			parseIp(argv[i+1], &settings->remoteAddress);
			continue;
		}
		if(strcmp(argv[i], "-p") == 0) {
			if(argv[i+1] == NULL) {
				return PORT_NOT_SPECIFIED;
			}
			long int port = strtol(argv[i+1], NULL, 10);
			settings->remoteAddress.port = (int)port;
			continue;
		}
	}
	if(strcmp(settings->name, "") == 0) {
		return NAME_NOT_SPECIFIED;
	}
	if(settings->remoteAddress.address == 0) {
		return REMOTE_ADDRESS_NOT_SPECIFIED;
	}
	if(settings->remoteAddress.port == 0) {
		return PORT_NOT_SPECIFIED;
	}
	return CLIENT_OK;
}

void init(ClientSettings* settings) {
	int code;
	settings->name = "";
	settings->remoteAddress.address = 0;
	settings->remoteAddress.port = 0;
	printf("Initializing sockets... ");
	code = initSockets();
	if(code == 0) {
		printf("success!\n");
	} else {
		printf("error! (code %d)", code);
	}
}

int main(int argc, char* argv[]) {

	ClientSettings settings;
	init(&settings);

	ClientError error = readArgs(argc, argv, &settings);
	if(error != CLIENT_OK) {
		handleError(error);
	}

	error = runClient(&settings);
	if(error != CLIENT_OK) {
		handleError(error);
	}

	exit(0);
}

void handleError(ClientError error) {
	printf("Fatal error: ");
	switch(error) {
	case NAME_NOT_SPECIFIED: {
		printf("missing argument <user name>");
		break;
	}
	case REMOTE_ADDRESS_NOT_SPECIFIED: {
		printf("missing argument <remote address>");
		break;
	}
	case PORT_NOT_SPECIFIED: {
		printf("missing argument <remote port>");
		break;
	}
	case CONNECTION_ERROR: {
		printf("connection lost");
		break;
	}
	default: {
		printf("unknown error");
	}
	}
	printf("\n");
	exit((int)error);
}
