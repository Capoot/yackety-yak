#include <stdio.h>
#include <stdlib.h>
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
	Connection* client = malloc(sizeof(Connection));
	error = waitForConnection(client);
	if(error != SERVER_OK) {
		return error;
	} else {
		printf("Received new connection request\n");
	}

	printf("Sending IDENTIFY... ");
	YakMessage* msg = createIdentifyMessage(0);
	if(sendMessage(msg, client) != 0) {
		printf("failed!\n");
		return -1;
	}
	printf("success!\n");

	printf("Shutting down... ");
	shutDownServer();
	printf("done!\n");

	return error;
}
