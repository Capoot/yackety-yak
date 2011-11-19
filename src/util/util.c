#include <windows.h>
#include <stdio.h>

void printError(int code) {

	printf("fatal Error: ");

	// WSA Error
	if(code >= 100 && code < 200) {
		int wsaError = WSAGetLastError();
		printf("WSA Error %d: ");
		switch(wsaError) {
		default: {
			printf("unknown WSA error");
		}
		}
	// generic error
	} else {
		printf("unexpected internal server error");
	}

	printf("\n");
}
