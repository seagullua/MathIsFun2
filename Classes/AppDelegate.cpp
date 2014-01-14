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

        AdsOffListener* listener = new AdsOffListener;
        listener->autorelease();
        listener->retain();

        //manager.addSupportedBannerSize(ads::BannerSize::BANNER_BIG);
        //manager.addSupportedBannerSize(ads::BannerSize::BANNER_SMALL);

        //std::string pid = "ca-app-pub-1097233306002326/4129712095";
        CREATE_DECODED_STRING(pid, CODE(0x3797,0x7a70,0x4907,0xd5e,0x17a6,0x72cd,0x3442,0x70a5,0xd6b,0x264,0x4b5a,0x266a,0x20c8,0x3c4c,0x44,0x7fa,0x73fd,0x75ee,0x788d,0x2ae,0x645d,0x5bdc,0xd2d,0x272e,0x3253,0x1f33,0x1f73,0x379b,0x1f92,0x1d0c,0x68af,0x4c86,0x55b1,0x43ec,0x2674,0x6040,0x108f,0x1af2), CODE(0x37f4,0x7a11,0x492a,0xd3f,0x17d6,0x72bd,0x346f,0x70d5,0xd1e,0x206,0x4b77,0x265b,0x20f8,0x3c75,0x73,0x7c8,0x73ce,0x75dd,0x78be,0x29e,0x646b,0x5bec,0xd1d,0x271c,0x3260,0x1f01,0x1f45,0x37b4,0x1fa6,0x1d3d,0x689d,0x4cbf,0x5586,0x43dd,0x2646,0x6070,0x10b6,0x1ac7),38);

        //manager.setBannerZoneKey(pid);
        manager.registerBannerType("BANNER", pid);
        manager.registerBannerType("IAB_BANNER", pid);

        std::string pid_inter = "ca-app-pub-1097233306002326/6366969291";
        manager.registerInterstitialType(pid_inter);

        manager.addTestDevice("419CBB113860522A7AB95487DBB0CC2B"); //Andriy Tab
        manager.addTestDevice("9AC43D4250441F63E2E677C2C06F5D41"); //Diana Tab
        manager.addTestDevice("C31238A94F2B52E9F4B77E58270A3943"); //Tonya


//        manager.addOwnItem(ads::OwnAdItem(
//                               new ads::UrlClickAction(
//                                   Browser::createRedirectURL(
//                                       "mif-kids-"+Distributor::getStoreName(store))))
//                           .addImage(ads::BannerSize::BANNER_BIG, "banners/banner1_big.png")
//                           .addImage(ads::BannerSize::BANNER_SMALL, "banners/banner1_small.png"));
    }

    //    std::string junior_in_market = "";
    //
    //    if(store == Distributor::GooglePlay)
    //        junior_in_market = "market://details?id=com.x4enjoy.mathisfunjunior";
    //    else if(store == Distributor::iTunes)
    //        junior_in_market = "samsungapps://ProductDetail/com.x4enjoy.mathisfunjunior";

    //    manager.addOwnItem(ads::OwnAdItem(
    //                           new ads::UrlClickAction(Browser::MARKET_PERFIX + junior_in_market))
    //                       .addImage(ads::BannerSize::BANNER_BIG, "banners/banner2_big.png")
    //                       .addImage(ads::BannerSize::BANNER_SMALL, "banners/banner2_small.png"));
#else
    manager.stopAllAdsCurrentSeanse();
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
