#include "Settings.h"
#include "Scenes/MainMenu.h"
#include "Scenes/Developers.h"
#include "Core/MusicSettings.h"
#include "Logic/RW.h"
#include "Logic/Language.h"
#include "Store.h"
#include "Core/Statistics.h"

Settings::Settings():_menu_name(0),
    _sound_on(MusicSettings::isSoundEffectOn()),
    _music_on(MusicSettings::isMusicOn()),
    _expert_mode_on(RW::isExpertMode()), _pop_up_manager(this)
{
}
bool Settings::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    //To trigger back button
    this->setKeypadEnabled(true);

    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();

    //Get the screen start of cordinates
    CCPoint origin = Screen::getOrigin();
    float scaled = Screen::getScaleFactor();
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


    _settings_menu = GraphicsManager::getLoaderFor(
                0,
                Language::localizeFileName("settings/settings_menu.plist").c_str(),
                Language::localizeFileName("settings/settings_menu.png").c_str());

    _settings_menu_new = GraphicsManager::getLoaderFor(
                0,
                "settings/settings_menu_new.plist",
                "settings/settings_menu_new.png");
    //Create menu with collections
    MenuSpriteBatch* menu = MenuSpriteBatch::create(_settings_menu);
    MenuSpriteBatch* menu_new = MenuSpriteBatch::create(_settings_menu_new);

    //TODO: calculate is Expert mode off or on and show the correct logo
    //expert_mode
    _menu_item.reserve(6);

    if(_expert_mode_on)
    {
        _menu_item.push_back(AnimatedMenuItem::create(
                                 _settings_menu_new->loadSprite("Expert_mode_on.png"),
                                 this, menu_selector(Settings::onExpertModeSelect)));
    }
    else
    {
        _menu_item.push_back(AnimatedMenuItem::create(
                                 _settings_menu_new->loadSprite("Expert_mode_off.png"),
                                 this, menu_selector(Settings::onExpertModeSelect)));
    }

    if (MusicSettings::isSoundEffectOn() == true)
    {
        //sound =
        _menu_item.push_back(AnimatedMenuItem::create(
                                 _settings_menu_new->loadSprite("Sound_on.png"),
                                 this, menu_selector(Settings::onSoundSelect)));
    }
    else if(MusicSettings::isSoundEffectOn() == false)
    {
        //sound =
        _menu_item.push_back(AnimatedMenuItem::create(
                                 _settings_menu_new->loadSprite("Sound_off.png"),
                                 this, menu_selector(Settings::onSoundSelect)));
    }


    if (MusicSettings::isMusicOn() == true )
    {
        //music =
        _menu_item.push_back(AnimatedMenuItem::create(
                                 _settings_menu_new->loadSprite("Music_on.png"),
                                 this, menu_selector(Settings::onMusicSelect)));
    }
    else if (MusicSettings::isMusicOn() == false)
    {
        //music =
        _menu_item.push_back(AnimatedMenuItem::create(
                                 _settings_menu_new->loadSprite("Music_off.png"),
                                 this, menu_selector(Settings::onMusicSelect)));
    }

    menu_new->menu()->addChild(_menu_item[0]);
    menu_new->menu()->addChild(_menu_item[1]);
    menu_new->menu()->addChild(_menu_item[2]);
    //menu->menu()->alignItemsVerticallyWithPadding(30/scaled);
    //menu->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //AnimatedMenuItem* developers =
    _menu_item.push_back(AnimatedMenuItem::create(
                             _settings_menu->loadSprite("Developers.png"),
                             this, menu_selector(Settings::onDevelopersSelect)));

    _menu_item.push_back(AnimatedMenuItem::create(
                             _settings_menu->loadSprite("Reset_progress.png"),
                             this, menu_selector(Settings::onResetProgressSelect)));
#ifndef JUNIOR
    AnimatedMenuItem* restore = AnimatedMenuItem::create(
                _settings_menu->loadSprite("Restore_purchases.png"),
                this, menu_selector(Settings::onRestorePurchasesSelect));
    _menu_item.push_back(restore);
#endif

    menu->menu()->addChild(_menu_item[3]);
    menu->menu()->addChild(_menu_item[4]);
#ifndef JUNIOR
    menu->menu()->addChild(_menu_item[5]);
#endif
    //_menu_item[3]->setPosition(ccp(0, -visibleSize.height/3 + origin.y ));
    menu->menu()->alignItemsVerticallyWithPadding(20/scaled);
    menu_new->menu()->alignItemsHorizontallyWithPadding(70/scaled);
    menu->setPosition(ccp(x_middle_of_sheet,130/scaled + origin.y));
    menu_new->setPosition(ccp(x_middle_of_sheet, visibleSize.height/2 + 30/scaled + origin.y));
#ifndef JUNIOR
    restore->setPositionY(restore->getPositionY()+30/scaled);
#endif

    this->addChild(menu);
    this->addChild(menu_new);
    _pop_up_manager.addMenuToAutoDisable(menu->menu());
    _pop_up_manager.addMenuToAutoDisable(menu_new->menu());
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
                                          callfunc_selector(Settings::onKeyBackClicked));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}
void Settings::onExpertModeSelect(CCObject* sender)
{
    AnimatedMenuItem* item = dynamic_cast<AnimatedMenuItem*>(sender);
    CCAssert(item, "");
    if(item)
    {

        if (_expert_mode_on==true)
        {

            RW::setExpertMode(false);
            _expert_mode_on=false;
            CCNode* old = item->getNormalImage();
            item->setNormalImage(_settings_menu_new->loadSprite("Expert_mode_off.png"));
            old->removeFromParent();
        }
        else
        {
            RW::setExpertMode(true);
            _expert_mode_on=true;
            CCNode* old = item->getNormalImage();
            item->setNormalImage(_settings_menu_new->loadSprite("Expert_mode_on.png"));
            old->removeFromParent();
        }
    }
}
void Settings::onResetProgressSelect(CCObject*)
{
    CCSprite* label = CCSprite::create(Language::localizeFileName("reset_progress.png").c_str());

    _pop_up_manager.openWindow(new YesNoDialog(label,
                                               this, callfunc_selector(Settings::doDeleteProgress)));

}
void Settings::doDeleteProgress()
{
    RW::deletePersistentInfo();
}

void Settings::onRestorePurchasesSelect(CCObject*)
{
    Store::restorePurchases();

}

void Settings::onSoundSelect(CCObject* sender)
{
    AnimatedMenuItem* item = dynamic_cast<AnimatedMenuItem*>(sender);
    CCAssert(item, "");
    if(item)
    {
        if (_sound_on==true)
        {
            _sound_on=false;
            MusicSettings::turnOffSoundEffect();
            CCNode* old = item->getNormalImage();
            item->setNormalImage(_settings_menu_new->loadSprite("Sound_off.png"));
            old->removeFromParent();
        }
        else
        {
            _sound_on=true;
            MusicSettings::turnOnSoundEffect();
            CCNode* old = item->getNormalImage();
            item->setNormalImage(_settings_menu_new->loadSprite("Sound_on.png"));
            old->removeFromParent();
        }
    }
}

void Settings::onMusicSelect(CCObject* sender)
{
    AnimatedMenuItem* item = dynamic_cast<AnimatedMenuItem*>(sender);
    CCAssert(item, "");
    if(item)
    {
        BackgroundHolder::resetMusic();
        if (_music_on==true)
        {
            _music_on=false;
            MusicSettings::turnOffMusic();
            CCNode* old = item->getNormalImage();
            item->setNormalImage(_settings_menu_new->loadSprite("Music_off.png"));
            old->removeFromParent();
        }
        else
        {
            _music_on=true;
            MusicSettings::turnOnMusic();
            CCNode* old = item->getNormalImage();
            item->setNormalImage(_settings_menu_new->loadSprite("Music_on.png"));
            old->removeFromParent();
        }
    }
}
void Settings::onDevelopersSelect(CCObject* /*sender*/)
{
    ADStatistics::logEvent("Developers Click");
    CCDirector::sharedDirector()->replaceScene(Developers::scene());
}

void Settings::keyBackClicked()
{
    if(!_pop_up_manager.backAction())
    {
        RW::flushSettings();
        hideEverything(
                    CCCallFunc::create(
                        this,
                        callfunc_selector(Settings::doGoBack)));
    }
}
void Settings::doGoBack()
{
    CCDirector::sharedDirector()->replaceScene(MainMenu::scene());
}

void Settings::hideEverything(cocos2d::CCCallFunc *callback)
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
                    callback,
                    NULL));
}
