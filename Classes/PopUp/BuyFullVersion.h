#ifndef BUYFULLVERSION_H
#define BUYFULLVERSION_H

#include "Layers/PopUpWindow.h"
#include "Store.h"
#include "GameInfo.h"

class BuyFullVersion : public PopUpWindowStyle
{

public:
    BuyFullVersion()
    {}
private:
    void onBuyFullVersionClick()
    {
        Store::buyItem(GameInfo::getPurchaseID());
    }
    void onBuyLaterClick()
    {
        //Store::buyItem(Store::ITEM_BUY_ALL);
        this->closeWindow();
    }



    void onCreate(cocos2d::CCNode *parent);
};

#endif // BUYFULLVERSION_H
