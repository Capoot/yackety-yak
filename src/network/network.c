#include <windows.h>
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

	int received = 0;
	int size = sizeof(YakHeader);
	char* buffer = malloc(size);

	int result = recvfrom(socket, buffer, size, 0, (SOCKADDR*)remoteAddr, addrSize);
	if(result <= 0) {
		// result is an error code
		return result;
	} else {
		received += result;
	}

	YakHeader* header = deSerializeHeader(buffer, size);
	msg->header = *header;
	free(buffer);
	free(header);

	size = header->dataSize;
	if(size > 0) {
		buffer = malloc(size);
		result = recvfrom(socket, buffer, size, 0, (SOCKADDR*)remoteAddr, addrSize);
		if(result <= 0) {
			// result is an error code
			return result;
		} else {
			received += result;
		}
		msg->data = malloc(size * sizeof(unsigned char));
		for(int i=0; i<size; i++) {
			msg->data[i] = buffer[i];
		}
	}

	return received;
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

	int size = sizeof(remoteAddr);
	code = receiveMessage(msg, s, remoteAddr, &size);
	return code;
}
