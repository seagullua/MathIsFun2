#include "Store.h"

#include <ADLib/Device/ADInApp.h>
#include "Layers/BackgroundHolder.h"
//std::string Store::ITEM_UNLOCK_ALL="unlock_all";
//std::string Store::ITEM_BUY_ALL="buy_all";
//std::string Store::PREFIX_BUY_COLLECTION="buy_";
//std::string Store::PREFIX_BUY_HINTS="hint_";
//std::string Store::ITEM_KILL_ADS="kill_ads";
std::string Store::BUY_FULL_VERSION="buy_full_version";

bool Store::buyItem(const std::string& item)
{
#ifndef JUNIOR
    ADInApp::buyProduct(item);
#endif
    return true;
}
void Store::restorePurchases()
{
#ifndef JUNIOR
    ADInApp::restorePurchases();
#endif
}
