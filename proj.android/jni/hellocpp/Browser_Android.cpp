#include "Core/Browser.h"
#include "JNI_Library.h"
using namespace cocos2d;
#define Context "android/content/Context"
#define Intent "android/content/Intent"
#define MainActivity "com/x4enjoy/MainActivity"
#define Uri "android/net/Uri"

class BrowserHelper
{
public:
    void openWebURL(const std::string& url);
    void openMarketItem(const std::string& item_id);
    void sendMail(const std::string& email, const std::string &subject="");
    static BrowserHelper& getInstance();
private:
    BrowserHelper();
    void startActivity(jobject intent);

    int _Intent_FLAG_ACTIVITY_NEW_TASK;
    jobject _Intent_EXTRA_EMAIL;
    jobject _Intent_EXTRA_SUBJECT;
    jobject _Intent_EXTRA_TEXT;
    jobject _Intent_ACTION_VIEW;
    jobject _Intent_ACTION_SEND;
    jclass _Intent;

    jmethodID _Intent_addFlags;
    jmethodID _Intent_putExtra_String;
    jmethodID _Intent_putExtra_StringArr;
    jmethodID _Intent_setType;
    jmethodID _Intent_construct2;
    jmethodID _Intent_construct;

    jobject _main_context;
    jmethodID _Context_startActivity;

    jclass _Uri;
    jmethodID _Uri_parse;

    JNIEnv* _env;
};

void BrowserHelper::openWebURL(const std::string& url)
{
    //Intent browserIntent = new Intent(Intent.ACTION_VIEW,
    //                    Uri.parse("http://4enjoy.com/r/android/"+id+'/'));
    jobject intent = _env->NewObject(
                _Intent, _Intent_construct2, _Intent_ACTION_VIEW,
                _env->CallStaticObjectMethod(_Uri, _Uri_parse, string2jstring(_env, url)));

    //browserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    _env->CallObjectMethod(intent, _Intent_addFlags, _Intent_FLAG_ACTIVITY_NEW_TASK);

    startActivity(intent);
}

void BrowserHelper::openMarketItem(const std::string& item_id)
{
    openWebURL(item_id);
}

void BrowserHelper::sendMail(const std::string& email, const std::string &subject)
{
//    Intent i = new Intent(Intent.ACTION_SEND);
    jobject intent = _env->NewObject(
                _Intent, _Intent_construct, _Intent_ACTION_SEND);

//    i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    _env->CallObjectMethod(intent, _Intent_addFlags, _Intent_FLAG_ACTIVITY_NEW_TASK);

//    i.setType("message/rfc822");
    _env->CallObjectMethod(intent, _Intent_setType, string2jstring(_env, "message/rfc822"));

//    i.putExtra(Intent.EXTRA_EMAIL  , new String[]{mailTo});
    _env->CallObjectMethod(intent, _Intent_putExtra_StringArr,
                           _Intent_EXTRA_EMAIL,
                           _env->NewObjectArray(1, getClass(_env, String),
                                                string2jstring(_env, email)));


//    i.putExtra(Intent.EXTRA_SUBJECT, "Math Is Fun Android");
    _env->CallObjectMethod(intent, _Intent_putExtra_String,
                           _Intent_EXTRA_SUBJECT, string2jstring(_env, subject));
//    i.putExtra(Intent.EXTRA_TEXT   , "");
    _env->CallObjectMethod(intent, _Intent_putExtra_String,
                           _Intent_EXTRA_TEXT, string2jstring(_env, ""));
//    try {
//        App.getAppContext().startActivity(i);
//        //App.getAppContext().startActivity(Intent.createChooser(i, "Send mail..."));
//    } catch (android.content.ActivityNotFoundException ex) {
//        Log.d("Browser", "ERRROR");
//    }
    startActivity(intent);
}
void BrowserHelper::startActivity(jobject intent)
{
    //App.getAppContext().startActivity(browserIntent);
    _env->CallVoidMethod(_main_context, _Context_startActivity, intent);

    if(_env->ExceptionCheck())
    {
        _env->ExceptionDescribe();
        _env->ExceptionClear();
    }
}

BrowserHelper& BrowserHelper::getInstance()
{
    static BrowserHelper helper;
    return helper;
}

BrowserHelper::BrowserHelper()
{

    _env = getEnv();
    jclass MainActivity_class = getClass(_env, MainActivity);
    jmethodID MainActivity_getContext = getStaticMethod(
                _env, MainActivity_class, "getContext", F(J(Context), None));

    _main_context = GRef(_env, _env->CallStaticObjectMethod(
                             MainActivity_class, MainActivity_getContext));
    _Context_startActivity = getMethod(_env, getClass(_env, Context), "startActivity", F(Void, J(Intent)));

    _Intent = GRef(_env, getClass(_env, Intent));

    _Intent_construct2 = getMethod(_env, _Intent, "<init>", F(Void, J(String) J(Uri)));
    _Intent_construct = getMethod(_env, _Intent, "<init>", F(Void, J(String)));
    _Intent_addFlags = getMethod(_env, _Intent, "addFlags", F(J(Intent), Int));
    _Intent_putExtra_String = getMethod(
                _env, _Intent, "putExtra", F(J(Intent), J(String) J(String)));
    _Intent_putExtra_StringArr = getMethod(
                _env, _Intent, "putExtra", F(J(Intent), J(String) Arr(J(String))));
    _Intent_setType = getMethod(_env, _Intent, "setType", F(J(Intent), J(String)));


    _Intent_FLAG_ACTIVITY_NEW_TASK = getStaticIntFieldValue(
                _env, _Intent, "FLAG_ACTIVITY_NEW_TASK");

    _Intent_EXTRA_EMAIL = GRef(_env, getStaticObjectFieldValue(
                _env, _Intent, "EXTRA_EMAIL", J(String)));

    _Intent_ACTION_SEND = GRef(_env, getStaticObjectFieldValue(
                                  _env, _Intent, "ACTION_SEND", J(String)));


    _Intent_EXTRA_SUBJECT =GRef(_env, getStaticObjectFieldValue(
                _env, _Intent, "EXTRA_SUBJECT", J(String)));

    _Intent_EXTRA_TEXT = GRef(_env, getStaticObjectFieldValue(
                _env, _Intent, "EXTRA_TEXT", J(String)));

    _Intent_ACTION_VIEW = GRef(_env, getStaticObjectFieldValue(
                _env, _Intent, "ACTION_VIEW", J(String)));

    _Uri = GRef(_env, getClass(_env, Uri));
    _Uri_parse = getStaticMethod(_env, _Uri, "parse", F(J(Uri), J(String)));
}

void Browser::openWebURL(const std::string& url)
{
    BrowserHelper::getInstance().openWebURL(url);
}

void Browser::openMarketItem(const std::string& item_id)
{
    BrowserHelper::getInstance().openMarketItem(item_id);
}

void Browser::sendMail(const std::string& email, const std::string &subject)
{
    BrowserHelper::getInstance().sendMail(email, subject);
}


