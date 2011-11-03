#include <windows.h>
#include <string.h>

#include "connection.h"
#include "../protocol/protocol.h"
#include "../client/client.h"
#include "../server/server.h"

#define SOCKET_IMPL "wsock32"

/******** PRIVATE *********/

int createSocket(SOCKET* s) {
	*s = socket(AF_INET, SOCK_STREAM, 0);
	if(*s == INVALID_SOCKET) {
		return -1;
	}
	return 0;
}

SOCKADDR_IN* createIpAddress(const char* ip, unsigned short port) {
	SOCKADDR_IN* addr = malloc(sizeof(SOCKADDR_IN));
	memset(addr, 0, sizeof(SOCKADDR_IN));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = inet_addr(ip);
	return addr;
}

int initSockets(void) {
	WSADATA wsa;
	int code = WSAStartup(MAKEWORD(2,0), &wsa);
	if(code == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}

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

/******** PUBLIC *********/

int sendMessage(YakMessage* msg, Connection* con) {
	int size = sizeof(YakHeader) + msg->header.dataSize * sizeof(unsigned char);
	char* bytes = serializeMessage(msg, size);
	int sent = send(con->socketId, bytes, size, 0);
	if(sent == SOCKET_ERROR) {
		free(bytes);
		return -1;
	}
	free(bytes);
	return 0;
}

int receiveMessage(YakMessage* msg, Connection* con) {


	int size = sizeof(YakHeader);
	char* buffer = malloc(size);

	int result = recv(con->socketId, buffer, size, 0);
	if(result == 0) {
		return CONN_LOST;
	} else if(result == SOCKET_ERROR) {
		return CONN_ERROR;
	}

	YakHeader* header = deSerializeHeader(buffer, size);
	msg->header = *header;
	free(buffer);
	free(header);

	size = header->dataSize;
	if(size > 0) {
		buffer = malloc(size);
		result = recv(con->socketId, buffer, size, 0);
		if(result == 0) {
			return CONN_LOST;
		} else if(result == SOCKET_ERROR) {
			return CONN_ERROR;
		}
		msg->data = malloc(size * sizeof(unsigned char));
		for(int i=0; i<size; i++) {
			msg->data[i] = buffer[i];
		}
	}

	return 0;
}

ClientError startClient(const char* ip, unsigned short port, Connection* server) {

	int code;
	code = initSockets();
	if(code != 0) {
		return INIT_SOCKET_ERROR;
	}

	SOCKADDR_IN* address = createIpAddress(ip, port);
	SOCKET s;

	code = createSocket(&s);
	if(code != 0) {
		return CREATE_SOCKET_ERROR;
	}

	code = connect(s, (SOCKADDR*)address, sizeof(SOCKADDR));
	if(code == SOCKET_ERROR) {
		return CONNECT_SOCKET_ERROR;
	}

	server->socketId = s;
	server->type = SERVER;
	server->address = malloc(strlen(ip));
	strcpy(server->address, ip);
	server->port = port;

	return CLIENT_OK;
}

void shutDownClient(Connection* con) {
	closesocket(con->socketId);
	WSACleanup();
}

ServerError startServer(unsigned short port, int maxConnections, Connection* acceptSocket) {

	SOCKADDR_IN addr;
	SOCKET s;

	if(initSockets() != 0) {
		return SERVER_INIT_SOCKET_ERROR;
	}

	int error = createSocket(&s);
	if(error != 0) {
		return SOCKET_CREATE_ERROR;
	}

	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ADDR_ANY;

	error = bind(s, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if(error == SOCKET_ERROR) {
		return SOCKET_BIND_ERROR;
	}

	error = listen(s, maxConnections);
	if(error == SOCKET_ERROR) {
		return SOCKET_LISTEN_ERROR;
	}

	acceptSocket->socketId = s;
	acceptSocket->port = port;

	return SERVER_OK;
}

ServerError waitForConnection(Connection* client, Connection* acceptSocket) {
	client->socketId = accept(acceptSocket->socketId, NULL, NULL);
	if(client->socketId == INVALID_SOCKET) {
		return SOCKET_ACCEPT_ERROR;
	}
	return SERVER_OK;
}

void shutDownServer(Connection* acceptSocket) {
	closesocket(acceptSocket->socketId);
	WSACleanup();
}

int getErrorCode(void) {
	return WSAGetLastError();
}
