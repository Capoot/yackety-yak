#include <windows.h>

int initSockets(void) {
	WSADATA wsa;
	return WSAStartup(MAKEWORD(2,0), &wsa);
}
