#ifndef RATEGAME_H
#define RATEGAME_H
#include "Layers/PopUpWindow.h"
#include "Logic/SavesManager.h"
#include <ADLib/Device/ADBrowser.h>
#include "GameInfo.h"

class RateGame: public PopUpWindowStyle
{

public:
    RateGame()
    {}
private:
    void onRateNowClick()
    {
        SavesManager::getInstance()->showedRateMe();
        ADStatistics::logEvent("Rate Me Clicked");

        cocos2d::CCLog("on Rate Level clicked");
        ADBrowser::openApplicationPage(GameInfo::getPackageName("mif28"));
        this->closeWindow();
    }
    void onRateLaterClick()
    {
        this->closeWindow();
    }



    void onCreate(cocos2d::CCNode *parent);
};

#endif // RATEGAME_H
