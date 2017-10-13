
#include "json.h"
#include "macros.h"

#include <string.h>
#include <stdlib.h>

	//returns dynamically allocated string
jsmntok_t* json_goto(jsmntok_t* token, char* path, char* json) {
	BEGIN("jsmntok_t*token,char*path=\"%s\",char*json",path);
	switch(*path) {
		case '[': { //array
			DEBUG("have array");
			uint32_t n = 0, i = 1, len, size = token->size;
				//string to int of "[i]"
			while(*++path && *path != ']') n = (n<<3) + (n<<1) + *path - '0';
			if(n >= size) { //index overflow
				error_exit("json error: index %i does not exist", n);
				return token;
			}

			DEBUG("searching index [%i]", n);
				//jump to list[n]
			while(n--) {
				//i += 1 + token[i].size;
				len = token[i].end;
				while(token[i].start < len) i++;
			}

				//process next value in path
			DEBUG("return token %i of size %i from char %i to %i",
				token[i].pos, token[i].size, token[i].start, token[i].end);
			return json_goto(token+i, path+1, json);
		} break;
		case 0: DEBUG("have primitive"); return token;
		default: { //object
			DEBUG("have object");
			uint32_t n = 0, i = 1, k = 0, end, size = token->size;
				//get length of ".property" name
			if(*path == '.') path++;
			while(path[n] && path[n] != '.' && path[n] != '[') n++;
			DEBUG("searching property \"%.*s\"", n, path);
			DEBUG("%3i start at token %i, size: %i, value token: %i, value size: %i",
					i, token[i].pos, token[i].size, token[i+1].pos, token[i+1].size);
				//search property
			while(strncmp(path, json+token[i].start, token[i].end - token[i].start) && ++k < size) {
				//i += 2 + token[i+1].size;
				end = token[i+1].end;
				while(token[i].start < end) i++;
				DEBUG("am at token %i, size: %i, value token: %i, value size: %i",
					token[i].pos, token[i].size, token[i+1].pos, token[i+1].size);
			}
			if(k >= size) { //property doesn't exist
				error_exit("json error: couldn't find property \"%.*s\"", n, path);
				return token;
			}

			DEBUG("return token %i of size %i from char %i to %i",
				token[i+1].pos, token[i+1].size, token[i+1].start, token[i+1].end);
				//process next value in path
			return json_goto(token+i+1, path+n, json);
		}
	}
	END();
}

char* json_get(JSON v, char* path) {
	BEGIN("JSON v=\"%s\", char* path=\"%s\"",v.json,path);
	jsmntok_t* token = json_goto(v.tokens, path, v.json);
	char *ret = malloc(token->end - token->start);
	ret[token->end - token->start] = 0;
	END();
	return memcpy(ret, v.json + token->start, token->end - token->start);
}

void json_forEach(void(*callback)(JSON), JSON v, char* path) {
	BEGIN("void(*callb)(JSON),JSON v,char*path=\"%s\"",path);
	jsmntok_t* token = json_goto(v.tokens, path, v.json);
	jsmntype_t type = token->type;

	if(type == JSMN_ARRAY || type == JSMN_OBJECT) {
		uint32_t i = 0, p = 1, size = token->size;
		for(; i < size; i++) {
			if(type == JSMN_OBJECT) p++;

			char *json = malloc(token[p].end - token[p].start);
			json = memcpy(json, v.json + token[p].start, token[p].end - token[p].start);
			json[token[p].end - token[p].start] = 0;

			JSON arg;
			json_init(arg, json);
			callback(arg);
			free(arg.tokens); //json_free(arg);
			free(json);

			uint32_t end = token[p++].end;
			while(token[p].start < end) p++;
		}
	}
	END();
}
