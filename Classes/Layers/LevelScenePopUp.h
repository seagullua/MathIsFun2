#ifndef LEVELSCENEPOPUP_H
#define LEVELSCENEPOPUP_H
#include "cocos2d-A.h"
#include "Core/Ads.h"
class EquationDrawer;
class LevelScenePopUp : public cocos2d::CCNode
{
private:
    LevelScenePopUp(EquationDrawer* parent) : _parent(parent), _crown(0), _pause_banner(0)
    {}
    //void remove_self();

    EquationDrawer* _parent;
    CCNode* _sheet_menu;
    CCPoint _sheet_target_position;
    CCSprite* _crown;
    ads::Banner* _pause_banner;
    void onFindMoreSolutions(CCObject*);
    void onLevels(CCObject*);

    void do_onLevels();
    void do_onNextLevel();


public:
    void hideMe(CCCallFunc* callback);
    virtual bool init();
    void onNextLevel(CCObject*);
    //void onCloseClicked(CCObject* pSender);
    //void onResumeClicked(CCObject* pSender);
    void selfDestroy();
    EquationDrawer* getEquationDrawer()
    {
        return _parent;
    }

    CCSprite* getCrown();
    static LevelScenePopUp* create(EquationDrawer* obj)
    {
        LevelScenePopUp *pRet = new LevelScenePopUp(obj);
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



#endif // LEVELSCENEPOPUP_H
