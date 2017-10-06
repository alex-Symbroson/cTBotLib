
#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <stdint.h>
#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

char hash8(char *str);

typedef struct {
	char active;
	char* host;
	BIO* bio;
	SSL_CTX* ctx;
} Socket;

	//default value for port = 443
#define __Socket_init(a,b,c,...) _Socket_init(a,b,c)
#define Socket_init(...) __Socket_init(__VA_ARGS__,443)
void _Socket_init(Socket* sock, char* host, uint16_t port);

char* Socket_send(Socket* sock, char* args);
void Socket_close(Socket* sock);

char* extractBody(char* text);

#endif //_REQUEST_H_
