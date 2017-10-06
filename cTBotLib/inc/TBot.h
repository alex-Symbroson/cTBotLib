
#ifndef _TBOT_H_
#define _TBOT_H_

#include "request.h"

void TBot_init(char* token);
void TBot_destroy();

char* TBot_send(
	char* func,     //function name (i.e. "sendMessage")
	char* chat_id,  //target where to send message
	char* msg_id,   //needed for editing specific messages
	char* text,     //your message text
	char* reply_id, //needed for replying to specific messages
	char* markup,   //for inline keyboards
	char* full);    //"\0" (false) by default -> extracts the response by skipping the header

#define _TBot_sendMsg(chat_id,text,reply_id,markup,full,...)\
	TBot_send("sendMessage",chat_id,"",text,reply_id,markup,full)
#define TBot_sendMessage(...) _TBot_sendMsg(__VA_ARGS__,"","","","","")

#define _TBot_edtMsg(chat_id,msg_id,text,reply_id,markup,full,...)\
	TBot_send("editMessageText",chat_id,msg_id,text,reply_id,markup,full)
#define TBot_editMessage(...) _TBot_edtMsg(__VA_ARGS__,"","","","","","")

#endif
