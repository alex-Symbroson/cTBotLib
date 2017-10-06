
#include "macros.h"
#include "TBot.h"
#include "JSON.h"

#include <stdio.h>

#define TOKEN "000000000:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define ME "000000000"

int main(int argc,char *argv[]) {
	BEGIN("int argc=%i,char *argv[]",argc);
	INFO(STATUS_S, STATUS_A);

	TBot_init(TOKEN);
	char* res = TBot_sendMessage(ME, "this is awesome!");
	printf("%s\n\n", res);

	JSON v;
	json_init(v,res);

	if(strncmp(json_get(v, "ok"), "true", 4))
		error("telegram error %s: %s",
			json_get(v, "error_code"), json_get(v, "description"));
	else {
		if(*json_get(v, "result.chat.username"))
			printf("%s -> %s: \"%s\"\n",
				json_get(v, "result.from.username"),
				json_get(v, "result.chat.username"),
				json_get(v, "result.text")
			);
		else
			printf("%s -> %s: \"%s\"\n",
				json_get(v, "result.from.username"),
				json_get(v, "result.chat.id"),
				json_get(v, "result.text")
			);
	}

	TBot_destroy();
	json_free(v);
	return 0;
}
