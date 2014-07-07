#ifndef JUNIOR
#include "cocos2d.h"
#include "PurchaseHandler.h"
#include "Layers/BackgroundHolder.h"
#include "Store.h"
#include "Logic/RW.h"
#include "Scenes/SelectCollection.h"
#include "Scenes/LevelScene.h"
#include "Logic/SavesManager.h"
using namespace cocos2d;


bool isPrefixItem(const std::string& item_id, const std::string& prefix)
{
    if(item_id.length() < prefix.length())
    {
        return false;
    }

    std::string text_prefix(item_id.begin(), item_id.begin()+prefix.length());
    return text_prefix == prefix;
}
unsigned int strToInt(const std::string& str)
{
    std::stringstream ss(str);
    unsigned int num = 0;
    ss >> num;
    return num;
}

unsigned int itemIdToId(const std::string& item_id, const std::string& prefix)
{
    unsigned int res = 0;
    if(item_id.length() > prefix.length())
    {
        std::string number(item_id.begin()+prefix.length(), item_id.end());
        res = strToInt(number);
    }
    return res;
}

void InAppDelegate::purchaseSuccessful(const ADInApp::ProductID& itemId)
{
    //bool kill_ads = false;
    if(itemId == Store::BUY_FULL_VERSION)
    {
        RW::unlockAllCollectionsPurchased();
        SelectCollection::purchaseReload();

        SavesManager::getInstance()->setFullVersion(true);

        RW::disableAds();
        ADAds::disableAds();
    }
//    if(itemId == Store::ITEM_UNLOCK_ALL)
//    {
//        RW::unlockAllCollectionsPurchased();
//        SelectCollection::purchaseReload();
//        kill_ads = true;
//    }
//    else if(itemId == Store::ITEM_BUY_ALL)
//    {
//        RW::buyAllCollectionsPurchased();
//        SelectCollection::purchaseOpenCollections();
//        kill_ads = true;
//    }
//    else if(isPrefixItem(itemId, Store::PREFIX_BUY_COLLECTION))
//    {
//        unsigned int coll_id = itemIdToId(itemId, Store::PREFIX_BUY_COLLECTION);
//        RW::buyCollection(coll_id);
//        SelectCollection::purchaseOpenCollections();
//        kill_ads = true;
//    }
//    else if(isPrefixItem(itemId, Store::PREFIX_BUY_HINTS))
//    {
//        unsigned int count = itemIdToId(itemId, Store::PREFIX_BUY_HINTS);
//        RW::addHints(count);
//        LevelScene::purchaseUpdateHints();
//        kill_ads = true;
//    }
//    if(itemId == Store::ITEM_KILL_ADS || kill_ads)
//    {
//        if(!RW::isAdsDisabled())
//        {
//            RW::disableAds();

//            if (itemId == Store::ITEM_KILL_ADS)
//            {
//                RW::addHints(10);
//                LevelScene::purchaseUpdateHints();
//            }

//            ADAds::disableAds();
//        }
//    }
}
void InAppDelegate::purchaseFailed(const ADInApp::ProductID&, const ADInApp::ErrorType)
{}

void InAppDelegate::restorePurchasesSuccessfully()
{
    ADNotification::showNotification("Purchases were restored successfully");
}

void InAppDelegate::restorePurchasesFailed()
{
    ADNotification::showNotification("Error has occurred");
}

#endif



