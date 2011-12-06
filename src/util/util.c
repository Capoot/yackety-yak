#include <windows.h>
#include <stdio.h>
#include "../client/client.h"

void printError(int code) {

	printf("fatal error: ");

	// WSA Error
	if(code >= 100 && code < 200) {
		int wsaError = WSAGetLastError();
		switch(wsaError) {
		case 10014: {
			printf("Bad pointer (internal memory corruption)");
			break;
		}
		case 10038: {
			printf("Socket operation on nonsocket");
			break;
		}
		case 10040: {
			printf("Message too long");
			break;
		}
		case 10047: {
			printf("Address family not supported by protocol family");
			break;
		}
		case 10054: {
			printf("Connection reset by peer");
			break;
		}
		default: {
			printf("unknown WSA error %d", wsaError);
		}
		}
		printf("\n");
		return;
	}

	// client error
	if(code >= 200 && code < 300) {
		switch(code) {
		case INVALID_RESPONSE: {
			printf("server responed with invalid message type");
			break;
		}
		case CONNECTION_REQUEST_FAIL: {
			printf("connecting to server failed after retrying three times");
			break;
		}
		case REJECTED_NAME_TAKEN: {
			printf("user name not available");
			break;
		}
		case REJECTED_SERVER_FULL: {
			printf("server is full");
			break;
		}
		case REJECTED_WRONG_PASSWORD: {
			printf("wrong password");
			break;
		}
		default: {
			printf("internal error %d", code);
		}
		}
		printf("\n");
		return;
	}

	printf("unexpected internal error %d\n", code);
}

void readLine(char* buffer, int maxLength) {
	fgets(buffer, 32, stdin);
	// now remove trailing line feeds
	int len = strlen(buffer);
	for(int i=1; i<2; i++) {
		if(buffer[len-i] == '\n' || buffer[len-i] == '\r') {
			buffer[len-i] = '\0';
		}
	}
}
