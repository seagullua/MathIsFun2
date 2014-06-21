#include "Settings.h"
#include "Scenes/MainMenu.h"
#include "Scenes/Developers.h"
#include "Logic/RW.h"
#include "Logic/Language.h"
#include "Store.h"
#include <ADLib/Device/ADSoundManager.h>
static const int NORMAL_SPRITE = 10;
static const int SELECTED_SPRITE = 20;

using namespace cocos2d;

void switchImages(ADMenuItem* item);



Settings::Settings():_menu_name(0),
    _sound_on(ADSoundManager::isSoundTurnedOn()),
    _music_on(ADSoundManager::isMusicTurnedOn()),
    _expert_mode_on(RW::isExpertMode())
{
}

ADMenuItem* createToggleButton(const std::string& normal_sprite,
                               const std::string& selected_sprite,
                               bool is_selected)
{
    CCSprite* expert_mode_on = CCSprite::create(normal_sprite.c_str());
    expert_mode_on->setTag(NORMAL_SPRITE);
    CCSprite* expert_mode_off = CCSprite::create(selected_sprite.c_str());
    expert_mode_off->setTag(SELECTED_SPRITE);
    expert_mode_off->setVisible(false);
    expert_mode_off->setPosition(expert_mode_off->getContentSize()*0.5f);

    ADMenuItem* expert_mode = ADMenuItem::create(expert_mode_on);
    expert_mode->addChild(expert_mode_off);

    if(is_selected)
        switchImages(expert_mode);
    return expert_mode;
}

bool Settings::init()
{
    if ( !SceneStyle::init() )
    {
        return false;
    }

    //To trigger back button
    this->setKeypadEnabled(true);

    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();

    //Get the screen start of cordinates
    CCPoint origin = ADScreen::getOrigin();
    float scaled = ADScreen::getScaleFactor();
    float x_middle_of_sheet = (visibleSize.width-133/scaled)/2 + origin.x;


    //Get the Select level label
    _menu_name = CCSprite::create(Language::localizeFileName("settings/Settings.png").c_str());
    this->addChild(_menu_name);
    //Put this label at the top of the screen
    _menu_name->setAnchorPoint(ccp(0.5, 1));
    CCPoint logo_target_position = ccp(x_middle_of_sheet,
                                       visibleSize.height + origin.y - 50/scaled);
    _menu_name->setPosition(logo_target_position);
    //_menu_name->setColor(ccc3(71,218,196));
    //Make it fade in slowly

    //make slowly come to the screen
    _menu_name->setOpacity(0);
    CCFadeTo* developers_fade_in = CCFadeTo::create(0.2f, 255);
    _menu_name->runAction(developers_fade_in);


    //    _settings_menu = GraphicsManager::getLoaderFor(
    //                0,
    //                Language::localizeFileName("settings/settings_menu.plist").c_str(),
    //                Language::localizeFileName("settings/settings_menu.png").c_str());

    //    _settings_menu_new = GraphicsManager::getLoaderFor(
    //                0,
    //                "settings/settings_menu_new.plist",
    //                "settings/settings_menu_new.png");
    //Create menu with collections
    CCMenu* menu = CCMenu::create();
    CCMenu* menu_new = CCMenu::create();

    //TODO: calculate is Expert mode off or on and show the correct logo
    //expert_mode
    _menu_item.reserve(6);


    ADMenuItem* expert_mode = createToggleButton(
                "settings/Expert_mode_on.png",
                "settings/Expert_mode_off.png",
                !_expert_mode_on);
    expert_mode->setClickAction([expert_mode, this](){
        this->onExpertModeSelect(expert_mode);
    });
    _menu_item.push_back(expert_mode);

    ADMenuItem* sound = createToggleButton(
                "settings/Sound_on.png",
                "settings/Sound_off.png",
                !ADSoundManager::isSoundTurnedOn());
    sound->setClickAction([sound, this](){
        this->onSoundSelect(sound);
    });
    _menu_item.push_back(sound);


    ADMenuItem* music = createToggleButton(
                "settings/Music_on.png",
                "settings/Music_off.png",
                !ADSoundManager::isMusicTurnedOn());
    music->setClickAction([music, this](){
        this->onMusicSelect(music);
    });
    _menu_item.push_back(music);


    menu_new->addChild(_menu_item[0]);
    menu_new->addChild(_menu_item[1]);
    menu_new->addChild(_menu_item[2]);
    //menu->menu()->alignItemsVerticallyWithPadding(30/scaled);
    //menu->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //ADMenuItem* developers =
    ADMenuItem* developers = ADMenuItem::create(
                CCSprite::create("settings/Developers.png"));
    CONNECT(developers->signalOnClick,
            this, &Settings::onDevelopersSelect);

    _menu_item.push_back(developers);

    ADMenuItem* reset_progress = ADMenuItem::create(
                CCSprite::create("settings/Reset_progress.png"));
    CONNECT(reset_progress->signalOnClick,
            this, &Settings::onResetProgressSelect);

    _menu_item.push_back(reset_progress);


    ADMenuItem* restore = ADMenuItem::create(
                CCSprite::create("settings/Restore_purchases.png"));
    CONNECT(restore->signalOnClick,
            this, &Settings::onRestorePurchasesSelect);
    _menu_item.push_back(restore);


    menu->addChild(_menu_item[3]);
    menu->addChild(_menu_item[4]);
    menu->addChild(_menu_item[5]);

    //_menu_item[3]->setPosition(ccp(0, -visibleSize.height/3 + origin.y ));
    menu->alignItemsVerticallyWithPadding(20/scaled);
    menu_new->alignItemsHorizontallyWithPadding(70/scaled);
    menu->setPosition(ccp(x_middle_of_sheet,130/scaled + origin.y));
    menu_new->setPosition(ccp(x_middle_of_sheet, visibleSize.height/2 + 30/scaled + origin.y));
    restore->setPositionY(restore->getPositionY()+30/scaled);

    this->addChild(menu);
    this->addChild(menu_new);
    _pop_up_manager.addMenuToAutoDisable(menu);
    _pop_up_manager.addMenuToAutoDisable(menu_new);
    //slowly came to the screen
    //for (unsigned int i=0; i<_menu_item.size(); ++i)
    //{
    // _menu_item[i]->setOpacity(0);
    // }

    for (unsigned int i=0; i<_menu_item.size(); ++i)
    {
        _menu_item[i]->setOpacity(0);
        CCFadeTo* fade_in = CCFadeTo::create(0.3f, 255);
        _menu_item[i]->runAction(fade_in);
    }
    return true;
}
cocos2d::CCScene* Settings::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    Settings *layer = Settings::create();

    // add layer as a child to scene
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(SceneStyle::simulateBackClick));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}
void Settings::onExpertModeSelect(ADMenuItem* item)
{
    if (_expert_mode_on==true)
    {

        RW::setExpertMode(false);
        _expert_mode_on=false;

    }
    else
    {
        RW::setExpertMode(true);
        _expert_mode_on=true;

    }
    switchImages(item);

}
void Settings::onResetProgressSelect()
{
    CCSprite* label = CCSprite::create(Language::localizeFileName("reset_progress.png").c_str());

    _pop_up_manager.openWindow(new YesNoDialog(label,
                                               this, callfunc_selector(Settings::doDeleteProgress)));

}
void Settings::doDeleteProgress()
{
    RW::deletePersistentInfo();
}

void Settings::onRestorePurchasesSelect()
{
    Store::restorePurchases();

}

void Settings::onSoundSelect(ADMenuItem* item)
{

    if (_sound_on==true)
    {
        _sound_on=false;
        ADSoundManager::turnOffSound();

    }
    else
    {
        _sound_on=true;
        ADSoundManager::turnOnSound();

    }
    switchImages(item);
}

void Settings::onMusicSelect(ADMenuItem* item)
{
    if (_music_on==true)
    {
        _music_on=false;
        ADSoundManager::turnOffMusic();

    }
    else
    {
        _music_on=true;
        ADSoundManager::turnOnMusic();

    }
    switchImages(item);
}
void Settings::onDevelopersSelect()
{
    ADStatistics::logEvent("Developers Click");
    CCDirector::sharedDirector()->replaceScene(Developers::scene());
}

void Settings::onBackClick()
{

    RW::flushSettings();
    hideEverything([](){
        CCDirector::sharedDirector()->replaceScene(MainMenu::scene());
    });

}


void Settings::hideEverything(const Action& callback)
{
    const float delay = 0.20f;
    CCFadeTo* settings_move = CCFadeTo::create(delay, 0);
    _menu_name->runAction(settings_move);

    float fade_out_duration = 0.15f;
    for(unsigned int i=0; i<_menu_item.size(); ++i)
    {
        CCFadeTo* item_fade = CCFadeTo::create(fade_out_duration, 0);
        _menu_item[i]->runAction(
                    item_fade);
        //delay += fade_out_duration/2;
        //fade_out_duration+=0.2f;
    }
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    ADCallFunc::create(callback),
                    NULL));
}
