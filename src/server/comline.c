#include <stdio.h>
#include "server.h"
#include "../network/network.h"

ServerError runServer(ServerSettings* settings) {

	printf("Launching server at port %d... ", settings->listenPort);
	ServerError error = startServer(settings->listenPort, settings->maxConnections);
	if(error != SERVER_OK) {
		printf("failed! (code %d)\n", getErrorCode());
		return error;
	} else {
		printf("success!\n");
	}

	printf("Awaiting incoming connection...\n");
	error = waitForConnection();
	if(error != SERVER_OK) {
		return error;
	} else {
		printf("Received new connection request\n");
	}

	printf("Shutting down... ");
	shutDownServer();
	printf("done!\n");

	return error;
}
