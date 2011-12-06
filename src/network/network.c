#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "network.h"

char* serializeMessage(YakMessage* msg, int size) {
	char* bytes = malloc(size);
	char* pointer = (char*)msg;
	int yakSize = sizeof(YakHeader);
	for(int i=0; i<yakSize; i++) {
		bytes[i] = pointer[i];
	}
	for(int i=0; i<msg->header.dataSize; i++) {
		bytes[yakSize+i] = (char)msg->data[i];
	}
	return bytes;
}

YakHeader* deSerializeHeader(char* bytes, int size) {
	YakHeader* header = malloc(sizeof(YakHeader));
	char* pointer = (char*)header;
	for(int i=0; i<size; i++) {
		pointer[i] = bytes[i];
	}
	return header;
}

int receiveMessage(YakMessage* msg, unsigned int socket, SOCKADDR_IN* remoteAddr, int* addrSize) {

	int size = sizeof(YakHeader) + MAX_DATA_SIZE;
	char* buffer = malloc(size);

	int result = recvfrom(socket, buffer, size, 0, (SOCKADDR*)remoteAddr, addrSize);
	if(result <= 0) {
		// result is an error code
		free(buffer);
		return result;
	}

	YakHeader* header = deSerializeHeader(buffer, sizeof(YakHeader));
	msg->header = *header;
	free(header);

	int dataSize = header->dataSize;
	int dataStart = sizeof(YakHeader);
	if(dataSize > 0) {
		msg->data = malloc(dataSize * sizeof(BYTE));
		for(int i=0; i<dataSize; i++) {
			msg->data[i] = buffer[dataStart+i]; // FIXME implement network to host translation
		}
	}
	free(buffer);

	return result;
}

int sendMessage(YakMessage* msg, unsigned int socket, SOCKADDR_IN* target) {
	int size = sizeof(YakHeader) + msg->header.dataSize * sizeof(unsigned char);
	char* bytes = serializeMessage(msg, size);
	int sent = sendto(socket, bytes, size, 0, (SOCKADDR*)target, sizeof(SOCKADDR_IN));
	free(bytes);
	return sent;
}

int getMessage(YakMessage* msg, SOCKET s, struct timeval timeout, SOCKADDR_IN* remoteAddr) {

	FD_SET fdset;
	FD_ZERO(&fdset);
	FD_SET(s, &fdset);

	int code = select(0, &fdset, NULL, NULL, &timeout);
	if(code == 0) {
		// timeout - socket not ready yet
		return 0;
	}else if(code == SOCKET_ERROR) {
		return code;
	}

	if(!FD_ISSET(s, &fdset)) {
		// nothing to read
		return 0;
	}

	int size = sizeof(SOCKADDR_IN);
	code = receiveMessage(msg, s, remoteAddr, &size);
	return code;
}

int ipToString(unsigned long ip, char* buffer) {
	unsigned long reversedIp = ntohl(ip);
	BYTE byte1 = (BYTE)(reversedIp >> 24);
	BYTE byte2 = (BYTE)(reversedIp >> 16);
	BYTE byte3 = (BYTE)(reversedIp >> 8);
	BYTE byte4 = (BYTE)reversedIp;
	sprintf(buffer, "%u.%u.%u.%u", byte1, byte2, byte3, byte4);
	return strlen(buffer);
}
