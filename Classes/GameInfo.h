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
    static cocos2d::ccColor3B COLOR_GRAY;
};

#endif // GAMEINFO_H
