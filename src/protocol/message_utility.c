
#include <stdlib.h>

#include "protocol.h"

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
