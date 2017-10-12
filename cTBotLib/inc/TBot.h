
#ifndef _TBOT_H_
#define _TBOT_H_

#include "request.h"

void TBot_init(char* token);
void TBot_destroy();

uint8_t _TBot_checkOk(char* result, char* print);
char* _TBot_getUpdates(char* offset);

char* TBot_send(
	char* func,     //function name (i.e. "sendMessage")
	char* chat_id,  //target where to send message
	char* msg_id,   //needed for editing specific messages
	char* text,     //your message text
	char* reply_id, //needed for replying to specific messages
	char* markup);  //for inline keyboards

#define __TBot_checkOk(result, print,...) _TBot_checkOk(result,print)
#define TBot_checkOk(...) __TBot_checkOk(__VA_ARGS__,"","")

#define TBot_getUpdates(...) _TBot_getUpdates(0##__VA_ARGS__?__VA_ARGS__:"")

#define __TBot_sendMessage(chat_id,text,reply_id,markup,...)\
	TBot_send("sendMessage",chat_id,"",text,reply_id,markup)
#define TBot_sendMessage(...) __TBot_sendMessage(__VA_ARGS__,"","","","")

#define __TBot_edtMsg(chat_id,msg_id,text,reply_id,markup,...)\
	TBot_send("editMessageText",chat_id,msg_id,text,reply_id,markup)
#define TBot_editMessageText(...) __TBot_edtMsg(__VA_ARGS__,"","","","","")

#endif
