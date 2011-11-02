#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "../network/network.h"

ClientError runClient(ClientSettings* settings) {

	ClientError error;

	printf("connecting to host %s:%u...", settings->serverIp, settings->serverPort);
	error = startClient(settings->serverIp, settings->serverPort);
	if(error != CLIENT_OK) {
		printf("failed!\n");
	} else {
		printf("success!\n");
	}

	printf("Shutting down... ");
	shutDownClient();
	printf("done!\n");

	return error;;
}
