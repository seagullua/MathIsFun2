#include "Core/Ads.h"



namespace ads
{







BannerID AdsManager::assignNewBannerId()
{
    return 0;
}

void AdsManager::createNativeBanner(const BannerID, const BannerSize&, const CCPoint&)
{

}

void AdsManager::showNativeBanner(const BannerID)
{
}

void AdsManager::hideNativeBanner(const BannerID)
{
}

void AdsManager::nativeInit()
{
}
float AdsManager::getDeviceDensity()
{
    return 1;
}

void AdsManager::destroyNativeBanner(const BannerID)
{
}

void AdsManager::isNativeBannerShown(const BannerID id)
{
	AdsManager::getInstance().bannerShownResult(id, false);
}

}
