#ifndef SETTINGS_H
#define SETTINGS_H
#include "cocos2d-A.h"
#include "Layers/YesNoDialog.h"
#include "SceneStyle.h"
class Settings: public SceneStyle
{
public:
    Settings();
    virtual bool init();
    static cocos2d::CCScene* scene();
private:
//    SpritesLoader _settings_menu;
//    SpritesLoader _settings_menu_new;
    std::vector<ADMenuItem*> _menu_item;
    cocos2d::CCSprite* _menu_name;
    bool _sound_on;
    bool _music_on;
    bool _expert_mode_on;


    void doDeleteProgress();
    void onBackClick();

    void hideEverything(const Action &callback);
    void onExpertModeSelect(ADMenuItem* sender);
    void onSoundSelect(ADMenuItem* sender);
    void onMusicSelect(ADMenuItem* sender);
    void onDevelopersSelect();

    void onResetProgressSelect();
    void onRestorePurchasesSelect();

public:
    CREATE_FUNC(Settings)
};

#endif // SETTINGS_H
