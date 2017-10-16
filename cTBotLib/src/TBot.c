
#include "TBot.h"
#include "json.h"
#include "macros.h"
#include "request.h"

#include <time.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>

struct {
	char* token;
	char* offset;
	CURL* curl;
	pthread_t thread;
	uint32_t update_interval;
} TBot = { .offset="", .thread = 0, .update_interval = 500 };

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
		if(i <= len) buf = realloc(buf, ++len);
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

	if(curl_global_init(CURL_GLOBAL_DEFAULT) || !(TBot.curl = curl_easy_init()))
		error_exit("couldn't init curl");

	TBot.token = token;
	TBot.offset = readFile("offset");
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
	while(offset[i]) ioffset = ioffset*10 + offset[i++] - '0';

	char* url = malloc(48 + strlen(TBot.token) + strlen(offset));
	sprintf(url, "https://api.telegram.org/bot%s/getUpdates?offset=%u", TBot.token, ioffset + 1);
	curl_easy_setopt(TBot.curl, CURLOPT_URL, url);
	free(url);

	char* result = sendRequest(TBot.curl, NULL);
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
	TBot.update_interval = interval;
	pthread_create(&TBot.thread, NULL, &_TBot_onUpdate, (void*)callback);
	END();
}

char* TBot_send(char* func, char* chat_id, char* message_id,
	char* text, char* file, char* disable_notification, char* reply_markup, char* special) {
	BEGIN("...");

		//concat main url
	char* buf = malloc(30 + strlen(TBot.token) + strlen(func));
	sprintf(buf, "https://api.telegram.org/bot%s/%s", TBot.token, func);
	curl_easy_setopt(TBot.curl, CURLOPT_URL, buf);
	free(buf);

		//init formdata for request
	struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;

	#define addArg(n) if(*n) curl_formadd(&post, &last, CURLFORM_COPYNAME, ""#n, CURLFORM_COPYCONTENTS, n, CURLFORM_END)

	if(strcmp(special, "custom")) {
		addArg(chat_id);
		addArg(reply_markup);
		addArg(disable_notification);

		if(strncmp(func, "send", 4)) {
			addArg(message_id);
		} else curl_formadd(&post, &last, CURLFORM_COPYNAME, "reply_to_message_id", CURLFORM_COPYCONTENTS, message_id, CURLFORM_END);

		if(*file) {
				//use caption key instead of text
			if(*text) curl_formadd(&post, &last, CURLFORM_COPYNAME, "caption", CURLFORM_COPYCONTENTS, text, CURLFORM_END);

				//extract ie. 'sticker' from 'sendSticker'
			buf = malloc(strlen(func+4));
			sprintf(buf, "%c%s", func[4] + 32, func + 5);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, buf, CURLFORM_FILE, file, CURLFORM_END);
			free(buf);
		} else addArg(text);

			//add special arguments
		addSpecials:
		if(*special) {
			JSON v;
			json_init(v, special);
			uint32_t i = 1, end = v.tokens[2].end, size = v.tokens->size, len;
			char* key = malloc(1);
			buf = malloc(1);

			while(size--) {
					//get key
				len = v.tokens[i].end - v.tokens[i].start + 1;
				key = realloc(key, len);
				snprintf(key, len, "%s", special + v.tokens[i].start);

					//get value
				len = v.tokens[i+1].end - v.tokens[i+1].start + 1;
				buf = realloc(buf, len);
				snprintf(buf, len, "%s", special + v.tokens[i+1].start);

					//add to form if value not empty
				if(len) curl_formadd(&post, &last, CURLFORM_COPYNAME, key, CURLFORM_COPYCONTENTS, buf, CURLFORM_END);

					//jump to next key
				end = v.tokens[i+1].end;
				while(v.tokens[i].start < end) i++;
			}
			free(buf);
			free(key);
			json_free(v);
		}
	} else {
		#define addCArg(k,n) if(*n) curl_formadd(&post, &last, CURLFORM_COPYNAME, k, CURLFORM_COPYCONTENTS, n, CURLFORM_END)

		if(!strcmp(func, "getStickerSet")) {
			addCArg("name", chat_id);
		} else
		if(!strcmp(func, "uploadStickerFile")) {
			addCArg("user_id", chat_id);
			addCArg("png_sticker", message_id);
		} else
		if(!strcmp(func, "createNewStickerSet")) {
			addCArg("user_id", chat_id);
			addCArg("name", message_id);
			addCArg("title", text);
			addCArg("png_sticker", file);
			addCArg("emojis", disable_notification);
			if(*reply_markup) {
				special = reply_markup;
				goto addSpecials;
			}
		} else
		if(!strcmp(func, "addStickerToSet")) {
			addCArg("user_id", chat_id);
			addCArg("name", message_id);
			addCArg("png_sticker", text);
			addCArg("emojis", file);
			addCArg("mask_position", disable_notification);
		} else
		if(!strcmp(func, "setStickerPositionInSet")) {
			addCArg("sticker", message_id);
			addCArg("position", text);
		} else
		if(!strcmp(func, "deleteStickerFromSet")) {
			addCArg("sticker", message_id);
		} else
		if(!strcmp(func, "answerInlineQuery")) {
			addCArg("inline_query_id", message_id);
			addCArg("results", text);
			if(*reply_markup) {
				special = reply_markup;
				goto addSpecials;
			}
		}
	}

	END();
		//send and return request
	return sendRequest(TBot.curl, post);
}

void TBot_destroy() {
	BEGIN();
	if(*TBot.offset) free(TBot.offset);
	if(TBot.thread) pthread_cancel(TBot.thread);
	curl_easy_cleanup(TBot.curl);
	END();
}
