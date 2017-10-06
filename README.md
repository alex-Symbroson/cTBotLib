
#C - Library for making Telegram-Bot's

Features
--------

- easy-to-use bot api
- currently supported Telegram api functions:
	- sendMessage
	- editMessage
- builtin json parser
	- based on jsmn parser: https://github.com/zserge/jsmn

- will be extended...


Dependencies
------------

- openssl (ssl.h, err.h, bio.h)
	- https://github.com/openssl/openssl -> /include/openssl/

Compiling
---------

I suggest using the included Makefile. Tree should be:
	```
	Makefile
	main.c
	inc/
	src/

execute '$ make' in the Terminal.
You can specify the output file name with pasing 'out=name' as argument to 'make'

Usage
-----

**Bot api:**
initialisize the bot with your Bot-Token (get it from BotFather)
	```c
	TBot_init("<TOKEN>");
	```

now you can call the api functions. They will return the response of Telegram.
Note that all arguments should be char* or const char*
	```c
	char* result = TBot_sendMessage(chat_id, "Hello World!");
	```

at the of your program destroy it. This will free dynamically
allocated variables and the socket
	```c
	TBot_destroy();
	```

**JSON parser:**
initialisize a new parser with the json-string:
	```c
	JSON json_obj;
	json_init(json_obj, json_string);
	```

get specific values using json_get. it will return "\0" if the member doesn't exist.
I've tried to make this nearly equal to like other languages access objects:
	- access lists with "[i]" (i is the index)
	- access object properties with ".propname"
	```c
	JSON obj;
	json_init(obj, "{list:[1,{value:\"nothing\"}]}");
	printf( "value: \"%s\"\n", json_get(obj, ".list[1].value"));
	```

finally free the JSON instance:
	```c
	json_free(obj);
	```