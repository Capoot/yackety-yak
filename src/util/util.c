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
		case 10040: {
			printf("Message too long");
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
		default: {
			printf("unknown internal client error");
		}
		}
		printf("\n");
		return;
	}

	printf("unexpected internal error\n");
}
