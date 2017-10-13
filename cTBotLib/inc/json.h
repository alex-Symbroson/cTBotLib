
#ifndef _JSON_H_
#define _JSON_H_

#include "jsmn.h"

#include <stdint.h>

typedef struct {
	jsmn_parser parser;
	jsmntok_t* tokens;
	char* json;
	int len;
} JSON;

	//returns dynamically allocated string
char* json_get(JSON v, char* path);
jsmntok_t* json_goto(jsmntok_t*, char*, char*);
void json_forEach(void(*)(JSON), JSON, char*);

#define json_getLength(v,p) json_goto(v.tokens,p,v.json)->size

#define json_init(v,s)\
	jsmn_init(&v.parser);\
	v.json = s;\
	v.tokens = malloc(1000*sizeof(jsmntok_t));\
	v.len = jsmn_parse(&v.parser, s, strlen(s), v.tokens, 1000);\
	do { int i = v.len; while(i--) v.tokens[i].pos = i; } while(0);\
	v.tokens = realloc(v.tokens, sizeof(jsmntok_t)*v.len)

#define json_free(v)\
	if(v.len) free(v.tokens), v.len = 0

#define PRINT_JSON_INFO(v) do {\
		INFO("%s", v.json); int i;\
		for(i=0;i<v.len;i++)\
			INFO("%.*s%i-%i", v.tokens[i].start, SPACE, v.tokens[i].pos, v.tokens[i].size);\
	} while(0)

#define SPACE "                                                                                                                                                                                                                                                                                                 "\
	"                                                                                                                                                                                                                                                                                                           "\
	"                                                                                                                                                                                                                                                                                                           "\
	"                                                                                                                                                                                                                                                                                                           "\
	"                                                                                                                                                                                                                                                                                                           "

#endif
