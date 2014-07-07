#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d-A.h"
#include "Layers/YesNoDialog.h"
#include "SceneStyle.h"
class MainMenu : public SceneStyle
{
private:
    cocos2d::CCMenu* _main_menu;

    cocos2d::CCMenu* _share_menu;

    cocos2d::CCSprite* _logo;
    ADMenuItem* _facebook_button;
    ADMenuItem *_settings;
    ADMenuItem *_play;
    ADMenuItem *_buy_full_version;


    void hideEverything(const Action& callback);
    void onShareFacebookPressed();
    void onBuyFullVersionClicked();

    //Action when user presses on Play
    void onPlayPressed();

    //Action when user presses Settings
    void onSettingsPressed();

    //on back clicked
    void onBackClick();

    void doCloseGame();
    static MainMenu* _me;
public:
    MainMenu();
    ~MainMenu();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    static void purchaseReload();
    CREATE_FUNC(MainMenu)
};


#endif // __MAIN_MENU_SCENE_H__
