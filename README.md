
# cTBotLib

A simple C - library for making Telegram-Bot's

Features
--------

- **easy-to-use bot api:** (-> TBot.h)
	- TBot_init(token)
	- TBot_destroy()
	- TBot_getUpdates()
	- TBot_setOnUpdate(callback [,interval=1000])
	- Telegram-API functions (-> Methods.txt)

	- extra:
		- readFile(path)
		- writeFile(path, text)
		- TBot_random(max)

- **currently supported Telegram api functions:**
	- see Methods.txt

- **builtin json parser (json.h) based on jsmn parser:** (-> https://github.com/zserge/jsmn)
	- type 'JSON'
	- json_init(json_obj, json_str)
	- json_free(json_obj)
	- json_get(json_obj, path)
	- json_forEach(callback, json_obj, path)
	- json_getLength(json_obj, path)

- **debug macros** (-> macros.h)
	- debugging
		- level 0: no debug output
		- level 1: INFO(format, ...) -> turkis
		- level 2: BEGIN(args, ...) -> green "filename functionname(" args ")"
	               END(args, ...) -> same as begin but with red * instead of yellow
		- level 3: DEBUG(format, ...) -> yellow
	- errors
		- error(format, ...) -> red
		- error_exit(format, ...) -> red, calls exit() if ERR_EXIT is 1
	- other
		- wait_enter() -> secure method to wait for enter

- **don't care about update offsets** - TBotLib does it for you!

- **more functions will come**...


Dependencies
------------

- **curl** (-> https://github.com/curl sudo apt-get install libcurl4-gnutls-dev)

Compiling
---------

I suggest using the included Makefile (execute '$ make' in the Terminal)
It generates the object files in the 'build/' dir and linkks them together to
the object file lcTBotLib. Additional it compiles the 'main.c' file to 'main.out'
You can specify the output file name with passing 'out=name' as argument to 'make'

Usage
-----

**Bot api:**
Initialisize the bot with your bot-token (get it from BotFather)

```c
TBot_init("<TOKEN>");
```

Specify a function which was called when an update arrives. This method will
call TBot_getUpdates in a given interval in an other thread (parallelized) and
calls the callback function with the response.

```c
	//calls 'void onUpdate(JSON result);' every second
TBot_setOnUpdate(onUpdate, 1000);
```

To avoid your program is leaving the main function and terminate the program
you should define a global variable 'volatile uint8_t running' and add following
line after TBot_setOnUpdate:

```
while(running) usleep(50);
```

Because the result is an array of messages you would have to iterate through the
json list. This is what json_forEach method does for you.

```c
	//calls 'void handleUpdate(JSON msg);' for every message object
json_forEach(handleUpdate, result, "result");
```

You can call the api functions with TBot_<api_function>. They will return the
response of Telegram. Note that all arguments should have the type 'char*'!
Every function has an additional 'special' option which accepts additional
arguments as a json string object ("{key:value}").
Parameters which are optional in telegram are optional in cTBotLib too.

```c
char* result = TBot_sendMessage(chat_id, "Hello World!");
```

If a method wasn't supported directly by the cTBotLib macros you can use the
TBot_sendCustom() method instead, which accepts the method name, chat_id and
the 'special' json string object.

At the of your program you should destroy it to free up allocated memory

```c
TBot_destroy();
```

**JSON parser:**
initialisize a new parser with the json-string

```c
JSON json_obj;
json_init(json_obj, json_string);
```

Get specific values using json_get. it will return "\0" if the member doesn't exist.
I've tried to make this nearly equal to like other languages access objects:
	- access lists with "[i]" (i is the index)
	- access object properties with ".propname"

```c
JSON obj;
json_init(obj, "{list:[1,{value:\"nothing\"}]}");
printf( "value: \"%s\"\n", json_get(obj, ".list[1].value"));
```

You can iterate through lists or object values using json_forEach. The first
argument is the function which was called for every value in the json object.

```c
	//calls 'void handleUpdate(JSON msg);' for every message object
json_forEach(handleUpdate, result, "result");
```

Finally free the JSON instance

```c
json_free(obj);
```
