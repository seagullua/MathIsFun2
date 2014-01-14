#include "Core/Ads.h"

#include <map>

#include "JNI_Library.h"
#include "UIThread.h"

#define AdSize "com/google/ads/AdSize"
#define AdView "com/google/ads/AdView"
#define String "java/lang/String"
#define Activity "android/app/Activity"
#define FrameLayout "android/widget/FrameLayout"
#define View "android/view/View"
#define AdRequest "com/google/ads/AdRequest"
#define LayoutParams "android/view/ViewGroup$LayoutParams"
#define RID "android/R$id"
#define Resources "android/content/res/Resources"
#define DisplayMetrics "android/util/DisplayMetrics"
#define TranslateAnimation "android/view/animation/TranslateAnimation"
#define Build_Version "android/os/Build$VERSION"
#define Animation "android/view/animation/Animation"

#define EAT_EXEPTIONS if(_env->ExceptionOccurred()) \
			_env->ExceptionClear();
namespace ads
{


class AdMobHelper
{
public:
    static AdMobHelper& getInstance();
    void createNativeBanner(const BannerID id, const BannerSize& size, const CCPoint& position);
    void destroyBanner(const BannerID id);
    void hideBanner(const BannerID id);
    void showBanner(const BannerID id);

    bool isBannerShown(const BannerID id);
    BannerID getNextID();
    float getDensity();
private:
    AdMobHelper();
    friend class InitAdMob;
    void init();
    static AdMobHelper* _instance;

    typedef std::map<BannerID, jobject> ObjectMap;
    ObjectMap _banners_map;

    float _density;
    jclass _MainActivity;
    jmethodID _MainActivity_getActivity;

    jobject _activity;

    void translateView(jobject view, float x, float y);

    /*jclass _HashMap;
    jmethodID _HashMap_put;
    jmethodID _HashMap_get;
    jmethodID _HashMap_containsKey;*/

    jclass _AdView;
    jmethodID _AdView_Construct;
    jmethodID _AdView_isReady;
    jmethodID _AdView_destroy;
    jmethodID _AdView_stopLoading;
    jmethodID _AdView_loadAd;
    jmethodID _AdView_setLayoutParams;
    jmethodID _AdView_setTranslationX;
    jmethodID _AdView_setTranslationY;
    jmethodID _AdView_setVisibility;
    jmethodID _AdView_startAnimation;

    int _View_GONE;
    int _View_VISIBLE;
    bool _no_native;


    jclass _TranslateAnimation;
    jmethodID _TranslateAnimation_construct;
    jmethodID _TranslateAnimation_setFillAfter;
    jmethodID _TranslateAnimation_setDuration;

    jobject _AdSize_BANNER; //320x50
    jobject _AdSize_IAB_BANNER; //468x60

    jclass _AdRequest;
    jmethodID _AdRequest_Construct;
    jmethodID _AdRequest_addTestDevice;

    jclass _FrameLayout;
    jobject _main_layout;
    jmethodID _FrameLayout_addView;
    jmethodID _FrameLayout_removeView;

    jobject _layout_params;

    jstring _banner_zone_key;

    JNIEnv* _env;
    BannerID _next_id;

    int _SDK_Version;
    static const int _Translation_SDKMin = 11;

    std::vector<jstring> _test_devices;
};
float AdMobHelper::getDensity()
{
    if(_density == 0)
    {
        JniMethodInfo getActivity;
        if(!JniHelper::getStaticMethodInfo(
                    getActivity, "com/x4enjoy/MainActivity",
                    "getActivity", "()Landroid/app/Activity;"))
        {

        }
        JNIEnv* env = getActivity.env;
        jclass MainActivity = getActivity.classID;
        jmethodID MainActivity_getActivity = getActivity.methodID;

        jobject activity = env->CallStaticObjectMethod(MainActivity, MainActivity_getActivity);
        jclass activity_class = JniHelper::getClassID(Activity, env);

        //Get density
        jclass Resources_class = JniHelper::getClassID(Resources, env);
        jmethodID getResources = env->GetMethodID(activity_class, "getResources", F(J(Resources), None));
        jobject resources_obj = env->CallObjectMethod(activity, getResources);
        jclass DisplayMetrics_class = JniHelper::getClassID(DisplayMetrics, env);
        jmethodID getDisplayMetrics = env->GetMethodID(Resources_class, "getDisplayMetrics", F(J(DisplayMetrics), None));
        jobject metrics_obj = env->CallObjectMethod(resources_obj, getDisplayMetrics);


        jfieldID density_field = env->GetFieldID(DisplayMetrics_class, "density", Float);
        _density = env->GetFloatField(metrics_obj, density_field);
        env->DeleteLocalRef(Resources_class);
        env->DeleteLocalRef(DisplayMetrics_class);
    }
    return _density;
}

class CreateBannerTask : public Runnable
{
public:
    CreateBannerTask(const BannerID id, const BannerSize& size, const CCPoint& position)
        : _id(id), _size(size), _position(position)
    {}

    void run()
    {
        AdMobHelper::getInstance().createNativeBanner(_id, _size, _position);
    }

private:
    BannerID _id;
    BannerSize _size;
    CCPoint _position;
};
class IsBannerShownResult : public Runnable
{
public:
    IsBannerShownResult(const BannerID id, bool res)
        : _id(id), _res(res)
    {}

    void run()
    {
        AdsManager::getInstance().bannerShownResult(_res, _id);
    }

private:
    BannerID _id;
    bool _res;
};
class IsBannerShownTask : public Runnable
{
public:
    IsBannerShownTask(const BannerID id)
        : _id(id)
    {}

    void run()
    {
        bool res = AdMobHelper::getInstance().isBannerShown(_id);
        UIThread::getInstance().runInCocos2dThread(
                    RunnablePtr(new IsBannerShownResult(_id, res)));
    }

private:
    BannerID _id;
};

class ShowBannerTask : public Runnable
{
public:
    ShowBannerTask(const BannerID id)
        : _id(id)
    {}

    void run()
    {
        AdMobHelper::getInstance().showBanner(_id);
    }

private:
    BannerID _id;
};

class HideBannerTask : public Runnable
{
public:
    HideBannerTask(const BannerID id)
        : _id(id)
    {}

    void run()
    {
        AdMobHelper::getInstance().hideBanner(_id);
    }

private:
    BannerID _id;
};


class DestroyBannerTask : public Runnable
{
public:
    DestroyBannerTask(const BannerID id)
        : _id(id)
    {}

    void run()
    {
        AdMobHelper::getInstance().destroyBanner(_id);
    }

private:
    BannerID _id;
};

class InitAdMob : public Runnable
{
public:
    InitAdMob(AdMobHelper* pointer)
        : _pointer(pointer)
    {}

    void run()
    {
        _pointer->init();
    }
private:
    AdMobHelper* _pointer;

};

BannerID AdsManager::assignNewBannerId()
{
    AdMobHelper& helper = AdMobHelper::getInstance();
    BannerID id = helper.getNextID();
    return id;
}

void AdsManager::createNativeBanner(const BannerID id, const BannerSize& size, const CCPoint& position)
{

    UIThread::getInstance().runInUIThread(
                RunnablePtr(new CreateBannerTask(id, size, position)));
}

void AdsManager::showNativeBanner(const BannerID id)
{
    UIThread::getInstance().runInUIThread(
                RunnablePtr(new ShowBannerTask(id)));
}

void AdsManager::hideNativeBanner(const BannerID id)
{
    UIThread::getInstance().runInUIThread(
                RunnablePtr(new HideBannerTask(id)));
}

void AdsManager::nativeInit()
{
    AdMobHelper::getInstance().getNextID();
}
float AdsManager::getDeviceDensity()
{
    return AdMobHelper::getInstance().getDensity();
}

void AdsManager::destroyNativeBanner(const BannerID id)
{
    UIThread::getInstance().runInUIThread(
                RunnablePtr(new DestroyBannerTask(id)));
}

void AdsManager::isNativeBannerShown(const BannerID id)
{
    UIThread::getInstance().runInUIThread(
                RunnablePtr(new IsBannerShownTask(id)));
}

AdMobHelper* AdMobHelper::_instance = 0;

AdMobHelper& AdMobHelper::getInstance()
{
    if(_instance == 0)
    {
        _instance = new AdMobHelper;
    }
    return *_instance;
}

BannerID AdMobHelper::getNextID()
{
    return _next_id++;
}

AdMobHelper::AdMobHelper()
    : _next_id(1), _density(0), _SDK_Version(0), _no_native(false)
{
    UIThread::getInstance().runInUIThread(
                RunnablePtr(new InitAdMob(this)));
}

void AdMobHelper::init()
{
    JniMethodInfo getActivity;
    if(!JniHelper::getStaticMethodInfo(
                getActivity, "com/x4enjoy/MainActivity",
                "getActivity", "()Landroid/app/Activity;"))
    {

    }
    _env = getActivity.env;
    _MainActivity = getActivity.classID;
    _MainActivity_getActivity = getActivity.methodID;

    _activity = _env->NewGlobalRef(_env->CallStaticObjectMethod(_MainActivity, _MainActivity_getActivity));


    _SDK_Version = getStaticIntFieldValue(_env, getClass(_env, Build_Version), "SDK_INT");
    /*_HashMap = JniHelper::getClassID("java/util/HashMap", env);

jmethodID HashMap_Construct = env->GetMethodID(_HashMap, "<init>", "()V");
_banners_map = _env->NewObject(_HashMap, HashMap_Construct);

_HashMap_put = _env->GetMethodID(_HashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
_HashMap_get = _env->GetMethodID(_HashMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
_HashMap_containsKey = _env->GetMethodID(_HashMap, "containsKey", "(Ljava/lang/Object;)Z");*/

    jclass adSize = JniHelper::getClassID(AdSize, _env);
    jclass _AdView_local = JniHelper::getClassID(AdView, _env);
    _AdView = (jclass)_env->NewGlobalRef(_AdView_local);
    _env->DeleteLocalRef(_AdView_local);

    jclass _AdRequest_local = JniHelper::getClassID(AdRequest, _env);
    _AdRequest = (jclass)_env->NewGlobalRef(_AdRequest_local);
    _env->DeleteLocalRef(_AdRequest_local);

    _FrameLayout = JniHelper::getClassID(FrameLayout, _env);

    jfieldID adSize_BANNER = _env->GetStaticFieldID(adSize, "BANNER", J(AdSize)); //320x50
    jfieldID adSize_IAB_BANNER = _env->GetStaticFieldID(adSize, "IAB_BANNER", J(AdSize)); //468x60

    _AdSize_BANNER = _env->NewGlobalRef(_env->GetStaticObjectField(adSize, adSize_BANNER));
    _AdSize_IAB_BANNER = _env->NewGlobalRef(_env->GetStaticObjectField(adSize, adSize_IAB_BANNER));


    _AdView_Construct = _env->GetMethodID(_AdView, "<init>", F(Void , J(Activity) J(AdSize) J(String)) );
    _AdView_isReady = _env->GetMethodID(_AdView, "isReady", F(Bool, None));
    _AdView_destroy = _env->GetMethodID(_AdView, "destroy", F(Void, None));
    _AdView_stopLoading = _env->GetMethodID(_AdView, "stopLoading", F(Void, None));
    _AdView_loadAd = _env->GetMethodID(_AdView, "loadAd", F(Void, J(AdRequest)));
    _AdView_setLayoutParams = _env->GetMethodID(_AdView, "setLayoutParams", F(Void, J(LayoutParams)));

    if(_SDK_Version >= _Translation_SDKMin)
    {
        _AdView_setTranslationX = _env->GetMethodID(_AdView, "setTranslationX", F(Void, Float));
        _AdView_setTranslationY = _env->GetMethodID(_AdView, "setTranslationY", F(Void, Float));
    }
    else
    {
        _no_native = true;
    }
    _AdView_setVisibility = _env->GetMethodID(_AdView, "setVisibility", F(Void, Int));

    jclass View_class = JniHelper::getClassID(View, _env);
    jfieldID adView_GONE_field = _env->GetStaticFieldID(View_class, "GONE", Int);
    jfieldID adView_VISIBLE_field = _env->GetStaticFieldID(View_class, "VISIBLE", Int);

    _View_GONE =  _env->GetStaticIntField(View_class, adView_GONE_field);
    _View_VISIBLE =  _env->GetStaticIntField(View_class, adView_VISIBLE_field);

    _AdRequest_Construct = _env->GetMethodID(_AdRequest, "<init>", F(Void, None));
    _AdRequest_addTestDevice = _env->GetMethodID(_AdRequest, "addTestDevice", F(J(AdRequest), J(String)));


    jclass R_id_class = JniHelper::getClassID(RID, _env);
    jfieldID R_id_content = _env->GetStaticFieldID(R_id_class, "content", Int);
    jint R_id_content_value = _env->GetStaticIntField(R_id_class, R_id_content);



    jclass activity_class = JniHelper::getClassID(Activity, _env);
    jmethodID findViewById = _env->GetMethodID(activity_class, "findViewById", F(J(View), Int));
    jobject _main_layout_local = _env->CallObjectMethod(_activity, findViewById, R_id_content_value);
    _main_layout = _env->NewGlobalRef(_main_layout_local);


    _FrameLayout_addView = _env->GetMethodID(_FrameLayout, "addView", F(Void, J(View) Int Int));
    _FrameLayout_removeView = _env->GetMethodID(_FrameLayout, "removeView", F(Void, J(View)));

    jclass layout_params_class = JniHelper::getClassID(LayoutParams, _env);
    jmethodID layout_params_constructor = _env->GetMethodID(layout_params_class, "<init>", F(Void, Int Int));
    jfieldID layout_params_FILL_PARENT = _env->GetStaticFieldID(layout_params_class, "FILL_PARENT", Int);
    jfieldID layout_params_WRAP_CONTENT = _env->GetStaticFieldID(layout_params_class, "WRAP_CONTENT", Int);

    jint FILL_PARENT = _env->GetStaticIntField(layout_params_class, layout_params_FILL_PARENT);
    jint WRAP_CONTENT = _env->GetStaticIntField(layout_params_class, layout_params_WRAP_CONTENT);


    _banner_zone_key = (jstring)_env->NewGlobalRef(string2jstring(_env, AdsManager::getInstance().getBannerZoneKey()));


    const AdsManager::StringVector& test_devices = AdsManager::getInstance().getTestDevices();
    _test_devices.reserve(test_devices.size());
    for(unsigned int i=0; i<test_devices.size(); ++i)
    {
        jstring local_str = string2jstring(_env, test_devices[i]);
        _test_devices.push_back((jstring)_env->NewGlobalRef(local_str));
        _env->DeleteLocalRef(local_str);
    }


    //ViewGroup.LayoutParams ad_layout_params = new ViewGroup.LayoutParams( ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    jobject _layout_params_local = _env->NewObject(layout_params_class, layout_params_constructor, FILL_PARENT, WRAP_CONTENT);
    _layout_params = _env->NewGlobalRef(_layout_params_local);


    _env->DeleteLocalRef(adSize);
    _env->DeleteLocalRef(R_id_class);
    _env->DeleteLocalRef(layout_params_class);
    _env->DeleteLocalRef(activity_class);




}

bool AdMobHelper::isBannerShown(const BannerID id)
{
    if(_no_native)
        return false;

    ObjectMap::iterator it = _banners_map.find(id);
    if(it != _banners_map.end())
    {
        jobject adView = it->second;
		bool result = false;
		
		
        result = _env->CallBooleanMethod(adView, _AdView_isReady);
		if(_env->ExceptionOccurred())
		{
			result = false;
			_env->ExceptionClear();
		}
		
		return result;
    }
    return false;
}

void AdMobHelper::destroyBanner(const BannerID id)
{
    if(_no_native)
        return;

	_env = getEnv();
	if(!_env) return;
		
    ObjectMap::iterator it = _banners_map.find(id);
    if(it != _banners_map.end())
    {
        jobject adView = it->second;
		
		_env->CallVoidMethod(_main_layout, _FrameLayout_removeView, adView);
		EAT_EXEPTIONS
			
		_env->CallVoidMethod(adView, _AdView_stopLoading);
		EAT_EXEPTIONS
			
		_env->CallVoidMethod(adView, _AdView_destroy);
		EAT_EXEPTIONS
			
        _env->DeleteGlobalRef(adView);
		EAT_EXEPTIONS
		
        _banners_map.erase(it);
    }
}
void AdMobHelper::hideBanner(const BannerID id)
{
    if(_no_native)
        return;

    ObjectMap::iterator it = _banners_map.find(id);
    if(it != _banners_map.end())
    {
		_env = getEnv();
		if(!_env) return;
	
        jobject adView = it->second;
        _env->CallVoidMethod(adView, _AdView_setVisibility, _View_GONE);
		EAT_EXEPTIONS
    }
}

void AdMobHelper::showBanner(const BannerID id)
{
    if(_no_native)
        return;

    ObjectMap::iterator it = _banners_map.find(id);
    if(it != _banners_map.end())
    {
		_env = getEnv();
		if(!_env) return;
		
        jobject adView = it->second;
        _env->CallVoidMethod(adView, _AdView_setVisibility, _View_VISIBLE);
		EAT_EXEPTIONS
    }
}
void AdMobHelper::translateView(jobject view, float x, float y)
{
    if(_SDK_Version >= _Translation_SDKMin)
    {
        //adView.setTranslationX(2);
        _env->CallVoidMethod(view, _AdView_setTranslationX, x);
		EAT_EXEPTIONS

        //adView.setTranslationY(2);
        _env->CallVoidMethod(view, _AdView_setTranslationY, y);
		EAT_EXEPTIONS
    }
}

void AdMobHelper::createNativeBanner(const BannerID id, const BannerSize& size, const CCPoint& position)
{
    if(_no_native)
        return;

	_env = getEnv();
	if(!_env) return;
		
    jobject banner_type = _AdSize_BANNER;
    if(size.getWidth() == 468)
        banner_type = _AdSize_IAB_BANNER;

    //adView = new AdView(this, AdSize.SMART_BANNER, "ca-app-pub-1097233306002326/4129712095");
    jobject adView_local = _env->NewObject(_AdView, _AdView_Construct, _activity, banner_type, _banner_zone_key);
    EAT_EXEPTIONS
	
	jobject adView = _env->NewGlobalRef(adView_local);
	EAT_EXEPTIONS
	
    _env->DeleteLocalRef(adView_local);
	EAT_EXEPTIONS
	
    //ViewGroup.LayoutParams ad_layout_params = new ViewGroup.LayoutParams( ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    //adView.setLayoutParams(ad_layout_params);
    _env->CallVoidMethod(adView, _AdView_setLayoutParams, _layout_params);
	EAT_EXEPTIONS


    translateView(adView, position.x/**_density*/, position.y/**_density*/);
	EAT_EXEPTIONS
	
    //AdRequest adRequest = new AdRequest();
    jobject adRequest = _env->NewObject(_AdRequest, _AdRequest_Construct);
	EAT_EXEPTIONS

    //adRequest.addTestDevice("419CBB113860522A7AB95487DBB0CC2B");
    for(unsigned int i=0; i<_test_devices.size(); ++i)
    {
        _env->CallObjectMethod(adRequest, _AdRequest_addTestDevice, _test_devices[i]);
		EAT_EXEPTIONS
    }

    //adView.loadAd(adRequest);
    _env->CallVoidMethod(adView, _AdView_loadAd, adRequest);
	EAT_EXEPTIONS

    //framelayout.addView(adView, 1280, 90);
    _env->CallVoidMethod(_main_layout, _FrameLayout_addView, adView,
                         static_cast<int>(size.getWidth()*_density),
                         static_cast<int>(size.getHeight()*_density));
	EAT_EXEPTIONS

    _env->DeleteLocalRef(adRequest);
	EAT_EXEPTIONS

    _banners_map[id] = adView;

}

}
