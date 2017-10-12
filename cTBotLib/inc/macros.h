
#ifndef _BS_EXT_HPP_
#define _BS_EXT_HPP_

#include <stdio.h>

#define _DEBUG_ 0    //enables 1:INFO 2:BEGIN and 3:DEBUG macro
#define _ERR_EXIT_ 1 //exit on error

#define error(a,...) printf("\033[1;31m" a "\033[0;37m\n", ##__VA_ARGS__)

	//macros for error mesages
#if defined(_ERR_EXIT_) && _ERR_EXIT_ != 0
	#define error_exit(a,...) (error(a,##__VA_ARGS__), exit(0))
#else
	#define error_exit error
#endif

#define wait_enter() do { stdin = freopen(NULL, "r", stdin); getchar() } while 0;

	//yellow coloured debug macro
#if _DEBUG_ > 0
	#define INFO(s, ...) printf("\033[0;36m    " s "\033[0;37m\n", ##__VA_ARGS__)
#else
	#define INFO(...)
#endif

#if _DEBUG_ > 0
	#define BEGIN(s, ...) printf("  \033[1;33m*\033[2;32m%s line %i %s(" s ")\033[0;37m\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
	#define BEGIN(...)
#endif

#if _DEBUG_ > 0
	#define DEBUG(s, ...) printf("\033[2;33m      " s "\033[0;37m\n", ##__VA_ARGS__)
#else
	#define DEBUG(...)
#endif

#define STATUS_S _DATE_S_ _TIME_S_ _STDC_S_ _STDC_VERSION_S_ _STDC_HOSTED_S_ "\n\n"
#define STATUS_A __DATE__,__TIME__,__STDC__,__STDC_VERSION__,__STDC_HOSTED__
#define STATUS STATUS_S,STATUS_A

#define _DATE_S_ "date:\"%s\" "
#ifndef __DATE__
#define __DATE__ "-"
#endif

#define _TIME_S_ "time:\"%s\" "
#ifndef __TIME__
#define __TIME__ "-"
#endif

#endif
