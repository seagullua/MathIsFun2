#include "MainMenu.h"
#include "AppMacros.h"
USING_NS_CC;
#include "SelectCollection.h"
#include "LevelScene.h"
#include "Settings.h"
#include "SimpleAudioEngine.h"
#include "Logic/Language.h"
#include "Logic/RW.h"

MainMenu::MainMenu()
    :
      _main_menu(0),
      _logo(0)
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
    if ( !SceneStyle::init() )
    {
        return false;
    }

    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = ADScreen::getOrigin();
    float scaled = ADScreen::getScaleFactor();


    //Place logo
    _logo = CCSprite::create("main_menu/logo.png");
    _logo->setScale(0.77f);
    _logo->setAnchorPoint(ccp(0.5, 1));

    float x_middle_of_sheet = (visibleSize.width-133/scaled)/2 + origin.x;
    CCPoint logo_target_position = ccp(x_middle_of_sheet,
                                       visibleSize.height + origin.y - 80/scaled);
    _logo->setPosition(ccp(x_middle_of_sheet,
                           2*visibleSize.height));
    this->addChild(_logo);

    CCMoveTo* logo_move = CCMoveTo::create(0.3f, logo_target_position);
    _logo->runAction(logo_move);

    //Create menu items
    //Play button
    _play = ADMenuItem::create(CCSprite::create("main_menu/play.png"));
    CONNECT(_play->signalOnClick,
            this, &MainMenu::onPlayPressed);

    //Settings button
    _settings = ADMenuItem::create(CCSprite::create("main_menu/settings.png"));
    CONNECT(_settings->signalOnClick,
            this, &MainMenu::onSettingsPressed);


    float right_x = visibleSize.width + origin.x - 122/scaled;
    float up_y = visibleSize.height + origin.y - 107/scaled;

    _share_menu = CCMenu::create();

    _facebook_button = ADMenuItem::create(CCSprite::create("main_menu/facebook_logo.png"));
    CONNECT(_facebook_button->signalOnClick,
            this,&MainMenu::onShareFacebookPressed);

    _facebook_button->setAnchorPoint(ccp(0.5,0.5));
    _facebook_button->setOpacity(0);
    CCFadeTo* facebook_move = CCFadeTo::create(0.5f, 255);
    _facebook_button->runAction
            (CCSequence::create(CCDelayTime::create(0.3f), facebook_move, NULL));

    _share_menu->setPosition(ccp(right_x,up_y));

    _share_menu->addChild(_facebook_button);
    this->addChild(_share_menu);

    //Create menu
    _main_menu = CCMenu::create();
    _main_menu->addChild(_play);
    _main_menu->addChild(_settings);
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

    _pop_up_manager.addMenuToAutoDisable(_main_menu);
    _pop_up_manager.addMenuToAutoDisable(_share_menu);



    return true;
}
void MainMenu::hideEverything(const Action& callback)
{

    CCFadeTo* settings_move = CCFadeTo::create(0.15f, 0);
    CCFadeTo* play_move = CCFadeTo::create(0.15f, 0);
    CCFadeTo* logo_move = CCFadeTo::create(0.15f, 0);
    _settings->runAction(settings_move);
    _play->runAction(CCSequence::create(CCDelayTime::create(0.15f), play_move, NULL));
    _logo->runAction(CCSequence::create(CCDelayTime::create(0.3f),
                                        logo_move,
                                        ADCallFunc::create(callback), NULL));

    CCFadeTo* facebook_move = CCFadeTo::create(0.15f, 0);
    _facebook_button->runAction
            (CCSequence::create(CCDelayTime::create(0.15f), facebook_move, NULL));



}


void MainMenu::onBackClick()
{

    CCSprite* label = CCSprite::create("quite_promt.png");

    _pop_up_manager.openWindow(new YesNoDialog(label,
                                               this, callfunc_selector(MainMenu::doCloseGame)));

}

void MainMenu::onShareFacebookPressed()
{
    ADBrowser::openWebURL(GameInfo::FACEBOOK_URL);
}

//Action when user presses on Play
void MainMenu::onPlayPressed()
{
    hideEverything([](){
        CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
    });
}


//Action when user presses Settings
void MainMenu::onSettingsPressed()
{
    hideEverything([](){
        CCDirector::sharedDirector()->replaceScene(Settings::scene());
    });
}

void MainMenu::doCloseGame()
{
    RW::saveGame();
    ADDeviceEvents::closeApp();
}

