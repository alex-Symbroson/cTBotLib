
#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <stdint.h>
#include <curl/curl.h>

typedef struct {
	char* data;
	uint16_t len;
} Response;

char* sendRequest(CURL *curl, struct curl_httppost *post);

#endif //_REQUEST_H_
