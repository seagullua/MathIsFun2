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

    //colors
    static cocos2d::ccColor3B COLOR_LIGHT_BLUE;
    static cocos2d::ccColor3B COLOR_DARK_BLUE;
};

#endif // GAMEINFO_H
