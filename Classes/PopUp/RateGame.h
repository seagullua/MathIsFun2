#ifndef RATEGAME_H
#define RATEGAME_H
#include "Layers/PopUpWindow.h"
#include "Logic/SavesManager.h"

class RateGame: public PopUpWindowStyle
{

public:
    RateGame()
    {}
private:
    void onRateNowClick()
    {
        //get store and aplication
        SavesManager::showedRateMe();
        //TODO: open rating in Store
    }
    void onRateLaterClick()
    {
        this->closeWindow();
    }



    void onCreate(cocos2d::CCNode *parent);
};

#endif // RATEGAME_H
