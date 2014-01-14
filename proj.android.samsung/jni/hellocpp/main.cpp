#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include "Core/Browser.h"
#include "Core/Statistics.h"
#include "Logic/Language.h"
#include "Core/Notification.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}
jstring string2jstring(JNIEnv *env, const std::string& str) 
{ 
	return env->NewStringUTF(str.c_str()); 
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

        AppDelegate *pAppDelegate = new AppDelegate();
        CCApplication::sharedApplication()->run();
    }
    else
    {
        ccDrawInit();
        ccGLInvalidateStateCache();
        
        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVNET_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues(); 
    }
}


void Statistics::logEvent(const std::string& name, const Params& p)
{
	JniMethodInfo flurryAgent;
	if (! JniHelper::getStaticMethodInfo(
		flurryAgent, "com/flurry/android/FlurryAgent", 
		"logEvent", "(Ljava/lang/String;Ljava/util/Map;)V"))
	{
		LOGD("Flurry is not supported");
		return;
	}
	LOGD("<Event '%s'", name.c_str());
	
	const Params::SVec& s_keys = p.getKeysString();
    const Params::SVec& s_values = p.getValuesString();

    if(s_keys.size() == s_values.size())
    {
		
		
		JNIEnv* env = flurryAgent.env;
		jclass mapClass = JniHelper::getClassID("java/util/HashMap", env);
		if(mapClass == NULL)
		{
			LOGD("Map Class not found");
			return;
		}
	
		jmethodID init = env->GetMethodID(mapClass, "<init>", "(I)V");
		jobject hashMap = env->NewObject(mapClass, init, s_keys.size());

		jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	
	
    
        for(unsigned int i=0; i<s_keys.size(); ++i)
        {
			jstring key = string2jstring(env, s_keys[i]);
			jstring value = string2jstring(env, s_values[i]);
			env->CallObjectMethod(hashMap, put, key, value);
            //LOGD("-Param '%s'='%s'", .c_str(), s_values[i].c_str());
			
			env->DeleteLocalRef(key);
			env->DeleteLocalRef(value);
        }
		
   
		
		jstring event = string2jstring(env, name);
		flurryAgent.env->CallStaticVoidMethod(flurryAgent.classID, flurryAgent.methodID, event, hashMap);
		
		
		env->DeleteLocalRef(event);
		env->DeleteLocalRef(mapClass);
		LOGD("*Event End>");
    }

   
}

/*void Statistics::statisticEvent(const StatisticsEvent id)
{
	JniMethodInfo methodInfo;
	 if (! JniHelper::getStaticMethodInfo(methodInfo, "com/x4enjoy/Statistics", "statisticEvent", "(I)V"))
	 {
	  return;
	 }

	 methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, id);
	 methodInfo.env->DeleteLocalRef(methodInfo.classID);
}*/




std::string Language::getDeviceLanguage()
{
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, "com/x4enjoy/Language", "getLanguage", "()Ljava/lang/String;"))
	{
		LOGD("Lang error");
		return "en";
	}

	jstring x = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);


    return JniHelper::jstring2string(x);
}

void Notification::notify(const Type t)
{
	JniMethodInfo methodInfo;
	 if (! JniHelper::getStaticMethodInfo(methodInfo, "com/x4enjoy/Notification", "notify", "(I)V"))
	 {
	  return;
	 }

	 methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, t);
	 methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
}
