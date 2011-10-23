/*
 * Defines the Yakkety-Yak protocol
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_
#endif

#define BYTE unsigned char

const int YAK_VERSION = 1;

typedef enum {
	HELLO,
	WELCOME,
	REJECTED,
	SAY,
	WHISPER,
	BYE,
	GOTO,
	GIVE_NAMES,
	NAMES,
	ADD_NAME,
	REMOVE_NAME,
	ROGER
} MessageType;

typedef struct {
	int nameStart;
	int nameLength;
	int passwordStart;
	int passwordLength;
} HelloParams;

typedef struct {
	int toStart;
	int toLength;
	int messageStart;
	int messageLength;
} WhisperParams;

typedef struct {
	int namesCount;
} NamesParams;

typedef struct {
	MessageType type;
	int version;
	int dataSize;
	union {
		HelloParams helloParams;
		WhisperParams whisperParams;
		NamesParams namesParams;
	} params;
} YakHeader;

typedef struct {
	YakHeader header;
	BYTE* data;
} YakMessage;

/*** Client Message Factories ***/
YakMessage* createHelloMessage(char*, char*);
YakMessage* createSayMessage(char*); // FIXME wie kommt der text zum client?? ->textmessage
YakMessage* createWhisperMessage(char*, char*);
YakMessage* createRogerMessage(char*);
YakMessage* createByeMessage();

/*** Server Message Factories ***/
YakMessage* createWelcomeMessage();
YakMessage* createRejectedMessage(int);
YakMessage* createGotoMessage(long int);
YakMessage* createGiveNamesMessage();
YakMessage* createNamesMessage(char**, int);
YakMessage* createAddNameMessage(char*);
YakMessage* createRemoveNameMessage(char*);

/*** Message Utility Functions ***/
void deleteMessage(YakMessage*);
