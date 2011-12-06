#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

ServerError readArgs(int argc, char* argv[], ServerSettings* settings) {
	if(argc > 1) {
		for(int i=1; i<argc-1; i++) {
			if(strcmp(argv[i], "-p") == 0) {
				if(argv[i+1] == NULL) {
					return LISTEN_PORT_NOT_SPECIFIED;
				}
				long int port = strtol(argv[i+1], NULL, 10);
				settings->serverPort = (unsigned short)port;
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
	}
	if(settings->serverPort == 0) {
		return LISTEN_PORT_NOT_SPECIFIED;
	}
	if(settings->maxConnections == 0) {
		return MAX_CONNECTIONS_NOT_SPECIFIED;
	}
	return SERVER_OK;
}

void init(ServerSettings* settings) {
	settings->serverPort = 55555;
	settings->clientPort = 55554;
	settings->maxConnections = 10;
	settings->timeout.tv_sec = 10; // FIXME use argument instead of default
	settings->timeout.tv_usec = 0;
	settings->password = "yak123"; // FIXME debug default password
}

void handleError(ServerError error) {
	printf("Fatal Error: ");
	switch(error) {
	case SOCKET_BIND_ERROR: { // TODO cleanup unused error msgs
		printf("could not bind socket");
		break;
	}
	case SOCKET_LISTEN_ERROR: {
		printf("could not listen at socket");
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

	runServer(settings);
	return 0;
}
