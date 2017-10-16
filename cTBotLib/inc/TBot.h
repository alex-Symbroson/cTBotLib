
#ifndef _TBOT_H_
#define _TBOT_H_

#include "request.h"
#include "json.h"

#include <unistd.h>

#define TBOT_NONE ""
#define TBOT_FALSE "false"
#define TBOT_TRUE "true"

void TBot_init(char* token);
void TBot_destroy();

void writeFile(char* file, char* text);
char* readFile(char* file);

#define delay_mys(t) usleep(t)
#define delay_ms(t) usleep(1000*t)
#define delay_s(t) usleep(1000000*t)

#define TBot_random(max) (srand(clock()), rand()%max)

uint8_t _TBot_checkOk(JSON v, char* print);

char* _TBot_getUpdates(char* offset);
#define TBot_getUpdates(...) _TBot_getUpdates(0##__VA_ARGS__?__VA_ARGS__:"")

	//TBot_setOnUpdate(void(*)(JSON), uint32_t interval=1000)
void _TBot_setOnUpdate(void(*callback)(JSON), uint32_t interval);
#define __TBot_setOnUpdate(onUpdate, interval,...) _TBot_setOnUpdate(onUpdate, interval)
#define TBot_setOnUpdate(onUpdate, ...) __TBot_setOnUpdate(&onUpdate, __VA_ARGS__,1000)

	//the types given in the comments below describes how the argument string should look like
	//'str' can be everything, int just numbers, 'bool' can be 'true' or 'false' and obj is '{}'
char* TBot_send(
	char* func,     //str:  function name (i.e. "sendMessage")
	char* chat_id,  //int:  target where to send message
	char* msg_id,   //int:  needed for editing/reply to specific messages
	char* text,     //str:  your message text
	char* file,     //str:  path to file which should be sent
	char* nonotify, //bool: force notification without sound
	char* markup,   //obj:  additional interface options like inline buttons
	char* special); //obj:  json string for additional function-related arguments

#define TBot_sendCustom(func, chat_id, args) TBot_send(func, chat_id, "", "", "", "", "", "", args)

	//TBot_checkOk(JSON v, char* print="")
#define __TBot_checkOk(v, print, ...) _TBot_checkOk(v, print)
#define TBot_checkOk(...) __TBot_checkOk(__VA_ARGS__, "")

#define getMe() TBot_sendCustom("getMe", "", "")

	//TBot_sendMessage(chat_id, text, nonotify="", reply_id="", markup="", special="")
#define __TBot_sendMessage(chat_id, text, nonotify, reply_id, markup, special, ...)\
	TBot_send("sendMessage", chat_id, reply_id, text, "", nonotify, markup, special)
#define TBot_sendMessage(...) __TBot_sendMessage(__VA_ARGS__, "", "", "", "")

	//! REQUIRES: 'from_chat_id'
#define TBot_forwardMessage(chat_id, msg_id, nonotify, special)\
	TBot_send("forwardMessage", chat_id, msg_id, "", "", "", nonotify, "", special)

	//TBot_sendPhoto(chat_id, photo, caption="", nonotify="", reply_id="", markup="", special="")
#define __TBot_sendPhoto(chat_id, photo, caption, nonotify, reply_id, markup, special, ...)\
	TBot_send("sendPhoto", chat_id, reply_id, caption, photo, nonotify, markup, special)
#define TBot_sendPhoto(...) __TBot_sendPhoto(__VA_ARGS__, "", "", "", "", "")

	//TBot_sendAudio(chat_id, audio, caption="", nonotify="", reply_id="", markup="", special="")
#define __TBot_sendAudio(chat_id, audio, caption, nonotify, reply_id, markup, special, ...)\
	TBot_send("sendAudio", chat_id, reply_id, caption, audio, nonotify, markup, special)
#define TBot_sendAudio(...) __TBot_sendAudio(__VA_ARGS__, "", "", "", "", "")

	//TBot_sendDocument(chat_id, document, caption="", nonotify="", reply_id="", markup="", special="")
#define __TBot_sendDocument(chat_id, document, caption, nonotify, reply_id, markup, special, ...)\
	TBot_send("sendDocument", chat_id, reply_id, caption, document, nonotify, markup, special)
#define TBot_sendDocument(...) __TBot_sendDocument(__VA_ARGS__, "", "", "", "", "")

	//TBot_sendVideo(chat_id, video, caption="", nonotify="", reply_id="", markup="", special="")
#define __TBot_sendVideo(chat_id, video, caption, nonotify, reply_id, markup, special, ...)\
	TBot_send("sendVideo", chat_id, reply_id, caption, video, nonotify, markup, special)
#define TBot_sendVideo(...) __TBot_sendVideo(__VA_ARGS__, "", "", "", "", "")

	//TBot_sendVoice(chat_id, voice, caption="", nonotify="", reply_id="", markup="", special="")
#define __TBot_sendVoice(chat_id, voice, caption, nonotify, reply_id, markup, special, ...)\
	TBot_send("sendVoice", chat_id, reply_id, caption, voice, nonotify, markup, special)
#define TBot_sendVoice(...) __TBot_sendVoice(__VA_ARGS__, "", "", "", "", "")

	//TBot_sendVideoNote(chat_id, video_note, caption="", nonotify="", reply_id="", markup="", special="")
#define __TBot_sendVideoNote(chat_id, video_note, caption, nonotify, reply_id, markup, special, ...)\
	TBot_send("sendVideoNote", chat_id, reply_id, caption, video_note, nonotify, markup, special)
#define TBot_sendVideoNote(...) __TBot_sendVideoNote(__VA_ARGS__, "", "", "", "", "")

	//! REQUIRES: 'phone_number', 'first_name'
	//TBot_sendContact(chat_id, nonotify="", reply_id="", markup="", special="")
#define __TBot_sendContact(chat_id, nonotify, reply_id, markup, special, ...)\
	TBot_send("sendContact", chat_id, reply_id, "", "", nonotify, markup, special)
#define TBot_sendContact(...) __TBot_sendContact(__VA_ARGS__, "", "", "", "")

	//! REQUIRES: 'action'
#define TBot_sendChatAction(chat_id, special) TBot_sendCustom("sendChatAction", chat_id, special)

	//! REQUIRES: 'user_id'
#define TBot_getUserProfilePhotos(special) TBot_sendCustom("getUserProfilePhotos", "", special)

	//! REQUIRES: 'file_id'
#define TBot_getFile(chat_id) TBot_sendCustom("getFile", chat_id, "")

#define TBot_getChat(chat_id) TBot_sendCustom("getChat", chat_id, "")

#define TBot_getChatAdministrators(chat_id) TBot_sendCustom("getChatAdministrators", chat_id, "")

#define TBot_getChatMembersCount(chat_id) TBot_sendCustom("getChatMembersCount", chat_id, "")

	//! REQUIRES: 'user_id'
#define TBot_getChatMember(chat_id, special) TBot_sendCustom("getChatMember", chat_id, special)

	//! REQUIRES: 'callback_query_id'
	//TBot_answerCallbackQuery(special, text="")
#define __TBot_answerCallbackQuery(special, text, ...)\
	TBot_send("getChatMember", "", "", text, "", "", "", "", special)
#define TBot_answerCallbackQuery(...) __TBot_answerCallbackQuery(__VA_ARGS__, "")

//EDITING

	//TBot_editMessageText(chat_id, msg_id, text, markup="", special="")
#define __TBot_editMessageText(chat_id, msg_id, text, markup, special, ...)\
	TBot_send("editMessageText", chat_id, msg_id, text, "", "", markup, special)
#define TBot_editMessageText(...) __TBot_editMessageText(__VA_ARGS__, "", "")

	//TBot_editMessageCaption(chat_id, msg_id, markup="", special="")
#define __TBot_editMessageCaption(chat_id, msg_id, markup, special, ...)\
	TBot_send("editMessageCaption", chat_id, msg_id, text, "", "", markup, special)
#define TBot_editMessageCaption(...) __TBot_editMessageCaption(__VA_ARGS__, "", "")

	//TBot_editMessageReplyMarkup(chat_id, msg_id, text, markup="", special="")
#define __TBot_editMessageReplyMarkup(chat_id, msg_id, text, markup, special, ...)\
	TBot_send("editMessageReplyMarkup", chat_id, msg_id, text, "", "", markup, special)
#define TBot_editMessageReplyMarkup(...) __TBot_editMessageReplyMarkup(__VA_ARGS__, "", "")

#define deleteMessage(chat_id, msg_id) TBot_sendCustom("sendSticker", chat_id, msg_id)

//STICKER

	//TBot_sendSticker(chat_id, msg_id, text, markup="", special="")
#define __TBot_sendSticker(chat_id, msg_id, text, markup, special, ...)\
	TBot_send("sendSticker", chat_id, msg_id, text, "", "", markup, special)
#define TBot_sendSticker(...) __TBot_sendSticker(__VA_ARGS__, "", "")

#define TBot_getStickerName(name) TBot_sendCustom("getStickerName", name, "custom")

#define TBot_uploadStickerFile(user_id, png_sticker)\
	TBot_send("uploadStickerFile", user_id, png_sticker, "", "", "", "", "custom")

#define __TBot_createNewStickerSet(user_id, name, title, png_sticker, emojis, special)\
	TBot_send("createNewStickerSet", user_id, name, title, png_sticker, emojis, special, "custom")
#define TBot_createNewStickerSet(...) __TBot_createNewStickerSet(__VA_ARGS__, "")

	//TBot_addStickerToSet(user_id, name, png_sticker, emojis, mask_position="")
#define __TBot_addStickerToSet(user_id, name, png_sticker, emojis, mask_position, ...)\
	TBot_send("addStickerToSet", user_id, name, png_sticker, emojis, "", "", "custom")
#define TBot_addStickerToSet(...) __TBot_addStickerToSet(__VA_ARGS__, "")

#define TBot_setStickerPositionInSet(sticker, position)\
	TBot_send("setStickerPositionInSet", sticker, position, "", "", "", "", "custom")

#define TBot_deleteStickerFromSet(sticker) TBot_sendCustom("deleteStickerFromSet", sticker, "custom")

//INLINE QUERY
	//TBot_answerInlineQuery(inline_query_id, results, special="")
#define __TBot_answerInlineQuery(inline_query_id, results, special)\
	TBot_send("answerInlineQuery", inline_query_id, results, "", "", "", special, "custom")
#define TBot_answerInlineQuery(...) __TBot_answerInlineQuery(__VA_ARGS__, "")

#endif
