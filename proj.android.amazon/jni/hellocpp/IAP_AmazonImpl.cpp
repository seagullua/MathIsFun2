#include "IAP_API.h"
#include "IAP_Event_API.h"
#include <jni.h>
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"
#include <string>
#define  LOG_TAG    "SamsungIAP"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;



jstring string2jstring(JNIEnv *env, const std::string& str) 
{ 
	return env->NewStringUTF(str.c_str()); 
}

void callStaticVoidMethod(const std::string& class_name, const std::string& method_name)
{
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, class_name.c_str(), method_name.c_str(), "()V"))
	{
		LOGD("Can't call method %s in class %s", method_name.c_str(), class_name.c_str());
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void callStaticVoidMethod(const std::string& class_name, const std::string& method_name, const std::string& param)
{
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, class_name.c_str(), method_name.c_str(), "(Ljava/lang/String;)V"))
	{
		LOGD("Can't call method %s in class %s", method_name.c_str(), class_name.c_str());
		return;
	}
	
	jstring jp = string2jstring(methodInfo.env, param); 
	
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jp);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

static const std::string CLASS = "com/amazon/AmazonStore";

void IAP_storeOpening()
{
	callStaticVoidMethod(CLASS, "storeOpening");
}

void IAP_storeClosing()
{
	callStaticVoidMethod(CLASS, "storeClosing");
}

void IAP_initialize(const std::string&)
{
	callStaticVoidMethod(CLASS, "initialize");
}

void IAP_buyMarketItem(const std::string& item)
{
	callStaticVoidMethod(CLASS, "buyMarketItem", item);
}

void IAP_restoreTransactions()
{
	callStaticVoidMethod(CLASS, "restoreTransactions");
}

	
extern "C" 
{
void Java_com_amazon_AmazonStore_EventbillingSupported(JNIEnv*  env, jobject thiz, jboolean param)
{
	if(param)
		LOGD("Native Event_billingSupported");
	else
		LOGD("Native Event_billingNotSupported");
	IAP_Event_billingSupported(param);
}

void Java_com_amazon_AmazonStore_EventrestoreTransactions(JNIEnv*  env, jobject thiz, jboolean param)
{
	LOGD("Native Event_restoreTransactions");
	IAP_Event_restoreTransactions(param);
}

void Java_com_amazon_AmazonStore_EventmarketPurchase(JNIEnv*  env, jobject thiz, jstring param)
{
	LOGD("Native Event_marketPurchase");
	IAP_Event_marketPurchase(JniHelper::jstring2string(param));
}

void Java_com_amazon_AmazonStore_EventmarketPurchaseCancelled(JNIEnv*  env, jobject thiz, jstring param)
{
	LOGD("Native Event_marketPurchaseCancelled");
	IAP_Event_marketPurchaseCancelled(JniHelper::jstring2string(param));
}
}