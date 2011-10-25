/**
 * A set of convenience functions for handling messages
 */

#include <stdlib.h>
#include <string.h>

#include "protocol.h"

void zeroTerminate(char*, int);

/**
 * Convenience function that makes sure freeing the message's data
 * will not be forgotten
 */
void deleteMessage(YakMessage* msg) {
	if(msg->data != NULL) {
		free(msg->data);
	}
	free(msg);
}

int readSayParams(YakMessage* msg, char* text) {
	if(msg->data == NULL) {
		return 100;
	}
	text = (char*)malloc(msg->header.dataSize * sizeof(char) + 1);
	strncpy(text, (char*)msg->data, msg->header.dataSize);
	zeroTerminate(text, msg->header.dataSize);
	return 0;
}

int readHelloParams(YakMessage* msg, char* name, char* password) {
	if(msg->data == NULL) {
		return 100;
	}
	HelloParams* params = &msg->header.params.helloParams;
	name = (char*)malloc(params->nameLength * sizeof(char) + 1);
	password = (char*)malloc(params->passwordLength * sizeof(char) + 1);
	strncpy(name, (char*)&msg->data[params->nameStart], params->nameLength);
	strncpy(password, (char*)&msg->data[params->passwordStart], params->passwordLength);
	zeroTerminate(name, params->nameLength);
	zeroTerminate(password, params->passwordLength);
	return 0;
}

int readWhisperParams(YakMessage* msg, char* to, char* text) {
	if(msg->data == NULL) {
		return 100;
	}
	WhisperParams* params = &msg->header.params.whisperParams;
	to = (char*)malloc(params->toLength * sizeof(char) + 1);
	text = (char*)malloc(params->messageLength * sizeof(char) + 1);
	strncpy(to, (char*)&msg->data[params->toStart], params->toLength);
	strncpy(text, (char*)&msg->data[params->messageStart], params->messageLength);
	zeroTerminate(to, params->toLength);
	zeroTerminate(text, params->messageLength);
	return 0;
}

int readRogerParams(YakMessage* msg, char* text) {
	if(msg->data == NULL) {
		return 100;
	}
	text = (char*)malloc(msg->header.dataSize * sizeof(char) + 1);
	strncpy(text, (char*)&msg->data, msg->header.dataSize);
	zeroTerminate(text, msg->header.dataSize);
	return 0;
}

int readRejectedParams(YakMessage* msg, int* reason) {
	if(msg->data == NULL) {
		return 100;
	}
	reason = (int*)malloc(sizeof(int));
	*reason = (int)msg->data;
	return 0;
}

int readGotoParams(YakMessage* msg, long int* target) {
	if(msg->data == NULL) {
		return 100;
	}
	target = (long int*)malloc(sizeof(long int));
	*target = (long int)msg->data;
	return 0;
}

int readNamesParams(YakMessage* msg, char** names, int* namesCount) {
	if(msg->data == NULL) {
		return 100;
	}
	int index = 0;
	int count = msg->header.params.namesParams.namesCount;
	namesCount = (int*)malloc(sizeof(int));
	*namesCount = count;
	for(int i=0; i<count; i++) {
		int len = (int)msg->data[index++];
		names[i] = (char*)malloc(sizeof(char) * (len+1));
		for(int j=0; j<len; j++) {
			names[i][j] = msg->data[index++];
		}
		names[i][len] = '\0';
	}
	return 0;
}

int readAddNameParams(YakMessage* msg, char* name) {
	if(msg->data == NULL) {
		return 100;
	}
	name = (char*)malloc(sizeof(char) * (msg->header.dataSize + 1));
	strncpy(name, (char*)&msg->data, msg->header.dataSize);
	zeroTerminate(name, msg->header.dataSize);
	return 0;
}

int readRemoveNameParams(YakMessage* msg, char* name) {
	if(msg->data == NULL) {
		return 100;
	}
	name = (char*)malloc(sizeof(char) * (msg->header.dataSize + 1));
	strncpy(name, (char*)&msg->data, msg->header.dataSize);
	zeroTerminate(name, msg->header.dataSize);
	return 0;
}

/**
 * Adds the terminator symbol '\0' to the end of a string. The function will
 * try to insert the terminator symbol at the position [length + 1]. Be sure
 * to allocate a sufficient ammount of memory.
 * Params:
 * 		char* string: the string to be terminated
 * 		int length: the length of the string
 */
void zeroTerminate(char* string, int length) {
	string[length + 1] = '\0';
}
