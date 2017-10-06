
#include "jsmn.h"

typedef struct {
	jsmn_parser parser;
	jsmntok_t* tokens;
	char* json;
	int len;
} JSON;

	//returns dynamically allocated string
#define json_get(v,p) _json_get(v.tokens ,p, v.json)
char* _json_get(jsmntok_t* token, char* path, char* json);

#define json_init(v,s)\
	jsmn_init(&v.parser);\
	v.json = s;\
	v.tokens = malloc(1000*sizeof(jsmntok_t));\
	v.len = jsmn_parse(&v.parser, s, strlen(s), v.tokens, 1000);\
	do { int i = v.len; while(i--) v.tokens[i].pos = i; } while(0);\
	v.tokens = realloc(v.tokens, sizeof(jsmntok_t)*v.len)

#define json_free(v)\
	if(v.len) free(v.tokens)

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
