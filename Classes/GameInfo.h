#ifndef GAMEINFO_H
#define GAMEINFO_H
#include "cocos2d-A.h"
#include <string>

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
    static bool useMultiply ()
    {
        return false;
    }


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
};

#endif // GAMEINFO_H
