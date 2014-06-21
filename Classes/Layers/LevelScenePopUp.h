#ifndef LEVELSCENEPOPUP_H
#define LEVELSCENEPOPUP_H
#include "cocos2d-A.h"

class EquationDrawer;
class LevelScenePopUp : public cocos2d::CCNode, public HasSlots
{
private:
    LevelScenePopUp(EquationDrawer* parent) : _parent(parent), _crown(0), _pause_banner(0)
    {}
    //void remove_self();

    EquationDrawer* _parent;
    CCNode* _sheet_menu;
    cocos2d::CCPoint _sheet_target_position;
    cocos2d::CCSprite* _crown;
    ADAds::Banner* _pause_banner;
    void onFindMoreSolutions();
    void onLevels();



    void showInterstitial();
public:
    void hideMe(const ADCallFunc::Action& callback);
    virtual bool init();
    void onNextLevel();
    //void onCloseClicked(CCObject* pSender);
    //void onResumeClicked(CCObject* pSender);
    void selfDestroy();
    EquationDrawer* getEquationDrawer()
    {
        return _parent;
    }

    cocos2d::CCSprite* getCrown();
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
