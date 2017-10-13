
#include "macros.h"
#include "TBot.h"
#include "json.h"

#include <stdio.h>

#define TOKEN "000000000:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define ME "000000000"

volatile uint8_t running;

void handleUpdate(JSON msg) {
	BEGIN("JSON msg=\"%s\"",msg.json);

	char* text = json_get(msg, "message.text");
	char* chat_id = json_get(msg, "message.chat.id");

	if(!strncmp("/dice", text, 5)) {
		char dice[1] = {'1' + TBot_random(6)};
		TBot_sendMessage(chat_id, dice);
	} else
	if(!strncmp("/stop", text, 5)) {
		INFO("Bot stopped!");
		running = 0;
	}

	free(text);
	free(chat_id);
	END();
}

void onUpdate(JSON result) {
	BEGIN("JSON result=\"%s\"", result.json);
	json_forEach(handleUpdate, result, "result");
	END();
}

int main(int argc,char *argv[]) {
	BEGIN("int argc=%i,char *argv[]",argc);

	INFO("initialisize bot");
	TBot_init(TOKEN);

	running = 1;
	TBot_setOnUpdate(onUpdate, 1000);

	INFO("listening...");
	while(running) usleep(50);

	TBot_destroy();
	END();
	return 0;
}
