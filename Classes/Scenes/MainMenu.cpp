#include "MainMenu.h"
#include "AppMacros.h"
USING_NS_CC;
#include "SelectCollection.h"
#include "LevelScene.h"
#include "Settings.h"
#include "SimpleAudioEngine.h"
#include "Logic/Language.h"
#include "Core/Browser.h"
#include "Core/Ads.h"
#include "Core/MusicSettings.h"
MainMenu::MainMenu()
    :
      _main_menu(0),
      _logo(0),
      _pop_up_manager(this),
      _poll_menu(0),
      _poll_yes(0),
      _poll_no(0),
      _poll(0)
{

}

CCScene* MainMenu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MainMenu *layer = MainMenu::create();

    // add layer as a child to scene
    BackgroundHolder::backgroundSwitchTo(scene, 0);
    //scene->addChild(backgorund);
    scene->addChild(layer);

    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    //To trigger back button
#ifndef CC_WIN8_PHONE
    this->setKeypadEnabled(true);
#endif

    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = Screen::getOrigin();
    float scaled = Screen::getScaleFactor();

    //Get the sprites loader
    SpritesLoader spl = GraphicsManager::getLoaderFor(this,
                                                      "main_menu/images.plist",
                                                      "main_menu/images.png");
    SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                           "main_menu/menu.plist",
                                                           "main_menu/menu.png");
#ifndef JUNIOR
    SpritesLoader share_spl = GraphicsManager::getLoaderFor(0,
                                                      "main_menu/share.plist",
                                                      "main_menu/share.png");
#endif;


    //Place logo
    _logo = spl->loadSprite("logo.png");
    _logo->setScale(0.77f);
    _logo->setAnchorPoint(ccp(0.5, 1));

    float x_middle_of_sheet = (visibleSize.width-133/scaled)/2 + origin.x;
    CCPoint logo_target_position = ccp(x_middle_of_sheet,
                                       visibleSize.height + origin.y - 80/scaled);
    _logo->setPosition(ccp(x_middle_of_sheet,
                           2*visibleSize.height));

    CCMoveTo* logo_move = CCMoveTo::create(0.3f, logo_target_position);
    _logo->runAction(logo_move);

#ifdef JUNIOR
    _junior = spl->loadSprite("junior.png");
    _junior->setAnchorPoint(ccp(0,1));
    _junior->setPosition(ccp(origin.x + 48/scaled, origin.y + visibleSize.height - 32/scaled));
    _junior->setOpacity(0);

    _junior->runAction(
                CCSequence::createWithTwoActions(
                    CCDelayTime::create(1.1f),
                    CCFadeTo::create(0.8f, 255)));
#endif
    //Create menu items
    //Play button
    _play = AnimatedMenuItem::create(
                menu_spl->loadSprite("play.png"),
                this, menu_selector(MainMenu::onPlayPressed));

    //Settings button
    _settings = AnimatedMenuItem::create(
                menu_spl->loadSprite("settings.png"),
                this, menu_selector(MainMenu::onSettingsPressed));

#ifndef JUNIOR
    float right_x = visibleSize.width + origin.x - 122/scaled;
    float up_y = visibleSize.height + origin.y - 107/scaled;

    _share_menu = MenuSpriteBatch::create(share_spl);

    _facebook_button = AnimatedMenuItem::create
            (share_spl->loadSprite("facebook_logo.png"),
             this,
             menu_selector(MainMenu::onShareFacebookPressed));
    _facebook_button->setAnchorPoint(ccp(0.5,0.5));
    _facebook_button->setOpacity(0);
    CCFadeTo* facebook_move = CCFadeTo::create(0.5f, 255);
    _facebook_button->runAction
            (CCSequence::create(CCDelayTime::create(0.3f), facebook_move, NULL));

    _share_menu->setPosition(ccp(right_x,up_y));

    _share_menu->menu()->addChild(_facebook_button);
    this->addChild(_share_menu);
#endif
    //Create menu
    _main_menu = MenuSpriteBatch::create(menu_spl);
    _main_menu->menu()->addChild(_play);
    _main_menu->menu()->addChild(_settings);
    //_main_menu->menu()->alignItemsVertically();
    this->addChild(_main_menu);




    //Calculate menu position
    float logo_size = _logo->boundingBox().size.height;
    float logo_end = logo_target_position.y - logo_size;

    _play->setPosition(ccp(x_middle_of_sheet, logo_end / 2));
    _play->setAnchorPoint(ccp(0.5f, 0.5f));
    CCPoint menuPlay_target_position = _play->getPosition();
    _play->setPosition(ccp(_play->getPositionX(), -visibleSize.height));

    //_settings->setAnchorPoint(ccp(1, 0));

    _settings->setPosition(ccp(origin.x + visibleSize.width - 122/scaled, origin.y + 75/scaled));
    CCPoint settings_target_position = _settings->getPosition();
    _settings->setPosition(ccp(_settings->getPositionX(), -visibleSize.height));

    CCMoveTo* play_move = CCMoveTo::create(0.3f, menuPlay_target_position);
    _play->runAction(CCSequence::create(CCDelayTime::create(logo_move->getDuration()), play_move, NULL));
    CCMoveTo* settings_move = CCMoveTo::create(0.3f, settings_target_position);
    _settings->runAction(CCSequence::create(CCDelayTime::create(logo_move->getDuration()*2), settings_move, NULL));


    _main_menu->setPosition(ccp(0, 0));

    pollShow();

    _pop_up_manager.addMenuToAutoDisable(_main_menu->menu());
#ifndef JUNIOR
    _pop_up_manager.addMenuToAutoDisable(_share_menu->menu());
#endif



    return true;
}
void MainMenu::hideEverything(cocos2d::CCCallFunc* callback)
{

    CCFadeTo* settings_move = CCFadeTo::create(0.15f, 0);
    CCFadeTo* play_move = CCFadeTo::create(0.15f, 0);
    CCFadeTo* logo_move = CCFadeTo::create(0.15f, 0);
    _settings->runAction(settings_move);
    _play->runAction(CCSequence::create(CCDelayTime::create(0.15f), play_move, NULL));
    _logo->runAction(CCSequence::create(CCDelayTime::create(0.3f), logo_move, callback, NULL));

    pollHide();

#ifndef JUNIOR
    CCFadeTo* facebook_move = CCFadeTo::create(0.15f, 0);
    _facebook_button->runAction
            (CCSequence::create(CCDelayTime::create(0.15f), facebook_move, NULL));
#else
    _junior->stopAllActions();
    _junior->runAction(CCFadeTo::create(0.3f,0));
#endif



}


void MainMenu::keyBackClicked()
{
    if(!_pop_up_manager.backAction())
    {
        CCSprite* label = CCSprite::create("quite_promt.png");

        _pop_up_manager.openWindow(new YesNoDialog(label,
                                                   this, callfunc_selector(MainMenu::doCloseGame)));
    }
}

void MainMenu::onShareFacebookPressed(CCObject*)
{
    ADBrowser::openWebURL(createRedirectURL("100"));
    //Browser::openURL(Browser::FacebookGroup);
}

//Action when user presses on Play
void MainMenu::onPlayPressed(CCObject*)
{
    //SoundPool.setOnLoadCompleteListener();

    CCCallFunc* after_hide = CCCallFunc::create(this,
                                                callfunc_selector(MainMenu::doPlayPressed));
    hideEverything(after_hide);
}

//Action after all animation have finished
void MainMenu::doPlayPressed()
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
}

//Action when user presses Settings
void MainMenu::onSettingsPressed(CCObject*)
{
    CCCallFunc* after_hide = CCCallFunc::create(this,
                                                callfunc_selector(MainMenu::doSettingsPressed));
    hideEverything(after_hide);
}
#include "Logic/RW.h"
//Action after all animation have finished
void MainMenu::doSettingsPressed()
{
     CCDirector::sharedDirector()->replaceScene(Settings::scene());
}
#include "Core/Statistics.h"
void MainMenu::pollYes(CCObject* pSender)
{
//    ADStatistics::logEvent("Like translation", ADStatistics::Params().add("Language", Language::getLangCode()));
//    pollHide();
//    RW::pollIsShown();
}

void MainMenu::pollNo(CCObject* pSender)
{
//    Statistics::logEvent("Dislike translation", Statistics::Params().add("Language", Language::getLangCode()));
//    pollHide();
//    RW::pollIsShown();
}

void MainMenu::pollHide()
{
    /*if(_poll_menu)
        _poll_menu->menu()->setEnabled(false);

    if(_poll_yes)
        _poll_yes->runAction(CCFadeTo::create(0.3f, 0));

    if(_poll_no)
        _poll_no->runAction(CCFadeTo::create(0.3f, 0));

    if(_poll)
        _poll->runAction(CCFadeTo::create(0.3f, 0));*/


}

void MainMenu::pollShow()
{
    /*Language::Languages lang = Language::getLanguage();
    if(lang != Language::Russian && lang != Language::English && !RW::hasTranslationPollAlreadyShown())
    {
        //Get the size of the screen we can see
        CCSize visibleSize = Screen::getVisibleSize();
        //Get the screen start of cordinates
        CCPoint origin = Screen::getOrigin();
        float scaled = Screen::getScaleFactor();

        _poll = CCSprite::create(Language::localizeFileName("translation_poll.png").c_str());
        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(this,
                                                               Language::localizeFileName("yes_no_menu.plist").c_str(),
                                                               Language::localizeFileName("yes_no_menu.png").c_str());
        //Create menu items
        //Yes button
        _poll_yes = AnimatedMenuItem::create(
                    menu_spl->loadSprite("yes.png"),
                    this, menu_selector(MainMenu::pollYes));
        _poll_yes->setOpacity(0);
        _poll_yes->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.6f),
                                                              CCFadeTo::create(0.3f, 255)));

        //Settings button
        _poll_no = AnimatedMenuItem::create(
                    menu_spl->loadSprite("no.png"),
                    this, menu_selector(MainMenu::pollNo));

        _poll_no->setOpacity(0);
        _poll_no->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.6f),
                                                              CCFadeTo::create(0.3f, 255)));

        _poll->setOpacity(0);
        _poll->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.6f),
                                                              CCFadeTo::create(0.3f, 255)));

        //Create menu
        _poll_menu = MenuSpriteBatch::create(menu_spl);
        _poll_menu->setPosition(ccp(visibleSize.width + origin.x - 200/scaled, origin.y+50/scaled));
        _poll->setPosition(ccp(visibleSize.width + origin.x - 200/scaled, origin.y+110/scaled));
        _poll_menu->menu()->addChild(_poll_yes);
        _poll_menu->menu()->addChild(_poll_no);
        _poll_menu->menu()->alignItemsHorizontallyWithPadding(30/scaled);
        _poll_menu->setScale(0.7f);
        this->addChild(_poll_menu);
        this->addChild(_poll);
    }*/
}

void MainMenu::doCloseGame()
{
    RW::saveGame();
    MusicSettings::stopMusicForBackgrHolder();
    //MusicSettings::turnOnMusic();
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

