#include "AppDelegate.h"
#include "Scenes/Loading.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Logic/Language.h"

#include "Store.h"
#include <ADLib/Device/ADInApp.h>
#include "PurchaseHandler.h"
USING_NS_CC;

#ifdef CC_WIN8_METRO
#include "CCEGLView.h"
#endif

#include "Logic/RW.h"

AppDelegate::AppDelegate()
    : _loaded(false)
{

}

AppDelegate::~AppDelegate() 
{
    RW::release();
}
void initInAppPurchases()
{
    typedef ADInApp::Product Product;
    ADStore store = ADInfo::getStore();


    ADInApp::Price price = "$1.99";
    if (store == ADStore::iTunes)
        price = "$2.99";
    Product buy_full_version("buy_full_version",price);



    if(store == ADStore::GooglePlay)
    {
        ADInApp::setStoreKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1eWcrKw7sfifjhLGCq+X7jOrFeh7eJdw3uqi9nFcP4cHRPwgpSqYm3+qn3CQNbBQ9D+rQXewnaKSaR3drVvl4cOPOOkO7iXiuA/t6AWx1dpOivPhzXCN+9vzz6wXVkzi0fPXydGNM1bbTQA0mQxmR+9j4HWfKgrnP6+3XoLHNahhf2ltLfFDukYl0woYrG9DBvFllmUfpaLLoLJL/9TmT8FWknSN8izUqfL4VwkBaceb/yIIELL3SEFnGQkUV7nO26E+giPNY7JRoHv1oQBPEcGNTxxM5nFZfp+2TYOtpGlznjy7yRwfGB+B2hhqZAM22GBBSlAMLm/XnD/jelDuIwIDAQAB");

        buy_full_version.setParameter("type", "non-consumable");
    }


    ADInApp::addProduct(buy_full_version);

    ADInApp::setDelegate(std::make_shared<InAppDelegate>());

}

bool AppDelegate::applicationDidFinishLaunching()
{
    //TODO: new keys set
    //Statistics init

    if(ADInfo::getPlatform() == ADPlatform::Android)
        ADStatistics::setApplicationKey("H4R6T9SPWVBVT5JCNJTZ");
    else if(ADInfo::getPlatform() == ADPlatform::iOS)
        ADStatistics::setApplicationKey("479VHJ5J349H24W94XKQ");
    ADStatistics::startSession();
    ADInfo::getStore();

    //Language init
    ADLanguage::addSupportedLanguage("en");
    ADLanguage::addSupportedLanguage("ru");
    ADLanguage::addSupportedLanguage("uk");

    ADLanguage::setDefaultLanguage("en");
    ADLanguage::getLanguage();

    initInAppPurchases();

    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    CCSize vsize = pEGLView->getVisibleSize();

    CCSize frameSize = pEGLView->getFrameSize();

    float width_change = vsize.width / designResolutionSize.width;
    // if the frame's height is larger than the height of medium resource size, select large resource.
    std::string resourse_directory = mediumResource.directory;
    float resourse_scale = 1;

//    if(frameSize.width < 500)
//    {
//        resourse_directory = "little";
//        resourse_scale = 0.375;
//    }
    std::vector<std::string> searchPath;
    searchPath.push_back(resourse_directory);

    ADScreen::setDesignScale(resourse_scale);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);

    pDirector->setContentScaleFactor(MIN(mediumResource.size.height*resourse_scale/designResolutionSize.height/width_change, mediumResource.size.width*resourse_scale/designResolutionSize.width/width_change));


    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);




    
    //std::ifstream is(CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile());
    // create a scene. it's an autorelease object
    CCScene *pScene = Loading::scene();

    // run
    pDirector->runWithScene(pScene);
    _loaded = true;


    ADStore store = ADInfo::getStore();
    ADPlatform platform = ADInfo::getPlatform();

    std::stringstream pid_interstitial;


    pid_interstitial << "ca-app-pub-" << 16126979 << 60946304 << "/";

    if(platform == ADPlatform::Android)
    {
        if(store == ADStore::SamsungStore)
        {
            //ca-app-pub-1612697960946304/3318394274
            pid_interstitial << 3318394274;
        }
        else if(store == ADStore::GooglePlay)
        {
            //ca-app-pub-1612697960946304/7149173479
            pid_interstitial << 7149173479;
        }
        else
        {
            //ca-app-pub-1612697960946304/7888194679
            pid_interstitial << 7888194679;
        }
    }
    else if(platform == ADPlatform::iOS)
    {
        //ca-app-pub-1612697960946304/9225327070
        pid_interstitial << 9225327070;
    }

    ADAds::registerInterstitialType(pid_interstitial.str());


    //Init ADMenuItem
    ADMenuItem::setAllClicksAction([](){
        ADSoundManager::playSoundEffect("music/choose.wav");
    });


    //fonts
    std::string cyrilic_font  = "fonts/Flow-Bold.ttf";

    //std::string latin_font = "fonts/Patrick Hand.ttf";

    ADLanguage::addSupportedLanguage("en", cyrilic_font);
    ADLanguage::addSupportedLanguage("uk", cyrilic_font);
    ADLanguage::addSupportedLanguage("ru", cyrilic_font);

    ADLanguage::setDefaultLanguage("en");
    ADLanguage::getLanguage();

    return true;
}
//void AppDelegate::AdsOffListener::wasClickedDisableAllAds()
//{
//    CCLog("Close ads clicked");
//    Store::buyItem(Store::ITEM_KILL_ADS);
//}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    ADDeviceEvents::onPause();


}
#include <ADLib/PlatformImpl/ADVirtualCurrency_None.hpp>
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    ADDeviceEvents::onResume();
}
