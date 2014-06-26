#ifndef UNLOCKWINDOW_H
#define UNLOCKWINDOW_H
#include "cocos2d.h"
#include "Layers/PopUpWindow.h"
#include "Store.h"

class UnlockWindow : public PopUpWindowStyle
{

public:
    UnlockWindow(const unsigned int stamp_to_unlock)
        : _stamp_to_unlock(stamp_to_unlock)
    {}
private:
    void onPlayMoreClick()
    {
        closeWindow();
    }
    void onUnlockClick()
    {
        //Store::buyItem(Store::ITEM_UNLOCK_ALL);
    }



    void onCreate(cocos2d::CCNode *parent);
    unsigned int _stamp_to_unlock;
};
#endif // UNLOCKWINDOW_H
