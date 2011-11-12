#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "../network/network.h"

int serverLoop(Connection*);

ServerError runServer(ServerSettings* settings) {

	Connection acceptSocket;

	printf("Launching server at port %d... ", settings->listenPort);
	ServerError error = startServer(settings->listenPort, settings->maxConnections, &acceptSocket);
	if(error != SERVER_OK) {
		printf("failed! (code %d)\n", getErrorCode());
		return error;
	} else {
		printf("success!\n");
	}

	printf("Awaiting incoming connection...\n");
	Connection client;
	error = waitForConnection(&client, &acceptSocket);
	if(error != SERVER_OK) {
		return error;
	} else {
		printf("Received new connection request\n");
	}

	printf("Sending IDENTIFY... ");
	YakMessage* msg = createIdentifyMessage(0);
	if(sendMessage(msg, &client) != 0) {
		printf("failed!\n");
		// TODO eval code, print error, return propper error
		return -1;
	}
	printf("success!\n");

	// TODO loop this
	YakMessage helloMessage;
	int result = receiveMessage(&helloMessage, &client);
	if(result != 0) {
		printf("*** error %d occured, expected hello message ***\n", getErrorCode());
	}
	printf("received a response: ");
	if(helloMessage.header.type == HELLO) {
		char* name = NULL;
		char* pw = NULL;
		readHelloParams(&helloMessage, &name, &pw);
		printf("HELLO (%s/%s)\n", name, pw);
	}
	// TODO respond to hello (welcome/rejected)

	printf("Shutting down... ");
	shutDownServer(&acceptSocket);
	printf("done!\n");

	return error;
}

int serverLoop(Connection* acceptSocket) {
	return 0;
}
