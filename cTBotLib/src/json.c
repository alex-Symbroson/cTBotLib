
#include "json.h"
#include "macros.h"

#include <string.h>
#include <stdlib.h>

	//returns dynamically allocated string
char* _json_get(jsmntok_t* token, char* path, char* json) {
	BEGIN("jsmntok_t*token,char*path=\"%s\",char*json",path);
	switch(*path) {
		case '[': { //array
			uint32_t n = 0, i = 1, len, size = token->size;
				//string to int of "[i]"
			while(*++path && *path != ']') n = (n<<3) + (n<<1) + *path - '0';
			if(n >= size) { //index overflow
				error_exit("json error: index %i does not exist", n);
				return "";
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
			return _json_get(token+i, path+1, json);
		} break;
		case 0: { //path empty -> finished
			char *ret = malloc(token->end - token->start);
			ret[token->end - token->start] = 0; //make pointer null terminated
			return memcpy(ret, json + token->start, token->end - token->start);
		}
		default: { //object
			uint32_t n = 0, i = 1, k = 0, end, size = token->size;
				//get length of ".property" name
			if(*path == '.') path++;
			while(path[n] && path[n] != '.' && path[n] != '[') n++;
			DEBUG("searching property \"%.*s\"", n, path);
			DEBUG("%3i start at token %i, size: %i, value token: %i, value size: %i",
					i, token[i].pos, token[i].size, token[i+1].pos, token[i+1].size);
				//search property
			INFO("max: %i", size);
			while(strncmp(path, json+token[i].start, token[i].end - token[i].start) && ++k < size) {
				//i += 2 + token[i+1].size;
				end = token[i+1].end;
				while(token[i].start < end) i++;
				DEBUG("am at token %i, size: %i, value token: %i, value size: %i",
					token[i].pos, token[i].size, token[i+1].pos, token[i+1].size);
			}
			if(k >= size) { //property doesn't exist
				error_exit("json error: couldn't find property \"%.*s\"", n, path);
				return "";
			}

			DEBUG("return token %i of size %i from char %i to %i",
				token[i+1].pos, token[i+1].size, token[i+1].start, token[i+1].end);
				//process next value in path
			return _json_get(token+i+1, path+n, json);
		}
	}
}

uint32_t json_getLength(JSON v, char* path) {
	BEGIN("JSON v,char*path=\"%s\"",path);
	JSON w;
	json_init(w, json_get(v, path));
	uint32_t ret = w.tokens[0].size;
	json_free(w);
	return ret;
};
