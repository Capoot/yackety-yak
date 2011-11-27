#include <windows.h>
#include <stdio.h>
#include "../client/client.h"

void printError(int code) {

	printf("fatal error: ");

	// WSA Error
	if(code >= 100 && code < 200) {
		int wsaError = WSAGetLastError();
		printf("WSA Error %d: ");
		switch(wsaError) {
		default: {
			printf("unknown WSA error");
		}
		}

	// client error
	} else if(code >= 200 && code < 300) {
		switch(code) {
		case INVALID_RESPONSE: {
			printf("server responed with invalid message type");
			break;
		}
		case CONNECTION_REQUEST_FAIL: {
			printf("connecting to server failed after retrying three times");
			break;
		}
		default: {
			printf("unknown internal client error");
		}
		}

	// generic error
	} else {
		printf("unexpected internal error");
	}

	printf("\n");
}
