
#include "request.h"
#include "macros.h"

#include <stdio.h>

void _Socket_init(Socket *s, char* host, uint16_t port) {
	BEGIN("Socket*s,char*host=\"%s\",uint16_t port=%u",host,port);

	s->host = host;
	s->ctx = SSL_CTX_new(SSLv23_client_method());
	if(s->ctx == NULL) error_exit("couldn't create ssl context");

	s->bio = BIO_new_ssl_connect(s->ctx);
	char* buf = malloc((strlen(host)+5));
	sprintf(buf, "%s:%u", host, port);
	BIO_set_conn_hostname(s->bio, buf);
	free(buf);

	if(BIO_do_connect(s->bio) <= 0) error_exit("couldn't connect to host");

	s->active = 1;
	END();
}

char* Socket_send(Socket *s, char* path) {
	BEGIN("Socket*s,char*path=\"%s\"",path);

	char* base = "POST %s HTTP/1.1\r\nHost: %s\r\n\r\n";
	char* msg = malloc((strlen(base) + strlen(path) + strlen(s->host)));
	sprintf(msg, base, path, s->host);
	if(BIO_write(s->bio, msg, strlen(msg)) <= 0) error_exit("couldn't send request");

	int size;
	char buf[1024];
	msg = malloc(0);

	do {
		size = BIO_read(s->bio, buf, 1024);
		buf[size] = 0;
		msg = realloc(msg, strlen(msg) + size);
		sprintf(msg, "%s%s", msg, buf);
	} while(BIO_pending(s->bio) && size);

	END();
	return msg;
}

void Socket_close(Socket* s) {
	BEGIN("Socket*s");

	if(s->active) {
		//BIO_free_all(s->bio);
		SSL_CTX_free(s->ctx);
		s->active = 0;
	}
	END();
}

char* extractBody(char* text) {
	BEGIN("char*text=\"%s\"", text);
	char* ret;
	uint32_t i = 0, len;
	while(text[i])
		if(!strncmp(text + i, "\r\n\r\n", 4)) {
			len = strlen(text + i + 4);
			ret = malloc(len + 1);
			memcpy(ret, text + i + 4, len + 1);
			ret[len] = 0;
			free(text);
			return ret;
		} else i++;
	free(text);
	ret = malloc(1);
	*ret = 0;
	END();
	return ret;
}

/*
	BIO* bio;
	SSL* ssl;
	SSL_CTX* ctx;

	printf("\nfreeing\n");
	BIO_free_all(bio);
	SSL_CTX_free(ctx);
*/
