
#include "TBot.h"
#include "macros.h"
#include "request.h"

#include <stdio.h>

struct {
		//socket related stuff
	uint16_t default_port;
	char* token;
	Socket socket;
} TBot = { .default_port = 443, .socket.active = 0 };

void TBot_init(char* token) {
	BEGIN("char*token=\"%s\"",token);
	SSL_library_init();
	TBot.token = token;
	Socket_init(&TBot.socket, "api.telegram.org", TBot.default_port);
}

char* TBot_send(
	char* func, char* chat_id,  char* msg_id,
	char* text, char* reply_id, char* markup, char* full) {
	BEGIN("...");

	char* base = "/bot%s/%s?";
	base = malloc((strlen(base)+strlen(TBot.token)+strlen(func)));
	sprintf(base, "/bot%s/%s?", TBot.token, func);

#define addArg(v)\
	if(strlen(v)) {\
		base = realloc(base, (strlen(base) + strlen(""#v) + strlen(v)));\
		sprintf(base, "%s%s=%s&", base, ""#v, v);\
	}

	addArg(chat_id);
	addArg(msg_id);
	addArg(text);
	addArg(reply_id);
	addArg(markup);
	INFO("%s", base);

	if(*full) return Socket_send(&TBot.socket, base);
	else return  extractBody(Socket_send(&TBot.socket, base));
}

void TBot_destroy() {
	BEGIN();
	Socket_close(&TBot.socket);
}
