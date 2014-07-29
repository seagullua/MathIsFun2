#include "GameInfo.h"
#include <ADLib/Device/ADSoundManager.h>
#include "Logic/SavesManager.h"

std::string GameInfo::FACEBOOK_URL = "http://www.facebook.com/4EnjoyGames";
std::string GameInfo::SITE_URL = "http://4enjoy.com/";
std::string GameInfo::BACKGROUND_MUSIC = "music/math_is_fun" MUSIC_EXT;

int GameInfo::SIZE_MENU_TITLE = 60;

cocos2d::ccColor3B GameInfo::COLOR_LIGHT_BLUE = cocos2d::ccc3(13,133,255);
cocos2d::ccColor3B GameInfo::COLOR_DARK_BLUE = cocos2d::ccc3(34,117,202);
cocos2d::ccColor3B GameInfo::COLOR_DARK_GREEN = cocos2d::ccc3(0,222,30);
cocos2d::ccColor3B GameInfo::COLOR_ORANGE= cocos2d::ccc3(255,144,0);
cocos2d::ccColor3B GameInfo::COLOR_GRAY= cocos2d::ccc3(75,75,75);
cocos2d::ccColor3B GameInfo::COLOR_LIGHT_GRAY= cocos2d::ccc3(121,121,121);
cocos2d::ccColor3B GameInfo::COLOR_RED= cocos2d::ccc3(250,70,10);
cocos2d::ccColor3B GameInfo::COLOR_DARK= cocos2d::ccc3(0,0,0);

int GameInfo::RATE_COLLECTIONID_FIRST=100;
int GameInfo::RATE_LEVELID_FIRST=15;

int GameInfo::RATE_COLLECTIONID_SECOND=200;
int GameInfo::RATE_LEVELID_SECOND=5;

//set collections and levels for BuyFullVersionWindow
int GameInfo::BUY_COLLECTIONID_FIRST=200;
int GameInfo::BUY_LEVELID_FIRST=10;


int GameInfo::BUY_COLLECTIONID_SECOND=200;
int GameInfo::BUY_LEVELID_SECOND=19;


//tutorial collections and levels
int GameInfo::TUTORIAL_COLLECTIONID_FIRST=100;
int GameInfo::TUTORIAL_LEVELID_FIRST=0;

int GameInfo::TUTORIAL_COLLECTIONID_SECOND=200;
int GameInfo::TUTORIAL_LEVELID_SECOND=0;

//game version
//TODO: change it to get another levels, resources and another

//hints
int GameInfo::HINT_ADD = 3;

const ADPackageName GameInfo::APP_MIF_27_FREE = ADPackageName::create(
            "com.x4enjoy.mathisfun.kids", //Android
            "id896468015", //iOS id
            "1af4d00f-fdc7-44e5-9486-1ab72d602e70"); //Windows Phone

const ADPackageName GameInfo::APP_MIF_27_FREE_DISABLE_ADS = ADPackageName::create(
            "com.x4enjoy.mathisfun.kids.free", //Android
            "id00...", //iOS id
            "1af4d00f-fdc7-44e5-9486-1ab72d602e70"); //Windows Phone

const ADPackageName APP_MIF_27_PREMIUM = ADPackageName::create(
	"com.x4enjoy.mathisfun.kids.premium", //Android
	"id00...", //iOS id
	"0000-",//Windows Phone
	"0000"); //Windows 8

const ADPackageName GameInfo::APP_MIF_MULTIPLICATION_FREE = ADPackageName::create(
            "com.x4enjoy.mathisfun.multiplication", //Android
            "id00000...", //iOS id
            "id-windows-phone"); //Windows Phone
const ADPackageName GameInfo::APP_MIF_MULTIPLICATION_PREMIUM = ADPackageName::create(
            "com.x4enjoy.mathisfun.multiplication.premium", //Android
            "id00000...", //iOS id
            "id-windows-phone"); //Windows Phone

std::string GameInfo::_purchase_id;


const std::string& GameInfo::getPurchaseID()
{
    return _purchase_id;
}

void GameInfo::setPurchaseID(const std::string& s)
{
    _purchase_id = s;
}

const ADPackageName& GameInfo::getMyPackageName()
{
    if(GAME_VERSION == GameVersion::VersionPlus)
    {
        if(PURCHASE_TYPE == PurchaseType::AdsPurchaseFullVersion) {
            return APP_MIF_27_FREE;
        } else if(PURCHASE_TYPE == PurchaseType::AdsPurchaseDisableAds) {
            return APP_MIF_27_FREE_DISABLE_ADS;
		}
		else {
			return APP_MIF_27_PREMIUM;
		}
    }
    else
    {
        return APP_MIF_MULTIPLICATION_FREE;
    }
}

bool GameInfo::isExpertModeAvaliable()
{
    return false;
}

bool GameInfo::isFreeCollection(int collection_id)
{
    if(SavesManager::isLevelsUnlocked())
    {
        return true;
    }
    else if(GAME_VERSION == GameVersion::VersionPlus)
    {
        return collection_id == 100 || collection_id == 200;
    }
    else if(GAME_VERSION == GameVersion::VerionMultiply)
    {
        return collection_id == 10001 || collection_id == 10002;
    }
    return false;
}
