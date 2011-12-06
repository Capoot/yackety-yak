/**
 * A set of convenience functions for handling messages
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void bytewiseStringCopy(const char* src, char* dest, int size) {
	for(int i=0; i<size; i++) {
		dest[i] = src[i];
	}
	dest[size] = '\0';
}

void readSayParams(YakMessage* msg, char** text) {
	*text = malloc(msg->header.dataSize * sizeof(char) + 1);
	bytewiseStringCopy((char*)msg->data, *text, msg->header.dataSize);
}

int readHelloParams(YakMessage* msg, char** name, char** password) {
	HelloParams* params = &msg->header.params.helloParams;
	*name = (char*)malloc(params->nameLength * sizeof(char) + 1);
	*password = (char*)malloc(params->passwordLength * sizeof(char) + 1);
	bytewiseStringCopy((char*)&msg->data[params->nameStart], *name, params->nameLength);
	bytewiseStringCopy((char*)&msg->data[params->passwordStart], *password, params->passwordLength);
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
	*reason = 0;
	*reason += ((int)msg->data[0]);
	*reason += ((int)msg->data[1]) >> 8;
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

void readSaysParams(YakMessage* msg, char** name, char** text, int* isWhisper) {
	SaysParams* params = &msg->header.params.saysParams;
	*name = (char*)malloc((params->nameLength + 1) * sizeof(char));
	*text = (char*)malloc((params->textLength + 1) * sizeof(char));
	*isWhisper = params->isWhisper;
	bytewiseStringCopy((char*)&msg->data[params->nameStart], *name, params->nameLength);
	bytewiseStringCopy((char*)&msg->data[params->textStart], *text, params->textLength);
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
