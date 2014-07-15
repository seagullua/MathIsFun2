#ifndef GAMEINFO_H
#define GAMEINFO_H
#include "cocos2d-A.h"
#include <string>

enum class GameVersion{
    VersionPlus,
    VerionMultiply
};


class GameInfo
{
public:
    static std::string FACEBOOK_URL;
    static std::string SITE_URL;
    static std::string BACKGROUND_MUSIC;

    //sizes
    static int SIZE_MENU_TITLE;

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

    static GameVersion GAME_VERSION;


    static const ADPackageName &getMyPackageName();

    static ADPackageName APP_MIF_27_FREE;
    static ADPackageName APP_MIF_27_PREMIUM;
    static ADPackageName APP_MIF_MULTIPLICATION_FREE;
    static ADPackageName APP_MIF_MULTIPLICATION_PREMIUM;
private:
    //static const std::string doGetPackageName(const std::string game_name);
};

#endif // GAMEINFO_H
