#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d-A.h"
#include "Layers/YesNoDialog.h"
class MainMenu : public cocos2d::CCLayer
{
private:
    MenuSpriteBatch* _main_menu;

    MenuSpriteBatch* _share_menu;

    CCSprite* _logo;
    AnimatedMenuItem* _facebook_button;
    AnimatedMenuItem *_settings;
    AnimatedMenuItem *_play;
    CCSprite* _junior;


    PopUpWindowManager _pop_up_manager;
    void hideEverything(cocos2d::CCCallFunc *callback);
    void onShareFacebookPressed(CCObject*);
    //Action when user presses on Play
    void onPlayPressed(CCObject* pSender);
    //Action after all animation have finished
    void doPlayPressed();

    //Action when user presses Settings
    void onSettingsPressed(CCObject* pSender);
    //Action after all animation have finished
    void doSettingsPressed();


    MenuSpriteBatch* _poll_menu;
    AnimatedMenuItem *_poll_yes;
    AnimatedMenuItem *_poll_no;
    CCSprite* _poll;
    void pollYes(CCObject* pSender);
    void pollNo(CCObject* pSender);
    void pollHide();
    void pollShow();

    //on back clicked
    void keyBackClicked();
    //void keyMenuClicked();


    void doCloseGame();
public:
    MainMenu();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();


    CREATE_FUNC(MainMenu)
};


#endif // __MAIN_MENU_SCENE_H__
