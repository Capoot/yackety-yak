#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define BYTE unsigned char
#define YAK_VERSION 1

typedef enum {
	HELLO,
	WELCOME,
	REJECTED,
	SAY,
	SAYS,
	WHISPER,
	BYE,
	GOTO,
	GIVE_NAMES,
	NAMES,
	ADD_NAME,
	REMOVE_NAME,
	ROGER,
	IDENTIFY
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
	int nameStart;
	int nameLength;
	int textStart;
	int textLength;
	int isWhisper;
} SaysParams;

typedef struct {
	MessageType type;
	int version;
	int dataSize;
	union Params {
		HelloParams helloParams;
		WhisperParams whisperParams;
		NamesParams namesParams;
		SaysParams saysParams;
	} params;
} YakHeader;

typedef struct {
	YakHeader header;
	BYTE* data;
} YakMessage;

/*** Message Factories ***/
YakMessage* createHelloMessage(char*,char*);
YakMessage* createSayMessage(char*);
YakMessage* createWhisperMessage(char*, char*);
YakMessage* createSaysMessage(char*, char*, int);
YakMessage* createRogerMessage(char*);
YakMessage* createByeMessage(void);
YakMessage* createWelcomeMessage(void);
YakMessage* createRejectedMessage(int);
YakMessage* createGotoMessage(long int);
YakMessage* createGiveNamesMessage(void);
YakMessage* createNamesMessage(char**, int);
YakMessage* createAddNameMessage(char*);
YakMessage* createRemoveNameMessage(char*);
YakMessage* createIdentifyMessage(unsigned char);

/*** Message Utility Functions ***/
void deleteMessage(YakMessage*);
int readSayParams(YakMessage*, char*);
int readHelloParams(YakMessage*, char**, char**);
int readWhisperParams(YakMessage*, char*, char*);
int readRogerParams(YakMessage*, char*);
int readRejectedParams(YakMessage*, int*);
int readGotoParams(YakMessage*, long int*);
int readNamesParams(YakMessage*, char**, int*);
int readAddNameParams(YakMessage*, char*);
int readRemoveNameParams(YakMessage*, char*);
int readSaysParams(YakMessage*, char*, char*, int*);

#endif
