#include "AppDelegate.h"
#include "Scenes/Loading.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Logic/Language.h"
#include "Core/Ads.h"
#include "Core/RealLog.h"
#include "Core/Distributor.h"
#include "Core/Browser.h"
#include "Store.h"
#include "Core/EncStrings.h"

USING_NS_CC;

#ifdef CC_WIN8_METRO
#include "CCEGLView.h"
#endif

#include "Logic/RW.h"
#ifndef JUNIOR
PurchaseHandler AppDelegate::_purchase_handler;
PurchaseHandler* AppDelegate::getPurchaseHandler()
{
    return &_purchase_handler;
}
#endif

AppDelegate::AppDelegate()
    : _loaded(false)
{

}

AppDelegate::~AppDelegate() 
{
    RW::release();
#ifndef JUNIOR
    cocos2dx_EventHandlers::getInstance()->removeHandler(&_purchase_handler);
#endif
    //delete _purchase_handler;
}
#ifdef CC_WIN8_METRO
bool AppDelegate::initInstance()
{
    bool bRet = false;
    do
    {



        // fix bug: 16bit aligned
        void* buff=_aligned_malloc(sizeof(CCEGLView),16);
        CCEGLView* mainView = new (buff) CCEGLView();

        CCDirector *pDirector = CCDirector::sharedDirector();
        mainView->Create();
        pDirector->setOpenGLView(mainView);



        bRet = true;
    } while (0);
    return bRet;
}
#endif
bool AppDelegate::applicationDidFinishLaunching() {

    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

#ifndef CC_WIN8_METRO
    pDirector->setOpenGLView(pEGLView);
#endif

    Language::setLanguage(Language::getDeviceLanguage());

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    CCSize vsize = pEGLView->getVisibleSize();

    CCSize frameSize = pEGLView->getFrameSize();
    //std::cout << "TESST" <<std::endl;
    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.
    float width_change = vsize.width / designResolutionSize.width;
    // if the frame's height is larger than the height of medium resource size, select large resource.
    std::string resourse_directory = mediumResource.directory;
    float resourse_scale = 1;

    if(frameSize.width < 500)
    {
        resourse_directory = "little";
        resourse_scale = 0.375;
    }
    Screen::setDesignScale(resourse_scale);
    CCFileUtils::sharedFileUtils()->setResourceDirectory(resourse_directory.c_str());
    pDirector->setContentScaleFactor(MIN(mediumResource.size.height*resourse_scale/designResolutionSize.height/width_change, mediumResource.size.width*resourse_scale/designResolutionSize.width/width_change));
    /*if (frameSize.height > mediumResource.size.height)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
    else
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(smallResource.directory);
        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }*/

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

    ads::AdsManager& manager = ads::AdsManager::getInstance();
#ifndef JUNIOR
    Distributor::Store store = Distributor::getCurrentStore();
    if(store == Distributor::GooglePlay)
    {
        manager.disableAds();
    }
    else
    {
        std::stringstream pid;
        //ca-app-pub-1097233306002326/4129712095
        pid << "ca-app-pub-" << 109723330 << 6002326 << "/" << 4129712095;

        manager.registerBannerType("BANNER", pid.str());
        manager.registerBannerType("IAB_BANNER", pid.str());

        std::stringstream pid_interstitial;
        //ca-app-pub-1097233306002326/6366969291
        pid_interstitial << "ca-app-pub-" << 109723330 << 6002326 << "/" << 6366969291;
        manager.registerInterstitialType(pid_interstitial.str());

        //manager.addTestDevice("419CBB113860522A7AB95487DBB0CC2B"); //Andriy Tab
        manager.addTestDevice("9AC43D4250441F63E2E677C2C06F5D41"); //Diana Tab
        manager.addTestDevice("C31238A94F2B52E9F4B77E58270A3943"); //Tonya

        //manager.prepareInterstitial();
    }

#else
    manager.disableAds();
#endif
    return true;
}
void AppDelegate::AdsOffListener::wasClickedDisableAllAds()
{
    realLog("Close ads clicked");
    Store::buyItem(Store::ITEM_KILL_ADS);
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    realLog("Pause Music");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    RW::saveGame();


}
#include "Core/MusicSettings.h"
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(MusicSettings::isBackgrHolderMusic() && MusicSettings::isMusicOn())
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
#else

    // if you use SimpleAudioEngine, it must resume here
    realLog("Resume Music");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
#endif

    //if(_loaded)
    //{
    //    ads::AdsManager::getInstance().stopAllAds();
    //}

}
