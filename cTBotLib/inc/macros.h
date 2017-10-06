
#ifndef _BS_EXT_HPP_
#define _BS_EXT_HPP_

	#include <stdio.h>

	#define _DEBUG_ 0    //enables INFO DEBUG and BEGIN macro
	#define _ERR_EXIT_ 1 //exit on error

		//macros for error mesages
	#if defined(_ERR_EXIT_) && _ERR_EXIT_ != 0
		#define error(a,...) (printf("\033[1;31m" a "\033[0;37m\n", ##__VA_ARGS__), exit(0))
	#else
		#define error(a,...) printf("\033[1;31m" a "\033[0;37m\n", ##__VA_ARGS__)
	#endif

	#define wait_enter() stdin = freopen(NULL, "r", stdin); getchar()

		//yellow coloured debug macro
	#if defined(_DEBUG_) && _DEBUG_ != 0
		#define INFO(s, ...) printf("\033[0;36m    " s "\033[0;37m\n", ##__VA_ARGS__)
		#define DEBUG(s, ...) printf("\033[2;33m      " s "\033[0;37m\n", ##__VA_ARGS__)
		#define BEGIN(s, ...) printf("\033[2;32m  %s line %i %s(" s ")\033[0;37m\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
	#else
		#define BEGIN(...)
		#define DEBUG(...)
		#define INFO(...)
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

#define _STDC_S_ "stdc:%i "
#ifndef __STDC__
#define __STDC__ 0
#endif

#define _STDC_VERSION_S_ "stdc_ver:%i "
#ifndef __STDC_VERSION__
#define __STDC_VERSION__ 0
#endif

#define _STDC_HOSTED_S_ "stdc_hosted:%i "
#ifndef __STDC_HOSTED__
#define __STDC_HOSTED__ 0
#endif

#endif
