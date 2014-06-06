#ifndef TUTORIAL_H
#define TUTORIAL_H
class EquationDrawer;
class Level;
class Loading;
class LevelScenePopUp;
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
    void onSubstitutionMade(EquationDrawer*);
    void onSolutionFound(LevelScenePopUp*);

    void hideLabels();
    void showLabels();
private:
    Tutorial();
    void init();
    friend class Loading;
    static Tutorial _tutorial;
    bool _initialized;
    void switchLabel1To(cocos2d::CCSprite*);
    void switchLabel2To(cocos2d::CCSprite*);
    void disposeLabel2();
    void disposeLabel1();
    void disposeLabel(cocos2d::CCSprite *& what);
    void switchLabel(cocos2d::CCSprite *& what, cocos2d::CCSprite* to);
    cocos2d::CCSprite* _help_label1;
    cocos2d::CCSprite* _help_label2;
};

#endif // TUTORIAL_H
