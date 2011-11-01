#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "../network/network.h"

ClientError runClient(ClientSettings* settings) {

	ClientError error;
	printf("Initializing sockets... ");
	error = initSockets();
	if(error != CLIENT_OK) {
		return NETWORK_ERROR;
	}
	printf("success!\n");

	printf("connecting to host %s:%u...", settings->serverIp, settings->serverPort);
	error = connectToServer(settings->serverIp, settings->serverPort);
	if(error != CLIENT_OK) {
		printf("failed!\n");
		return error;
	}
	printf("success!\n");

	printf("disconnecting...\n");
	disconnectFromServer();

	printf("Releasing sockets... ");
	cleanupSockets();
	printf("done!\n");

	return CLIENT_OK;
}
