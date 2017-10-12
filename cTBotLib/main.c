
#include "macros.h"
#include "TBot.h"
#include "json.h"

#include <stdio.h>

//#define TOKEN "000000000:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
//#define ME "000000000"
#define TOKEN "369333278:AAHWPWC8IXn5AmxzAE_XIINmFeQBQyXRP88"
#define ME "280031408"

int main(int argc,char *argv[]) {
	BEGIN("int argc=%i,char *argv[]",argc);
	INFO(STATUS_S, STATUS_A);

	TBot_init(TOKEN);
	char* result;
	JSON res;

	printf("getting updates...\n");
	result = TBot_getUpdates();
	printf("%s\n\n", result);
	if(!TBot_checkOk(result, "1")) return 0;

	result = TBot_sendMessage(ME, "this is awesome!");
	printf("%s\n\n", result);
	if(!TBot_checkOk(result, "1")) return 0;

	json_init(res, result);

	result = TBot_editMessageText(ME, json_get(res, "result.message_id"), "hellooo");
	if(!TBot_checkOk(result, "1")) return 0;
	printf("%s\n\n", result);

	TBot_destroy();
	json_free(res);
	return 0;
}
