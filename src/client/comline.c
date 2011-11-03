#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "../network/network.h"

ClientError runClient(ClientSettings* settings) {

	ClientError error;
	Connection server;

	printf("connecting to host %s:%u...", settings->serverIp, settings->serverPort);
	error = startClient(settings->serverIp, settings->serverPort, &server);
	if(error != CLIENT_OK) {
		printf("failed!\n");
	} else {
		printf("success!\n");
	}

	printf("Shutting down... ");
	shutDownClient(&server);
	free(server.address);
	printf("done!\n");

	return error;
}
