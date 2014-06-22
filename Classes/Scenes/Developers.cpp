#include "Developers.h"
#include "Scenes/Settings.h"
#include "Logic/Language.h"
#include "ADLib/Device/ADLanguage.h"
#include "GameInfo.h"
using namespace cocos2d;

Developers::Developers()
{
}
bool Developers::init()
{
    if ( !SceneStyle::init() )
    {
        return false;
    }

    //To trigger back button
    this->setKeypadEnabled(true);


    //Get the size of the screen we can see
    CCSize VISIBLE_SIZE = ADScreen::getVisibleSize();
    CCPoint ORIGIN = ADScreen::getOrigin();
    float SCALE = ADScreen::getScaleFactor();
    float x_middle_of_sheet = (VISIBLE_SIZE.width-133/SCALE)/2 + ORIGIN.x;


    ////////////////////////////////////////////////////////////////
    //developer window title
    _developers_window_title = CCLabelTTF::create(_("developers_scene.title"),
                                           ADLanguage::getFontName(),
                                           45);
    _developers_window_title->setAnchorPoint(ccp(0.5, 1));
    _developers_window_title->setPosition(ccp(x_middle_of_sheet,
                                         VISIBLE_SIZE.height + ORIGIN.y - 50/SCALE));
    _developers_window_title->setColor(GameInfo::COLOR_DARK_BLUE);
    this->addChild(_developers_window_title);

    //Make it fade in slowly
    _developers_window_title->setOpacity(0);
    CCFadeTo* title_fade_in = CCFadeTo::create(0.6f, 255);
    _developers_window_title->runAction(title_fade_in);

    /////////////////////////////////////////////////////////////////

    //developers
    //TODO:create developers list
    _developers_window_list = CCLabelTTF::create(_("developers.list"),
                                           ADLanguage::getFontName(),
                                           60);
    _developers_window_list->setAnchorPoint(ccp(0.5, 0.5));
    _developers_window_list->setPosition(ccp(x_middle_of_sheet,
                                         VISIBLE_SIZE.height*0.5f + ORIGIN.y));
    _developers_window_list->setColor(GameInfo::COLOR_DARK_GREEN);
    this->addChild(_developers_window_list);

    //Make it fade in slowly
    _developers_window_list->setOpacity(0);
    CCFadeTo* title_fade_in_list = CCFadeTo::create(0.6f, 255);
    _developers_window_list->runAction(title_fade_in_list);

    /////////////////////////////////////////////////////////////////




    //_("developers.list");

    /////////////////////////////////////////////////////////////////

    //4enjoy logo
    CCMenu* menu = CCMenu::create();
    _x4enjoy_item = ADMenuItem::create(CCSprite::create("settings/4enjoy.png"));
    CONNECT(_x4enjoy_item->signalOnClick, this, &Developers::onSite);
    _x4enjoy_item->setAnchorPoint(ccp(0.5,0.5));
    _x4enjoy_item->setPosition(ccp(x_middle_of_sheet,
                                  ORIGIN.y+100/SCALE));

    menu->addChild(_x4enjoy_item);
    menu->setPosition(ccp(0,0));

    _x4enjoy_item->setOpacity(0);
    CCFadeTo* x4enjoy_item_fade_in = CCFadeTo::create(0.6f, 255);
    _x4enjoy_item->runAction(x4enjoy_item_fade_in);

    this->addChild(menu);
    return true;
}
cocos2d::CCScene* Developers::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    Developers *layer = Developers::create();

    // add layer as a child to scene
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(SceneStyle::simulateBackClick));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}
void Developers::onBackClick()
{
    hideEverything([](){
        CCDirector::sharedDirector()->replaceScene(Settings::scene());
    });
}


void Developers::hideEverything(const Action &callback)
{
    float delay = 0.30f;
    //CCFadeTo* move = CCFadeTo::create(delay, 0);

    _x4enjoy_item->runAction(CCFadeTo::create(delay,0));
    _developers_window_title->runAction(CCFadeTo::create(delay, 0));
    //_developers->runAction(CCFadeTo::create(delay, 0));

    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    ADCallFunc::create(callback),
                    NULL));
}

void Developers::onSite()
{

    ADBrowser::openWebURL(GameInfo::SITE_URL);

}
