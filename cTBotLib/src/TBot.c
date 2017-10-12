
#include "TBot.h"
#include "macros.h"
#include "json.h"
#include "request.h"

#include <stdio.h>

struct {
		//socket related stuff
	uint16_t default_port;
	char* token;
	char* offset;
	Socket socket;
} TBot = { .default_port = 443, .offset="", .socket.active = 0 };

void writeFile(char* file, char* text) {
	BEGIN("char*file=\"%s\",char*text=\"%s\"",file,text);
	FILE *f = fopen(file, "w");
	uint32_t i = 0;
	while(text[i]) fputc(text[i++], f);
	fclose(f);
}

char* readFile(char* file) {
	BEGIN("char*file=\"%s\"",file);
	FILE *f = fopen(file, "r");
	if(f == NULL) return "";

	uint32_t len = 0, i = 0;
	int16_t c;
	char *buf = malloc(0);

	while((c = fgetc(f)) != EOF) {
		if(i <= len) buf = realloc(buf, len += 10);
		buf[i++] = c;
	}

	fclose(f);
	buf = realloc(buf, i+1);
	buf[i] = 0;
	return buf;
}

void TBot_init(char* token) {
	BEGIN("char*token=\"%s\"",token);

	SSL_library_init();
	TBot.token = token;
	if(strlen(TBot.offset)) free(TBot.offset);
	TBot.offset = readFile("offset");
	Socket_init(&TBot.socket, "api.telegram.org", TBot.default_port);
}

uint8_t _TBot_checkOk(char* result, char* print) {
	BEGIN("char*result=\"%s\",char*print=%s",result,*print?"true":"false");

	JSON res;
	json_init(res, result);

	if(strncmp(json_get(res, "ok"), "true", 4)) {
		error("telegram error %s: %s", json_get(res, "error_code"), json_get(res, "description"));
		json_free(res);
		return 0;
	} else {
		json_free(res);
		return 1;
	}
}

char* _TBot_getUpdates(char* offset) {
	BEGIN("char*offset=%s",offset);
	if(!*offset) offset = TBot.offset;

	uint32_t ioffset = 0;
	uint8_t i = 0;
	while(offset[i])
		ioffset = ioffset*10 + offset[i++] - '0';

	char* base = "/bot%s/getUpdates?offset=%u";
	base = malloc(strlen(base) + strlen(TBot.token) + strlen(offset));
	sprintf(base, "/bot%s/getUpdates?offset=%u", TBot.token, ioffset + 1);
	printf("BASE: \"%s\"\n", base);
	char* result = extractBody(Socket_send(&TBot.socket, base));
	free(base);

		//get and save latest offset
	if(_TBot_checkOk(result, "")) {
		JSON v;
		json_init(v, result);
		uint8_t len = json_getLength(v, "result");
		if(len) {
			char path[21] = "result[%i].update_id";
			sprintf(path, path, len - 1);
			writeFile("offset", json_get(v, path));
		}
		json_free(v);
	}

	return result;
}

char* TBot_send(
	char* func, char* chat_id,  char* message_id,
	char* text, char* reply_id, char* markup) {
	BEGIN("...");

	char* base = "/bot%s/%s?";
	base = malloc((strlen(base) + strlen(TBot.token) + strlen(func)));
	sprintf(base, "/bot%s/%s?", TBot.token, func);

#define addArg(v)\
	if(strlen(v)) {\
		base = realloc(base, (strlen(base) + strlen(""#v) + strlen(v))*sizeof(char)+3);\
		sprintf(base, "%s%s=%s&", base, ""#v, v);\
	}

	addArg(chat_id);
	addArg(message_id);
	addArg(text);
	addArg(reply_id);
	addArg(markup);
	INFO("%s", base);

	char* result = Socket_send(&TBot.socket, base);
	free(base);

	return extractBody(result);
}

void TBot_destroy() {
	BEGIN();
	if(strlen(TBot.offset)) free(TBot.offset);
	Socket_close(&TBot.socket);
}
