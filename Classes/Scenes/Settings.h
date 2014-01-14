#ifndef SETTINGS_H
#define SETTINGS_H
#include "cocos2d-A.h"
#include "Layers/YesNoDialog.h"
class Settings: public cocos2d::CCLayer
{
public:
    Settings();
    virtual bool init();
    static cocos2d::CCScene* scene();
private:
    SpritesLoader _settings_menu;
    SpritesLoader _settings_menu_new;
    std::vector<AnimatedMenuItem*> _menu_item;
    CCSprite* _menu_name;
    bool _sound_on;
    bool _music_on;
    bool _expert_mode_on;

    PopUpWindowManager _pop_up_manager;

    void doDeleteProgress();
    void keyBackClicked();
    void onKeyBackClicked()
    {
        keyBackClicked();
    }
    void doGoBack();
    void hideEverything(cocos2d::CCCallFunc *callback);
    void onExpertModeSelect(CCObject* sender);
    void onSoundSelect(CCObject* sender);
    void onMusicSelect(CCObject* sender);
    void onDevelopersSelect(CCObject* sender);

    void onResetProgressSelect(CCObject*);
    void onRestorePurchasesSelect(CCObject*);

public:
    CREATE_FUNC(Settings)
};

#endif // SETTINGS_H
