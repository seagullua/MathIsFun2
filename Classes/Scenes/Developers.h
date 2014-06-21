#ifndef DEVELOPERS_H
#define DEVELOPERS_H
#include "cocos2d-A.h"
#include "SceneStyle.h"
class Developers: public SceneStyle
{
public:
    Developers();
    virtual bool init();
    static cocos2d::CCScene* scene();

private:
    cocos2d::CCLabelTTF* _developers_window_title;
    ADMenuItem* _x4enjoy_item;

    void onBackClick();

    void hideEverything(const Action& callback);
    void onSendLetter();
    void onSite();
public:
    CREATE_FUNC(Developers)
};

#endif // DEVELOPERS_H
