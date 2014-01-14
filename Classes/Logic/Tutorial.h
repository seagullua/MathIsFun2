#ifndef TUTORIAL_H
#define TUTORIAL_H
class EquationDrawer;
class Level;
class Loading;
class LevelScenePopUp;
#include "cocos2d-A.h"
class Tutorial : public CCObject
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
    void switchLabel1To(CCSprite*);
    void switchLabel2To(CCSprite*);
    void disposeLabel2();
    void disposeLabel1();
    void disposeLabel(CCSprite *& what);
    void switchLabel(CCSprite *& what, CCSprite* to);
    CCSprite* _help_label1;
    CCSprite* _help_label2;
};

#endif // TUTORIAL_H
