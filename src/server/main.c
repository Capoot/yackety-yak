#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

ServerError readArgs(int argc, char* argv[], ServerSettings* settings) {
	for(int i=1; i<argc-1; i++) {
		if(strcmp(argv[i], "-p") == 0) {
			if(argv[i+1] == NULL) {
				return LISTEN_PORT_NOT_SPECIFIED;
			}
			long int port = strtol(argv[i+1], NULL, 10);
			settings->listenPort = (unsigned short)port;
			continue;
		}
		if(strcmp(argv[i], "-n") == 0) {
			if(argv[i+1] == NULL) {
				return MAX_CONNECTIONS_NOT_SPECIFIED;
			}
			long int max = strtol(argv[i+1], NULL, 10);
			settings->maxConnections = (int)max;
			continue;
		}
	}
	if(settings->listenPort == 0) {
		return LISTEN_PORT_NOT_SPECIFIED;
	}
	if(settings->maxConnections == 0) {
		return MAX_CONNECTIONS_NOT_SPECIFIED;
	}
	return SERVER_OK;
}

void init(ServerSettings* settings) {
	settings->listenPort = 0;
	settings->maxConnections = 0;
}

void handleError(ServerError error) {
	printf("Fatal Error: ");
	switch(error) {
	case SOCKET_BIND_ERROR: {
		printf("could not bind socket");
		break;
	}
	case SOCKET_LISTEN_ERROR: {
		printf("could not set socket to listen mode");
		break;
	}
	case SOCKET_CREATE_ERROR: {
		printf("could not create socket");
		break;
	}
	case SOCKET_ACCEPT_ERROR: {
		printf("error accepting connection");
		break;
	}
	case LISTEN_PORT_NOT_SPECIFIED: {
		printf("missing argument <port>");
		break;
	}
	case MAX_CONNECTIONS_NOT_SPECIFIED: {
		printf("missing argument <maxConnections>");
		break;
	}
	default: {
		printf("unknown error");
	}
	}
}

int main(int argc, char* argv[]) {

	ServerSettings* settings = malloc(sizeof(ServerSettings));
	init(settings);

	ServerError error = readArgs(argc, argv, settings);
	if(error != SERVER_OK) {
		handleError(error);
		exit((int)error);
	}

	error = runServer(settings);
	if(error != SERVER_OK) {
		handleError(error);
		exit((int)error);
	}

	return 0;
}
