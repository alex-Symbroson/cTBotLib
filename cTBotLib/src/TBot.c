
#include "TBot.h"
#include "macros.h"
#include "json.h"
#include "request.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct {
		//socket related stuff
	uint16_t default_port;
	char* token;
	char* offset;
	Socket socket;
	pthread_t thread;
	uint8_t thread_active;
	uint32_t update_interval;
} TBot = { .default_port = 443, .offset="", .socket.active = 0, .thread_active = 0, .update_interval = 500 };

void writeFile(char* file, char* text) {
	BEGIN("char*file=\"%s\",char*text=\"%s\"",file,text);
	FILE *f = fopen(file, "w");
	uint32_t i = 0;
	while(text[i]) fputc(text[i++], f);
	fclose(f);
	END();
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
	END();
	return buf;
}

void TBot_init(char* token) {
	BEGIN("char*token=\"%s\"",token);

	SSL_library_init();
	TBot.token = token;
	if(strlen(TBot.offset)) free(TBot.offset);
	TBot.offset = readFile("offset");
	Socket_init(&TBot.socket, "api.telegram.org", TBot.default_port);
	END();
}

uint8_t _TBot_checkOk(JSON v, char* print) {
	BEGIN("JSON v,char*print=%s",*print?"true":"false");

	if(strncmp(json_get(v, "ok"), "true", 4)) {
		error("telegram error %s: %s", json_get(v, "error_code"), json_get(v, "description"));
		END();
		return 0;
	}
	END();
	return 1;
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
	char* result = extractBody(Socket_send(&TBot.socket, base));
	free(base);

	JSON v;
	json_init(v, result);

		//get and save latest offset
	if(TBot_checkOk(v, "")) {
		uint8_t len = json_getLength(v, "result");
		if(len) {
			char path[21] = "result[%i].update_id";
			sprintf(path, path, len - 1);
			TBot.offset = json_get(v, path);
			writeFile("offset", TBot.offset);
		}
	}

	json_free(v);
	END();
	return result;
}

void* _TBot_onUpdate(void* foo) {
	void(*callback)(JSON) = (void(*)(JSON))foo;
	while(1) {
		BEGIN("while loop");
		char* result = _TBot_getUpdates(TBot.offset);
		JSON v;
		json_init(v, result);
		callback(v);
		json_free(v);
		delay_ms(TBot.update_interval);
		free(result);
	}
	END();
	return NULL;
}

void _TBot_setOnUpdate(void (*callback)(JSON), uint32_t interval) {
	BEGIN("void(*callb)(JSON),uint32_t itv=%u", interval);
	TBot.thread_active = 1;
	TBot.update_interval = interval;
	pthread_create(&TBot.thread, NULL, &_TBot_onUpdate, (void*)callback);
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

	char* result = Socket_send(&TBot.socket, base);
	free(base);
	END();
	return extractBody(result);
}

void TBot_destroy() {
	BEGIN();
	if(strlen(TBot.offset)) free(TBot.offset);
	if(TBot.thread_active) pthread_cancel(TBot.thread);
	Socket_close(&TBot.socket);
	END();
}
