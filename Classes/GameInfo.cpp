#include "GameInfo.h"
#include <ADLib/Device/ADSoundManager.h>

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
//GameVersion GameInfo::GAME_VERSION = GameVersion::VerionMultiply;
GameVersion GameInfo::GAME_VERSION = GameVersion::VersionPlus;//
//hints
int GameInfo::HINT_ADD = 3;

const std::string GameInfo::doGetPackageName(const std::string game_name)
{
    //TODO:chane it
    if(game_name =="mif28")
    {
        if(ADInfo::getPlatform() == ADPlatform::iOS)
            return "id657095501";
        else
            return "com.x4enjoy.mathisfun";
    }

}

const std::string GameInfo::getPackageName(std::string game_name)
{
    return doGetPackageName(game_name);
}
