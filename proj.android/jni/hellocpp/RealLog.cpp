#include <android/log.h>
#include <stdio.h>
#include <jni.h>

void realLog(const char * pszFormat, ...)
{
	const unsigned int MAX_LENGTH = 500;
    char buf[MAX_LENGTH];

    va_list args;
    va_start(args, pszFormat);        
    vsnprintf(buf, MAX_LENGTH, pszFormat, args);
    va_end(args);

    __android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info",  buf);
}