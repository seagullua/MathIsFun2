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

    Product hint_10("hint_10", "$0.99");
    Product hint_50("hint_50", "$1.99");
    Product hint_1000("hint_1000", "$4.99");

    Product buy_4000("buy_4000", "$0.99");
    Product buy_3000("buy_3000", "$0.99");
    Product buy_2000("buy_2000", "$0.99");
    Product buy_1000("buy_1000", "$0.99");

    Product buy_all("buy_all", "$1.99");
    Product kill_ads("kill_ads", "$0.99");
    Product unlock_all("unlock_all", "$0.99");


    ADStore store = ADInfo::getStore();
    if(store == ADStore::GooglePlay)
    {
        ADInApp::setStoreKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzczlRlnsAzWe4OxJmP5FimkV1yZHIq2WZJ0hjMRj0kej4qBOs44d5udwZSsvFpQU19H/K1i9AgYzjimwDe2boOcsfIKQ69UqG3qugyPnwkJafOGrNodB/zPKVwdSx2XmmbgQdIO8j2XGUkhAxQdGR/q2sKUycaodBum5n5H3PeyP/UlSci6djDenU4BN5It6bBulRgQpPFFdzptBRWFXJJ1WF+Js+OGY376Q+evp+9I+74JRJ8RSgwTgrnD3AO2b+GxvwQtZWcS4qCLpZ4BKqcBh1i842HqaKlaJIB7Ktn+p0P3pb0SUiUKdTrfUq6TBn22tkOcVnPlDAUjtXAFLQQIDAQAB");


        hint_10.setParameter("type", "consumable");
        hint_50.setParameter("type", "consumable");
        hint_1000.setParameter("type", "consumable");

        buy_4000.setParameter("type", "non-consumable");
        buy_3000.setParameter("type", "non-consumable");
        buy_2000.setParameter("type", "non-consumable");
        buy_1000.setParameter("type", "non-consumable");

        buy_all.setParameter("type", "non-consumable");
        kill_ads.setParameter("type", "non-consumable");
        unlock_all.setParameter("type", "non-consumable");
    }

    ADInApp::addProduct(hint_10);
    ADInApp::addProduct(hint_50);
    ADInApp::addProduct(hint_1000);

    ADInApp::addProduct(buy_4000);
    ADInApp::addProduct(buy_3000);
    ADInApp::addProduct(buy_2000);
    ADInApp::addProduct(buy_1000);

    ADInApp::addProduct(buy_all);
    ADInApp::addProduct(kill_ads);
    ADInApp::addProduct(unlock_all);

    ADInApp::setDelegate(std::make_shared<InAppDelegate>());




}

bool AppDelegate::applicationDidFinishLaunching()
{
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
    //ADLanguage::addSupportedLanguage("es");
    //ADLanguage::addSupportedLanguage("ja");
    //ADLanguage::addSupportedLanguage("pt");
    //ADLanguage::addSupportedLanguage("de");
    //ADLanguage::addSupportedLanguage("fr");
    //ADLanguage::addSupportedLanguage("ko");
    //ADLanguage::addSupportedLanguage("tr");
    //ADLanguage::addSupportedLanguage("it");
    ADLanguage::addSupportedLanguage("uk");
    //ADLanguage::addSupportedLanguage("nl");
    //ADLanguage::addSupportedLanguage("pl");
    //ADLanguage::addSupportedLanguage("hu");

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

    std::stringstream pid_banner;
    std::stringstream pid_interstitial;


    pid_banner << "ca-app-pub-" << 16126979 << 60946304 << "/";
    pid_interstitial << "ca-app-pub-" << 16126979 << 60946304 << "/";

    if(platform == ADPlatform::Android)
    {
        if(store == ADStore::SamsungStore)
        {
            //ca-app-pub-1612697960946304/1841661079
            pid_banner  << 1841661079;
            //ca-app-pub-1612697960946304/3318394274
            pid_interstitial << 3318394274;
        }
        else if(store == ADStore::GooglePlay)
        {
            //ca-app-pub-1612697960946304/3457995077
            pid_banner << 3457995077;
            //ca-app-pub-1612697960946304/4934728274
            pid_interstitial << 4934728274;
        }
        else
        {
            //ca-app-pub-1612697960946304/6411461477
            pid_banner << 6411461477;
            //ca-app-pub-1612697960946304/7888194679
            pid_interstitial << 7888194679;
        }
    }
    else if(platform == ADPlatform::iOS)
    {
        //ca-app-pub-1612697960946304/7748593879
        pid_banner << 7748593879;

        //ca-app-pub-1612697960946304/9225327070
        pid_interstitial << 9225327070;
    }

    ADAds::registerBannerType("BANNER", pid_banner.str());
    ADAds::registerBannerType("IAB_BANNER", pid_banner.str());

    ADAds::registerInterstitialType(pid_interstitial.str());

    if(platform == ADPlatform::Android)
    {
        //ADAds::addTestDevice("419CBB113860522A7AB95487DBB0CC2B"); //Andriy Tab
        ADAds::addTestDevice("9AC43D4250441F63E2E677C2C06F5D41"); //Diana Tab
        ADAds::addTestDevice("C31238A94F2B52E9F4B77E58270A3943"); //Tonya
    }

    //Init ADMenuItem
    ADMenuItem::setAllClicksAction([](){
        ADSoundManager::playSoundEffect("music/choose.wav");
    });


    //fonts
    std::string latin_font = "fonts/Flow Bold.ttf";

    std::string cyrilic_font = "fonts/Patrick Hand.ttf";

    ADLanguage::addSupportedLanguage("en", latin_font);
    //ADLanguage::addSupportedLanguage("uk", cyrilic_font);

    ADLanguage::setDefaultLanguage("en");
    ADLanguage::getLanguage();

    return true;
}
void AppDelegate::AdsOffListener::wasClickedDisableAllAds()
{
    CCLog("Close ads clicked");
    Store::buyItem(Store::ITEM_KILL_ADS);
}

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
