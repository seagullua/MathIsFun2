#include "StoreBridge/cocos2dx_EventHandlers.h"
#include "StoreBridge/cocos2dx_StoreController.h"
#include "AppDelegate.h"
#include "Core/Browser.h"
#include "Core/Statistics.h"
#include "Logic/Language.h"
#include "Core/Notification.h"
#include "Core/Distributor.h"

#include "cocos2d.h"


static float _density = 1;

void setDensity(float v)
{
    _density = v;
}

float getDensity()
{
    return _density;
}


#define AD_ADS_NOSIZE
#include <ADLib/PlatformImpl/ADAds_NoAds.hpp>
cocos2d::CCSize ADAds::Platform::getBannerSize(const BannerType& type)
{
    cocos2d::CCSize base (468, 60);
    if(type == "BANNER")
        base = cocos2d::CCSize(320, 50);

    return cocos2d::CCSize(base.width * getDensity(), base.height * getDensity());
}

#define AD_LANGUAGE_CURRENT "en"

#include <ADLib/PlatformImpl/ADBrowser_None.hpp>
#include <ADLib/PlatformImpl/ADStatistics_None.hpp>
#include <ADLib/PlatformImpl/ADLanguage_Default.hpp>
#include <ADLib/PlatformImpl/ADNotification_None.hpp>
#include <ADLib/PlatformImpl/ADInfo_Default.hpp>
#include <ADLib/PlatformImpl/ADInApp_Emulator.hpp>


