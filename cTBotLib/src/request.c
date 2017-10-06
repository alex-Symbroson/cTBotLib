
#include "request.h"
#include "macros.h"

#include <stdio.h>

void _Socket_init(Socket *s, char* host, uint16_t port) {
	BEGIN("Socket*s,char*host=\"%s\",uint16_t port=%i",host,port);

	s->host = host;
	s->ctx = SSL_CTX_new(SSLv23_client_method());
	if(s->ctx == NULL) error("couldn't create ssl context");

	s->bio = BIO_new_ssl_connect(s->ctx);
	char* buf = malloc((strlen(host)+5));
	sprintf(buf, "%s:%i", host, port);
	BIO_set_conn_hostname(s->bio, buf);
	free(buf);

	if(BIO_do_connect(s->bio) <= 0) error("couldn't connect to host");

	s->active = 1;
}

char* Socket_send(Socket *s, char* args) {
	BEGIN("Socket*s,char*args=%s",args);

	char* base = "POST %s HTTP/1.1\r\nHost: %s\r\n\r\n";
	char* msg = malloc((strlen(base)+strlen(args)+strlen(s->host)));
	sprintf(msg, base, args, s->host);

	if(BIO_write(s->bio, msg, strlen(msg)) <= 0) error("couldn't send request");

	int size;
	char buf[1024];
	msg = malloc(0);

	do {
		size = BIO_read(s->bio, buf, 1024);
		buf[size] = 0;
		msg = realloc(msg, size);
		sprintf(msg, "%s", buf);
	} while(BIO_pending(s->bio) && size);

	return msg;
}

void Socket_close(Socket* s) {
	BEGIN("Socket*s");

	if(s->active) {
		//BIO_free_all(s->bio);
		SSL_CTX_free(s->ctx);
		s->active = 0;
	}
}

char* extractBody(char* text) {
	BEGIN("char*text");
	while(*text)
		if(!strncmp(text++, "\r\n\r\n", 4))
			return text+3;
	return "\0";
}

/*
	BIO* bio;
	SSL* ssl;
	SSL_CTX* ctx;

	printf("\nfreeing\n");
	BIO_free_all(bio);
	SSL_CTX_free(ctx);
*/
