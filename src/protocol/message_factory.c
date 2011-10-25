
#include <string.h>
#include <stdlib.h>

#include "protocol.h"

/*** Private Functions ***/

void copyStringToData(int dataIndex, char* string, YakMessage* msg) {
	int length = strlen(string);
	for(int i=0; i<length; i++) {
		msg->data[dataIndex++] = (BYTE)string[i];
	}
}

int countChars(char** strings, int size) {

	if(strings == NULL) {
		return 0;
	}

	int count = 0;
	for(int i=0; i<size; i++) {
		count += strlen(strings[i]);
	}

	return count;
}

/*** Public Functions ***/

YakMessage* createHelloMessage(char* name, char* password) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));
	int nameLength = strlen(name);
	int passwordLength = strlen(password);

	msg->header.params.helloParams.nameStart = 0;
	msg->header.params.helloParams.nameLength = nameLength;
	msg->header.params.helloParams.passwordStart = nameLength;
	msg->header.params.helloParams.passwordLength = passwordLength;
	msg->header.type = HELLO;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = (nameLength + passwordLength) * sizeof(char);
	msg->data = (BYTE*)malloc(msg->header.dataSize * sizeof(BYTE));

	copyStringToData(0, name, msg);
	copyStringToData(nameLength, password, msg);

	return msg;
}

YakMessage* createWelcomeMessage() {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));

	msg->header.type = WELCOME;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = 0;
	msg->data = NULL;

	return msg;
}

YakMessage* createRejectedMessage(int reason) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));

	msg->header.type = REJECTED;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = sizeof(int);
	msg->data = (BYTE*)malloc(sizeof(int));
	msg->data[0] = reason;

	return msg;
}

YakMessage* createSayMessage(char* message) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));
	int length = strlen(message);

	msg->header.type = SAY;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = length * sizeof(char);
	copyStringToData(0, message, msg);

	return msg;
}

YakMessage* createWhisperMessage(char* to, char* message) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));
	int messageLength = strlen(message);
	int toLength = strlen(to);

	msg->header.params.whisperParams.toStart = 0;
	msg->header.params.whisperParams.toLength = toLength;
	msg->header.params.whisperParams.messageStart = toLength;
	msg->header.params.whisperParams.messageLength = messageLength;
	msg->header.type = WHISPER;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = (messageLength + toLength) * sizeof(char);
	copyStringToData(0, to, msg);
	copyStringToData(toLength, message, msg);

	return msg;
}

YakMessage* createByeMessage() {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));

	msg->header.type = BYE;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = 0;
	msg->data = NULL;

	return msg;
}

YakMessage* createGotoMessage(long int target) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));

	msg->header.type = GOTO;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = sizeof(long int);
	msg->data = (BYTE*)malloc(sizeof(long int));
	msg->data[0] = target;

	return msg;
}

YakMessage* createGiveNamesMessage() {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));

	msg->header.type = GIVE_NAMES;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = 0;
	msg->data = NULL;

	return msg;
}

YakMessage* createNamesMessage(char** names, int namesCount) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));
	int size = countChars(names, namesCount);

	msg->header.type = NAMES;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = size * (sizeof(char) + sizeof(int));
	msg->data = (BYTE*)malloc(size * (sizeof(char) + sizeof(int)));

	int index = 0;
	for(int i=0; i<namesCount; i++) {
		int len = strlen(names[i]);
		msg->data[index++] = (BYTE)len;
		copyStringToData(index, names[i], msg);
		index += len;
	}

	return msg;
}

YakMessage* createAddNameMessage(char* name) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));
	int len = strlen(name);

	msg->header.type = ADD_NAME;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = len * sizeof(char);
	msg->data = (BYTE*)malloc(len * sizeof(char));
	copyStringToData(0, name, msg);

	return msg;
}

YakMessage* createRemoveNameMessage(char* name) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));
	int len = strlen(name);

	msg->header.type = REMOVE_NAME;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = len * sizeof(char);
	msg->data = (BYTE*)malloc(len * sizeof(char));
	copyStringToData(0, name, msg);

	return msg;
}

YakMessage* createRogerMessage(char* message) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));

	msg->header.type = ROGER;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = 0;
	msg->data = NULL;

	return msg;
}

YakMessage* createSaysMessage(char* name, char* text, int isWhisper) {

	YakMessage* msg = (YakMessage*)malloc(sizeof(YakMessage));
	int nameLength = strlen(name);
	int textLength = strlen(text);

	msg->header.type = SAYS;
	msg->header.version = YAK_VERSION;
	msg->header.dataSize = (nameLength + textLength) * sizeof(char);
	msg->header.params.saysParams.nameLength = nameLength;
	msg->header.params.saysParams.nameStart = 0;
	msg->header.params.saysParams.textLength = textLength;
	msg->header.params.saysParams.textStart = nameLength;
	msg->header.params.saysParams.isWhisper = isWhisper;
	copyStringToData(0, name, msg);
	copyStringToData(nameLength, text, msg);

	return msg;
}
