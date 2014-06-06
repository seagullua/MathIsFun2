#ifndef LEVEL_SCENE_H
#define LEVEL_SCENE_H
#include "cocos2d-A.h"
#include "Logic/Level.h"
#include "Layers/PopUpWindow.h"
#include <ADLib/Device/ADAds.h>
#include "SceneStyle.h"

class EquationDrawer;
class LevelStop;
class LevelScenePopUp;
class FoundSolutionsLayer;
class LevelScene : public SceneStyle
{
public:
    LevelScene(Level* level);
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(Level* level);
    virtual void removeChild(CCNode* child, bool cleanup);

    void skipLevel(CCObject*);
    void levels();
    void restart();
    void showFoundSolutions();
    static void purchaseUpdateHints();
private:
    static LevelScene* _last_scene;
    void do_skipLevel();
    void do_levels();
    void do_restart();

    EquationDrawer* _equation;
    FoundSolutionsLayer* _found_solutions;
    Level* _level;
    LevelStop* _levelStop;
    cocos2d::CCLabelBMFont* _level_from;
    cocos2d::CCLabelBMFont* _hint_quantity;
    bool _found_solution_is_opened;

    ADMenuItem* _play;
    ADMenuItem* _pause;
    ADMenuItem* _restart;
    ADMenuItem* _hint;
    cocos2d::CCMenu* _screenEllements;
    LevelScenePopUp* _last_pop_up;
    Solution _hint_solution;
    ADAds::Banner* _top_banner;

    class BuyHints;
    void onKeyPauseClicked();
    void keyPauseClicked();
    void keyRestartClicked();
    void keyHintClicked();
    void onkeyRestartClicked();
    void onBackClick();

    void do_goToLevelSelect();
    void renewOneHint();
public:
    void hideMe(bool hide_all = false);
    void hidePauseAndPlay();
    void showPause();
    void showMe();

    void onFoundSolutionOpen(LevelScenePopUp* pop_up)
    {
        assert(!_last_pop_up);
        _last_pop_up =pop_up;
        _found_solution_is_opened = true;
    }
    void onFoundSolutionClose()
    {
        _found_solution_is_opened = false;
        _last_pop_up = 0;
    }

    static LevelScene* create(Level* level)
    {
        LevelScene *pRet = new LevelScene(level);
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }


};

#endif // LEVEL_SCENE_H
