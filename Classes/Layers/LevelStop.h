#ifndef LEVELSTOP_H
#define LEVELSTOP_H
#include "cocos2d-A.h"
#include "Layers/EquationDrawer.h"
#include "DebugLayer.h"

class LevelStop: public cocos2d::CCLayer, public HasSlots
{
private:
    LevelStop(EquationDrawer* parent, Level* level, LevelScene* scene ) :
        _parent(parent), _level(level), _level_scene(scene)
    {}
    EquationDrawer* _parent;
    Level* _level;
    cocos2d::CCPoint _sheet_target_position;
    CCNode* _sheet_menu;
    LevelScene* _level_scene;
    //std::vector<cocos2d::CCSprite*> _menu;


    void onSkipLevel();

    void onFoundSolutions();

    void onLevels();
public:
    virtual bool init();
    void selfDestroy();
    void hideMe(const ADCallFunc::Action&);

    void onResume();
    void onRestart();

    static LevelStop* create(EquationDrawer* obj,
                             Level* lev,
                             LevelScene* lev_scen)
    {
        LevelStop *pRet = new LevelStop(obj,lev,lev_scen);
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
typedef std::vector<Solution> Solutions;
class FoundSolutions: public cocos2d::CCLayer
{
private:
    FoundSolutions(const Solutions sol):
        _solutions(sol)
    {

    }

    Solutions _solutions;
public:
    virtual bool init();
    void selfDestroy();
    void hideMe(const ADCallFunc::Action&);
    static cocos2d::CCScene* scene(const Solutions sol);

    FoundSolutions* create(const Solutions sol)
    {
        FoundSolutions *pRet = new FoundSolutions(sol);
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

#endif // LEVELSTOP_H
