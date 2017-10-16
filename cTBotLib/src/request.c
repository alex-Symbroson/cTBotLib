
#include "request.h"
#include "macros.h"

#include <string.h>
#include <curl/curl.h>

uint16_t onData(void *data, uint32_t bsize, uint32_t blocks, void* resp) {
	Response* res = (Response*)resp;
	uint16_t len = bsize * blocks;
	res->data = realloc(res->data, res->len + len + 1);
	res->data[res->len + len] = 0;
	memcpy(res->data + res->len, data, len);
	res->len += len;
	return len;
}

char* sendRequest(CURL *curl, struct curl_httppost *post) {
	Response res = { .data = malloc(1), .len = 0 };

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
	if(post != NULL) curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

	CURLcode code = curl_easy_perform(curl);
	if(code) error("curl error %d: %s", code, curl_easy_strerror(code));

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if(code != 200) error("http error %d", code);

	return res.data;
}
