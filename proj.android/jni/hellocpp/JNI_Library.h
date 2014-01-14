#ifndef JNI_Library_H
#include <jni.h>
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define JNI_Library_H

#define F(Res, Params) "(" Params ")" Res
#define Arr(Code) "[" Code

#define J(Class) "L" Class ";"

#define Void "V"
#define None ""
#define Bool "Z"
#define Float "F"
#define Int "I"
#define String "java/lang/String"
#define Long "J"

template<class T>
inline T GRef(JNIEnv *env, T ref)
{
    T g_ref = (T)env->NewGlobalRef(ref);
    env->DeleteLocalRef(ref);
    return g_ref;
}


inline int getStaticIntFieldValue(JNIEnv *_env, jclass parent, const char* field_name)
{
    jfieldID field_id = _env->GetStaticFieldID(parent, field_name, Int);
    return _env->GetStaticIntField(parent, field_id);
}

inline jobject getStaticObjectFieldValue(JNIEnv *_env, jclass parent, const char* field_name, const char* field_signature)
{
    jfieldID field_id = _env->GetStaticFieldID(parent, field_name, field_signature);
    return _env->GetStaticObjectField(parent, field_id);
}

inline jmethodID getMethod(JNIEnv *_env, jclass parent, const char* method_name, const char* method_signature)
{
    return _env->GetMethodID(parent, method_name, method_signature);
}

inline jmethodID getStaticMethod(JNIEnv *_env, jclass parent, const char* method_name, const char* method_signature)
{
    return _env->GetStaticMethodID(parent, method_name, method_signature);
}

inline jclass getClass(JNIEnv *_env, const char* class_name)
{
    return cocos2d::JniHelper::getClassID(class_name, _env);
}

inline JNIEnv * getEnv()
{
    cocos2d::JniMethodInfo getActivity;
    if(!cocos2d::JniHelper::getStaticMethodInfo(
                getActivity, "java/lang/String",
                "valueOf", F(J("java/lang/String"), Int)))
    {
        return 0;
    }
    return getActivity.env;
}

inline jstring string2jstring(JNIEnv *env, const std::string& str)
{
    return env->NewStringUTF(str.c_str());
}

#endif
