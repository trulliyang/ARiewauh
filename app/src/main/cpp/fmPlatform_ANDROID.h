#include <android/log.h>
#include <jni.h>
#include <cstdlib>
#include <android/log.h>
#include <android/bitmap.h>




#ifndef  FM_LOG_TAG
#define  FM_LOG_TAG "FM"
#endif

//#if defined(DEBUG) || defined(_DEBUG) || defined(_FM_LOGS_)
#define  FM_LOG_INFO(...)  __android_log_print(ANDROID_LOG_INFO, FM_LOG_TAG, __VA_ARGS__)
#define  FM_LOG_ERROR(...)  __android_log_print(ANDROID_LOG_ERROR, FM_LOG_TAG, __VA_ARGS__)
//#else
//#define  FM_LOG_INFO(...)
//#define  FM_LOG_ERROR(...)
//#endif

#define FM_SPINE_DEFAULT_WIDTH 720
#define FM_SPINE_DEFAULT_HEIGHT 1280

#if defined(DEBUG) || defined(_DEBUG) || defined(_FM_LOGS_)
#define FM_LOG_CODE(...) __VA_ARGS__
#else
#define FM_LOG_CODE(...)
#endif

#ifndef FM_SHADER_STRING
#define FM_SHADER_STRING(...) #__VA_ARGS__
#endif

extern "C"{


typedef const char*  FMConstString;



}

