#include "StoreBridge/cocos2dx_EventHandlers.h"
#include "StoreBridge/cocos2dx_StoreController.h"
#include "AppDelegate.h"
#include "Core/Browser.h"
#include "Core/Statistics.h"
#include "Logic/Language.h"
#include "Core/Notification.h"
#include "Core/Distributor.h"
cocos2dx_EventHandlers* cocos2dx_EventHandlers::instance=0;

cocos2dx_EventHandlers* cocos2dx_EventHandlers::getInstance()
{
    if(instance == 0)
        instance = new cocos2dx_EventHandlers();
    return instance;
}

void cocos2dx_EventHandlers::addHandler(IEventHandler* )
{}
void cocos2dx_EventHandlers::removeHandler(IEventHandler*)
{}
void cocos2dx_StoreController::initialize(string)
{}
void cocos2dx_StoreController::restoreTransactions()
{
#ifndef JUNIOR
    unsigned int i = rand() % 2;
    bool res = i == 1;
    AppDelegate::getPurchaseHandler()->restoreTransactions(res);
#endif
}

void cocos2dx_StoreController::buyMarketItem(string item)
{
#ifndef JUNIOR
    AppDelegate::getPurchaseHandler()->marketPurchase(item);
#endif
}
void cocos2dx_StoreController::storeOpening()
{}
void cocos2dx_StoreController::storeClosing()
{}


std::string Language::getDeviceLanguage()
{
    return "en";
}

void Statistics::logEvent(const std::string& name, const Params& p)
{
    cocos2d::CCLog("<Event '%s'", name.c_str());
    const Params::SVec& s_keys = p.getKeysString();
    const Params::SVec& s_values = p.getValuesString();

    if(s_keys.size() == s_values.size())
    {
        for(unsigned int i=0; i<s_keys.size(); ++i)
        {
            cocos2d::CCLog("-Param '%s'='%s'", s_keys[i].c_str(), s_values[i].c_str());
        }
    }

    cocos2d::CCLog("Event End>");
}

void Notification::notify(const Type t)
{
    cocos2d::CCLog("Notification ID %d", t);

}

#include "Core/Ads.h"
namespace ads
{
BannerID AdsManager::assignNewBannerId()
{
    return rand();
}

void AdsManager::createNativeBanner(const BannerID id, const BannerSize& size, const CCPoint& position)
{
    cocos2d::CCLog("Banner created %dx%d at position x=%d y=%d with id=%d", size.getWidth(),
                   size.getHeight(), int(position.x), int(position.y), id);
}

void AdsManager::showNativeBanner(const BannerID id)
{
    cocos2d::CCLog("Banner shown id=%d", id);
}

void AdsManager::hideNativeBanner(const BannerID id)
{
    cocos2d::CCLog("Banner hide id=%d", id);
}

void AdsManager::destroyNativeBanner(const BannerID id)
{
    cocos2d::CCLog("Banner destroyed id=%d", id);
}

void AdsManager::isNativeBannerShown(const BannerID id)
{
    bannerShownResult(false, id);
}


}

#include <stdio.h>

void realLog(const char * pszFormat, ...)
{
    const unsigned int MAX_LENGTH = 500;
    char buf[MAX_LENGTH];

    va_list args;
    va_start(args, pszFormat);
    vsnprintf(buf, MAX_LENGTH, pszFormat, args);
    va_end(args);

    CCLog(buf);
}

static float _density = 1;

void setDensity(float v)
{
    _density = v;
}

float getDensity()
{
    return _density;
}
namespace ads
{
void AdsManager::nativeInit()
{

}

float AdsManager::getDeviceDensity()
{
    return getDensity();
}
}

void Browser::openWebURL(const std::string& url)
{
    CCLog("Open Web url: %s", url.c_str());
}
void Browser::openMarketItem(const std::string& item_id)
{
    CCLog("Open market item: %s", item_id.c_str());
}
void Browser::sendMail(const std::string& email, const std::string& subject)
{
    CCLog("Send letter: %s (%s)", email.c_str(), subject.c_str());
}

Distributor::Store Distributor::getCurrentStore()
{
    return Distributor::GooglePlay;
}
