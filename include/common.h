#ifndef __CHEETAH_COMMON
#define __CHEETAH_COMMON

#ifdef _WIN32
    #include <Windows.h>

    #ifdef _WINDLL
        #define CHEETAH_API __declspec(dllexport)
    #else
        #define CHEETAH_API
    #endif
#else
    #include <pthread.h>
    #define CHEETAH_API
#endif

#endif
