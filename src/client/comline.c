#include <stdio.h>
#include <stdlib.h>

#include "client.h"

NetworkError connectToServer(IpAddress*);
NetworkError disconnect(void);
int parseAction(char*, Action*);
void perform(Action*);
void readInput(char*);

ClientError runClient(ClientSettings* settings) {

	NetworkError error;
	error = connectToServer(&settings->remoteAddress);
	if(error != NETWORK_OK) {
		return CONNECTION_ERROR;
	}

	/*while(1) {
		char* buffer = NULL;
		Action* action = NULL;
		int code;
		readInput(buffer);
		code = parseAction(buffer, action);
		if(code != 0) {
			printf("Error parsing input\n");
		} else {
			perform(action);
		}
	}*/

	error = disconnect();
	if(error == CONNECTTION_TIMEOUT) {
		printf("WARNING: server did not confirm disconnect. Connection may have timed out.");
	}

	return CLIENT_OK;
}

NetworkError connectToServer(IpAddress* address) {
	printf("TODO: connect... (not implemented yet)\n");
	return NETWORK_OK;
}

NetworkError disconnect(void) {
	printf("TODO: disconnect... (not implemented yet)");
	return NETWORK_OK;
}

int parseAction(char* input, Action* action)  {
	return 0;
}

void perform(Action* action) {

}

void readInput(char* buffer) {

}
