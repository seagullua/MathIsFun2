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


    ADInApp::Price price = "$0.99";
    if (store == ADStore::iTunes)
        price = "$1.99";

    std::string purchase_name;
    std::string samsung_id;
    std::string google_play_key;

    if(GameInfo::GAME_VERSION == GameVersion::VersionPlus)
    {
        if(GameInfo::PURCHASE_TYPE == PurchaseType::AdsPurchaseFullVersion)
        {
            samsung_id = "100000104146";
            purchase_name = "buy_full_version";
            google_play_key = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1eWcrKw7sfifjhLGCq+X7jOrFeh7eJdw3uqi9nFcP4cHRPwgpSqYm3+qn3CQNbBQ9D+rQXewnaKSaR3drVvl4cOPOOkO7iXiuA/t6AWx1dpOivPhzXCN+9vzz6wXVkzi0fPXydGNM1bbTQA0mQxmR+9j4HWfKgrnP6+3XoLHNahhf2ltLfFDukYl0woYrG9DBvFllmUfpaLLoLJL/9TmT8FWknSN8izUqfL4VwkBaceb/yIIELL3SEFnGQkUV7nO26E+giPNY7JRoHv1oQBPEcGNTxxM5nFZfp+2TYOtpGlznjy7yRwfGB+B2hhqZAM22GBBSlAMLm/XnD/jelDuIwIDAQAB";
        }
        else
        {
            samsung_id = "100000104197";
            purchase_name = "mif27kids_ads_purchase";
            google_play_key = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAi3YVhJJvS59LeRF8QXROUgWJdROSQKX8DEFfLevVt0wUR3q+1D5t99OtAsWZxjgC6jO/+HgvGPTwL9wKrUpRRnsaVdhAFPZ1zcEWqrkgTJEs2g2DwxA1NIWKberTgF5/8GSgG/rkhNKqSrRhoxh7H1BWcutHAOcpew0gvXJ0CfLO2ziKSKatog9t/mCdoK3BrnLp8D/lHstPDFWjZnS3874GZLxjVG4Xg+X7JyCfdIPwHhg8XpV+0MJR5MUGcNkVUq7dW1y3RmjDtuOWJKM9+0omotc79wRnKnDZ2nEtSlAmNIK3lhptsYRgjGuKDLbFG9DHplRk6wIzKxeIK4iltwIDAQAB";
        }
    }
    else if(GameInfo::GAME_VERSION == GameVersion::VerionMultiply)
    {
        //Multiplication
        samsung_id = "100000104197???";
        purchase_name = "mif_mul_ads_purchase";
        google_play_key = "";
    }
    else
    {
        assert(false);
    }


    GameInfo::setPurchaseID(purchase_name);
    Product buy_full_version(purchase_name,price);



    if(store == ADStore::GooglePlay)
    {
        ADInApp::setStoreKey(google_play_key);
        buy_full_version.setParameter("type", "non-consumable");
    }
    else if(store == ADStore::SamsungStore)
    {
        ADInApp::setStoreKey(samsung_id);

        buy_full_version.setParameter("samsung-id",purchase_name);

    }
    else if(store == ADStore::AmazonStore)
    {
        buy_full_version.setParameter("asku",purchase_name);
    }
    else if(store == ADStore::iTunes)
    {
        buy_full_version.setParameter("asku",purchase_name);
    }


    ADInApp::addProduct(buy_full_version);

    ADInApp::setDelegate(std::make_shared<InAppDelegate>());

}

bool AppDelegate::applicationDidFinishLaunching()
{
    //Statistics init
    if(GameInfo::GAME_VERSION == GameVersion::VersionPlus)
    {
        if(ADInfo::getPlatform() == ADPlatform::Android)
            ADStatistics::setApplicationKey("3WK4P7YZWV5WBCSZWJTT");
        else if(ADInfo::getPlatform() == ADPlatform::iOS)
            ADStatistics::setApplicationKey("2N2D57R7FYHMY4PHNC7Y");
        else if(ADInfo::getPlatform() == ADPlatform::WindowsPhone)
            ADStatistics::setApplicationKey("DN44PJ9MC8QFPQPCHYCR");
    }
    else if(GameInfo::GAME_VERSION == GameVersion::VerionMultiply)
    {
        if(ADInfo::getPlatform() == ADPlatform::Android)
            ADStatistics::setApplicationKey("QFXY32JBS4WBCVHN6HBP");
        else if(ADInfo::getPlatform() == ADPlatform::iOS)
            ADStatistics::setApplicationKey("R23GSVSZJ2T7W2WZ2XPY");
        else if(ADInfo::getPlatform() == ADPlatform::WindowsPhone)
            ADStatistics::setApplicationKey("FMDVW8R5B5JX3MTQFTTQ");
    }

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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
#else
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
#endif
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

    if(GameInfo::GAME_VERSION == GameVersion::VersionPlus)
    {
        if(platform == ADPlatform::Android)
        {
            if(store == ADStore::SamsungStore)
            {
                //ca-app-pub-1612697960946304/5795034672
                pid_interstitial << 5795034672;
            }
            else if(store == ADStore::GooglePlay)
            {
                //ca-app-pub-1612697960946304/7149173479
                pid_interstitial << 7149173479;
            }
            else
            {
                //ca-app-pub-1612697960946304/5795034672
                pid_interstitial << 5795034672;
            }
        }
        else if(platform == ADPlatform::iOS)
        {
            //ca-app-pub-1612697960946304/9357276676
            pid_interstitial << 9357276676;
        }
        else if(platform == ADPlatform::WindowsPhone)
        {
            //ca-app-pub-1612697960946304/4272772278
            pid_interstitial << 4272772278;
        }
    }
    else if(GameInfo::GAME_VERSION == GameVersion::VerionMultiply)
    {
        if(platform == ADPlatform::Android)
        {
            //ca-app-pub-1612697960946304/1975977070
            pid_interstitial << 1975977070;
        }
        else if(platform == ADPlatform::iOS)
        {
            //ca-app-pub-1612697960946304/6406176672
            pid_interstitial << 6406176672;
        }
        else if(platform == ADPlatform::WindowsPhone)
        {
            //ca-app-pub-1612697960946304/3313109472
            pid_interstitial << 3313109472;
        }
    }
    else
    {
        assert(false);
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
