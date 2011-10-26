#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "client.h"

int handleError(ClientError);

void parseIp(char* ipString, IpAddress* ipAddress) {

	int len = strlen(ipString);
	int i = 0;
	char* buffer = (char*)malloc(4 * sizeof(char));
	int bufferIndex = 0;
	int byteNr = 0;

	do {
		if(ipString[i] == '.') {
			long int value = strtol(buffer, NULL, 10);
			switch(byteNr) {
			case 0: {
				ipAddress->ip.firstByte = (unsigned char)value;
				break;
			}
			case 1: {
				ipAddress->ip.secondByte = (unsigned char)value;
				break;
			}
			case 2: {
				ipAddress->ip.thirdByte = (unsigned char)value;
				break;
			}
			case 3: {
				ipAddress->ip.fourthByte = (unsigned char)value;
			}
			}
			bufferIndex = 0;
			byteNr++;
		} else {
			buffer[bufferIndex++] = ipString[i];
		}
		i++;
	} while(i < len);
}

ClientError readArgs(int argc, char* argv[], ClientSettings* settings) {
	for(int i=0; i<argc; i++) {
		if(strcmp(argv[i], "-n") == 0) {
			if(argv[i+1] == NULL) {
				return NAME_NOT_SPECIFIED;
			}
			settings->name = argv[i+1];
			continue;
		}
		if(strcmp(argv[i], "-a") == 0) {
			if(argv[i+1] == NULL) {
				return REMOTE_ADDRESS_NOT_SPECIFIED;
			}
			parseIp(argv[i+1], &settings->remoteAddress);
			continue;
		}
		if(strcmp(argv[i], "-p") == 0) {
			if(argv[i+1] == NULL) {
				return PORT_NOT_SPECIFIED;
			}
			long int port = strtol(argv[i+1], NULL, 10);
			settings->remoteAddress.port = (int)port;
			continue;
		}
	}
	return OK;
}

int main(int argc, char* argv[]) {

	ClientSettings settings;

	ClientError error = readArgs(argc, argv, &settings);
	if(error != OK) {
		return handleError(error);
	}

	printf("the name is: %s\n", settings.name);
	printf("the ip is: %d.%d.%d.%d (%d)\n",
			settings.remoteAddress.ip.firstByte,
			settings.remoteAddress.ip.secondByte,
			settings.remoteAddress.ip.thirdByte,
			settings.remoteAddress.ip.fourthByte,
			settings.remoteAddress.ip.longInt);
	printf("the port is %d", settings.remoteAddress.port);

	return 0;
}

int handleError(ClientError error) {
	printf("An error has occured: ");
	switch(error) {
	case NAME_NOT_SPECIFIED: {
		printf("missing argument <user name>");
		break;
	}
	case REMOTE_ADDRESS_NOT_SPECIFIED: {
		printf("missing argument <remote address>");
		break;
	}
	case PORT_NOT_SPECIFIED: {
		printf("missing argument <remote port>");
		break;
	}
	default: {
		printf("unknown error");
	}
	}
	printf("\n");
	return (int)error;
}
