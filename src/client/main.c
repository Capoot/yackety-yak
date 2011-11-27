#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "client.h"

void handleApplicationError(int error) { // TODO remove unused errors
	printf("Fatal error: ");
	switch(error) {
	case NAME_NOT_SPECIFIED: {
		printf("missing argument <user name>");
		break;
	}
	case REMOTE_ADDRESS_NOT_SPECIFIED: {
		printf("missing argument <server address>");
		break;
	}
	case PORT_NOT_SPECIFIED: {
		printf("missing argument <server port>");
		break;
	}
	case CONNECTION_ERROR: {
		printf("socket error");
		break;
	}
	case CREATE_SOCKET_ERROR: {
		printf("could not create socket");
		break;
	}
	case CONNECT_SOCKET_ERROR: {
		printf("could not establish connection to remote host");
		break;
	}
	case INIT_SOCKET_ERROR: {
		printf("could not initialize socket");
		break;
	}
	case NETWORK_ERROR: {
		printf("");
	}
	default: {
		printf("unknown error");
	}
	}
	exit((int)error);
}

ClientError readArgs(int argc, char* argv[], ClientSettings* settings) {
	for(int i=1; i<argc-1; i++) {
		if(strcmp(argv[i], "-n") == 0) {
			if(argv[i+1] == NULL) {
				return NAME_NOT_SPECIFIED;
			}
			settings->userName = argv[i+1];
			continue;
		}
		if(strcmp(argv[i], "-a") == 0) {
			if(argv[i+1] == NULL) {
				return REMOTE_ADDRESS_NOT_SPECIFIED;
			}
			settings->serverIp = argv[i+1];
			continue;
		}
		if(strcmp(argv[i], "-p") == 0) {
			if(argv[i+1] == NULL) {
				return PORT_NOT_SPECIFIED;
			}
			long int port = strtol(argv[i+1], NULL, 10);
			settings->serverPort = (int)port;
			continue;
		}
	}
	if(strcmp(settings->userName, "") == 0) {
		return NAME_NOT_SPECIFIED;
	}
	if(strcmp(settings->serverIp,"") == 0) {
		return REMOTE_ADDRESS_NOT_SPECIFIED;
	}
	if(settings->serverPort == 0) {
		return PORT_NOT_SPECIFIED;
	}
	return CLIENT_OK;
}

void init(ClientSettings* settings) {
	settings->userName = "";
	settings->serverIp = "";
	settings->clientPort = 55554;
	settings->serverPort = 55555;
	settings->timeout.tv_sec = 10; // TODO read param instead of default
	settings->timeout.tv_usec = 0;
}

int main(int argc, char* argv[]) {

	ClientSettings settings;
	init(&settings);

	ClientError error = readArgs(argc, argv, &settings);
	if(error != CLIENT_OK) {
		handleApplicationError(error);
	}

	runClient(&settings);
	return 0;
}
