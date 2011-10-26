
#ifndef NETWORK_H_
#define NETWORK_H_
#endif /* NETWORK_H_ */

typedef struct {
	union {
		unsigned char firstByte;
		unsigned char secondByte;
		unsigned char thirdByte;
		unsigned char fourthByte;
		long int longInt;
	} ip;
	int port;
} IpAddress;
