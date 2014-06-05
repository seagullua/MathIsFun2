#ifndef DEVELOPERS_H
#define DEVELOPERS_H
#include "cocos2d-A.h"
class Developers: public cocos2d::CCLayer
{
public:
    Developers();
    virtual bool init();
    static cocos2d::CCScene* scene();
private:
    CCSprite* _menu_name;
    CCSprite* _developers;
    CCSprite* _version;
    ADMenuItem* _x4enjoy_item;
    ADMenuItem* _send_item;
    void keyBackClicked();
    void onKeyBackClicked()
    {
        keyBackClicked();
    }
    void doGoBack();
    void hideEverything(cocos2d::CCCallFunc *callback);
    void onSendLetter(CCObject*);
    void onSite(CCObject*);
public:
    CREATE_FUNC(Developers)
};

#endif // DEVELOPERS_H
