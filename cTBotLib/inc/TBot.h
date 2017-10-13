
#ifndef _TBOT_H_
#define _TBOT_H_

#include "request.h"
#include "json.h"

#include <unistd.h>

void TBot_init(char* token);
void TBot_destroy();

void writeFile(char* file, char* text);
char* readFile(char* file);

void delay_mys(uint32_t t);
#define delay_mys(t) usleep(t)
#define delay_ms(t) usleep(1000*t)
#define delay_s(t) usleep(1000000*t)

#define TBot_random(max) (srand(clock()), rand()%max)

uint8_t _TBot_checkOk(JSON v, char* print);

char* _TBot_getUpdates(char* offset);
#define TBot_getUpdates(...) _TBot_getUpdates(0##__VA_ARGS__?__VA_ARGS__:"")

	//TBot_setOnUpdate(void(*)(JSON), uint32_t interval=1000)
void _TBot_setOnUpdate(void(*callback)(JSON), uint32_t interval);
#define __TBot_setOnUpdate(onUpdate,interval,...) _TBot_setOnUpdate(onUpdate,interval)
#define TBot_setOnUpdate(onUpdate,...) __TBot_setOnUpdate(&onUpdate,__VA_ARGS__,1000)

char* TBot_send(
	char* func,     //function name (i.e. "sendMessage")
	char* chat_id,  //target where to send message
	char* msg_id,   //needed for editing specific messages
	char* text,     //your message text
	char* reply_id, //needed for replying to specific messages
	char* markup);  //for inline keyboards

	//TBot_checkOk(JSON v, char* print="")
#define __TBot_checkOk(v, print,...) _TBot_checkOk(v,print)
#define TBot_checkOk(...) __TBot_checkOk(__VA_ARGS__,"")

	//TBot_sendMessage(char *chat_id, char *text, char *reply_id="", char *markup="")
#define __TBot_sendMessage(chat_id,text,reply_id,markup,...)\
	TBot_send("sendMessage",chat_id,"",text,reply_id,markup)
#define TBot_sendMessage(...) __TBot_sendMessage(__VA_ARGS__,"","")

	//TBot_editMessageText(char *chat_id, char *msg_id="", char *text, char *reply_id="", char *markup="")
#define __TBot_edtMsg(chat_id,msg_id,text,reply_id,markup,...)\
	TBot_send("editMessageText",chat_id,msg_id,text,reply_id,markup)
#define TBot_editMessageText(...) __TBot_edtMsg(__VA_ARGS__,"","","","","")

#endif
