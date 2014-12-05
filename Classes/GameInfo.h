#ifndef GAMEINFO_H
#define GAMEINFO_H
#include "cocos2d-A.h"
#include <string>

enum class GameVersion{
    VersionPlus,
    VerionMultiply,
    Version100,
    Version50
};

enum class PurchaseType{
    //Ads shown, levels locked, purchase
    AdsPurchaseFullVersion,
    //Ads shown, levels unlocked, purchase
    AdsPurchaseDisableAds,
    //Ads shown, levels unlocked, no purchase
    AdsNoPurchase,
    //No ads, levels unlocked
    NoAdsNoPurchase
};


class GameInfo
{
public:
    static std::string FACEBOOK_URL;
    static std::string SITE_URL;
    static std::string BACKGROUND_MUSIC;

    //sizes
    static int SIZE_MENU_TITLE;
    static bool isFreeCollection(int collection_id);
    //colors
    static cocos2d::ccColor3B COLOR_LIGHT_BLUE;
    static cocos2d::ccColor3B COLOR_DARK_BLUE;
    static cocos2d::ccColor3B COLOR_DARK_GREEN;
    static cocos2d::ccColor3B COLOR_ORANGE;
    static cocos2d::ccColor3B COLOR_RED;
    static cocos2d::ccColor3B COLOR_GRAY;
    static cocos2d::ccColor3B COLOR_LIGHT_GRAY;
    static cocos2d::ccColor3B COLOR_DARK;

    //how much hints add every day
    static int HINT_ADD;
    static bool useMultiply ()
    {
        if(GAME_VERSION == GameVersion::VersionPlus)
           return false;
        else
            return true;

    }
    static bool isExpertModeAvaliable();

    //set collection and levels for RatePopUpWindow
    static int RATE_COLLECTIONID_FIRST;
    static int RATE_COLLECTIONID_SECOND;

    static int RATE_LEVELID_FIRST;
    static int RATE_LEVELID_SECOND;

    //set collections and levels for BuyFullVersionWindow
    static int BUY_COLLECTIONID_FIRST;
    static int BUY_COLLECTIONID_SECOND;

    static int BUY_LEVELID_FIRST;
    static int BUY_LEVELID_SECOND;

    //tutorial levls
    static int TUTORIAL_COLLECTIONID_FIRST;
    static int TUTORIAL_LEVELID_FIRST;

    static int TUTORIAL_COLLECTIONID_SECOND;
    static int TUTORIAL_LEVELID_SECOND;

    static const GameVersion GAME_VERSION;
    static const PurchaseType PURCHASE_TYPE;


    static const ADPackageName &getMyPackageName();
    static const std::string& getPurchaseID();
    static void setPurchaseID(const std::string& );

    static const ADPackageName APP_MIF_27_FREE;
    static const ADPackageName APP_MIF_27_FREE_DISABLE_ADS;
    static const ADPackageName APP_MIF_27_FREE_NO_PURCHASE;

    static const ADPackageName APP_MIF_MULTIPLICATION_FREE;
    static const ADPackageName APP_MIF_MULTIPLICATION_PREMIUM;
private:
    static std::string _purchase_id;
    //static const std::string doGetPackageName(const std::string game_name);
};

#endif // GAMEINFO_H
