#include "Settings.h"
#include "Scenes/MainMenu.h"
#include "Scenes/Developers.h"
#include "Logic/RW.h"
#include "Logic/Language.h"
#include "Store.h"
#include <ADLib/Device/ADSoundManager.h>
#include <ADLib/Device/ADLanguage.h>
#include "GameInfo.h"
static const int NORMAL_SPRITE = 10;
static const int SELECTED_SPRITE = 20;

using namespace cocos2d;

void switchImages(ADMenuItem* item);



Settings::Settings():
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
    CCSize VISIBLE_SIZE = ADScreen::getVisibleSize();

    //Get the screen start of cordinates
    CCPoint ORIGIN = ADScreen::getOrigin();
    float SCALE = ADScreen::getScaleFactor();
    float x_middle_of_sheet = (VISIBLE_SIZE.width-133/SCALE)/2 + ORIGIN.x;


    /////////////////////////////////////////////////////

    //set scene title
    _settings_scene_title = CCLabelTTF::create(_("settings_menu.scene.title"),
                                           ADLanguage::getFontName(),
                                           45);
    _settings_scene_title->setColor(GameInfo::COLOR_DARK_BLUE);
    _settings_scene_title->setAnchorPoint(ccp(0.5,1));
    CCPoint settings_scene_title_target_position = ccp(x_middle_of_sheet,
                                       VISIBLE_SIZE.height + ORIGIN.y - 50/SCALE);
    _settings_scene_title->setPosition(settings_scene_title_target_position);

    //make slowly come to the screen
    _settings_scene_title->setOpacity(0);
    CCFadeTo* settings_scene_title_fade_in = CCFadeTo::create(0.2f, 255);
    _settings_scene_title->runAction(settings_scene_title_fade_in);
    this->addChild(_settings_scene_title);

    ////////////////////////////////////////////////////////

    //create function list
    CCMenu* menu = CCMenu::create();
    CCMenu* menu_new = CCMenu::create();
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

    //////////////////////////////////////////////////////

    //set Developers title, Restore, Reset
    CCLabelTTF* developers_title = CCLabelTTF::create(_("settings_menu.developers.title"),
                                           ADLanguage::getFontName(),
                                           45);
    developers_title->setColor(GameInfo::COLOR_LIGHT_BLUE);
    ADMenuItem* developers = ADMenuItem::create(developers_title);
    CONNECT(developers->signalOnClick,
            this, &Settings::onDevelopersSelect);

    _menu_item.push_back(developers);

    /////////////////////////////////////////////////////

    ADMenuItem* reset_progress = ADMenuItem::create(
                CCSprite::create("settings/Reset_progress.png"));
    CONNECT(reset_progress->signalOnClick,
            this, &Settings::onResetProgressSelect);

    _menu_item.push_back(reset_progress);

    //////////////////////////////////////////

    ADMenuItem* restore = ADMenuItem::create(
                CCSprite::create("settings/Restore_purchases.png"));
    CONNECT(restore->signalOnClick,
            this, &Settings::onRestorePurchasesSelect);
    _menu_item.push_back(restore);


    menu->addChild(_menu_item[3]);
    menu->addChild(_menu_item[4]);
    menu->addChild(_menu_item[5]);

    //_menu_item[3]->setPosition(ccp(0, -visibleSize.height/3 + origin.y ));
    menu->alignItemsVerticallyWithPadding(20/SCALE);
    menu_new->alignItemsHorizontallyWithPadding(70/SCALE);
    menu->setPosition(ccp(x_middle_of_sheet,130/SCALE + ORIGIN.y));
    menu_new->setPosition(ccp(x_middle_of_sheet, VISIBLE_SIZE.height/2 + 30/SCALE + ORIGIN.y));
    restore->setPositionY(restore->getPositionY()+30/SCALE);

    this->addChild(menu);
    this->addChild(menu_new);
    _pop_up_manager.addMenuToAutoDisable(menu);
    _pop_up_manager.addMenuToAutoDisable(menu_new);

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
    _settings_scene_title->runAction(settings_move);

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
