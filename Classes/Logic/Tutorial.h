#ifndef TUTORIAL_H
#define TUTORIAL_H
class EquationDrawer;
class Level;
class Loading;
//class LevelScenePopUp;
#include "cocos2d-A.h"


class Tutorial : public cocos2d::CCObject
{
public:
    static Tutorial* getInstance()
    {
        return &_tutorial;
    }

    void onLevelStart(EquationDrawer*);
    void onEmptySpaceTouch(EquationDrawer*);

    void hideLabels();
    void showLabels();
private:
    Tutorial();
    void init();
    friend class Loading;
    static Tutorial _tutorial;
    bool _initialized;

    cocos2d::CCSprite* _help_arrow;
};

#endif // TUTORIAL_H
