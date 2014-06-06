#include "Developers.h"
#include "Scenes/Settings.h"
#include "Logic/Language.h"
using namespace cocos2d;

Developers::Developers():_menu_name(0),_developers(0), _version(0)
{
}
bool Developers::init()
{
    if ( !CCLayer::init() )
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

    _menu_name = CCSprite::create(Language::localizeFileName("settings/developers_menu_name.png").c_str());
    this->addChild(_menu_name);
    _menu_name->setAnchorPoint(ccp(0.5, 1));
    CCPoint logo_target_position_1 = ccp(x_middle_of_sheet,
                                       visibleSize.height + origin.y - 50/scaled);
    _menu_name->setPosition(logo_target_position_1);

    //Make it fade in slowly
    _menu_name->setOpacity(0);
    CCFadeTo* logo_fade_in = CCFadeTo::create(0.6f, 255);
    _menu_name->runAction(logo_fade_in);

    CCPoint logo_target_position_2 = ccp(x_middle_of_sheet,
                                       visibleSize.height/2 + origin.y);



     _developers = CCSprite::create("settings/Diana_Andriy_Developers.png");
     this->addChild(_developers);
    //Put this label at the top of the screen
    _developers->setAnchorPoint(ccp(0.5, 0.5));

    _developers->setPosition(logo_target_position_2);
    _developers->setScale(0.75f);

    //Make it fade in slowly
    _developers->setOpacity(0);
    CCFadeTo* developers_fade_in = CCFadeTo::create(0.6f, 255);
    _developers->runAction(developers_fade_in);




    CCMenu* menu = CCMenu::create();
    float posY = origin.y + 150/scaled;
    _x4enjoy_item = ADMenuItem::create(
                             CCSprite::create("settings/4enjoy.png"));
    CONNECT(_x4enjoy_item->signalOnClick, this, &Developers::onSite);

    _send_item = ADMenuItem::create(
                             CCSprite::create("settings/button_send_email.png"));
    CONNECT(_send_item->signalOnClick, this, &Developers::onSendLetter);


    _send_item->setAnchorPoint(ccp(0.5,0.5));
    _send_item->setPosition(ccp(_developers->getPositionX(),posY));


    _x4enjoy_item->setAnchorPoint(ccp(0.5,0.5));
    _x4enjoy_item->setPosition(ccp(_developers->getPositionX(),
                                  origin.y + 50/scaled));

    menu->addChild(_x4enjoy_item);

    menu->addChild(_send_item);


    _version = CCSprite::create("settings/version.png");
    _version->setAnchorPoint(ccp(0, 0));
    CCPoint version_target_position = ccp(origin.x + 50/scaled,
                                       origin.y);
    _version->setPosition(version_target_position);
    this->addChild(_version);


    //Make it fade in slowly
    _version->setOpacity(0);
    CCFadeTo* version_fade_in = CCFadeTo::create(0.6f, 255);
    _version->runAction(version_fade_in);


    _send_item->setOpacity(0);
    CCFadeTo* send_item_fade_in = CCFadeTo::create(0.6f, 255);
    _send_item->runAction(send_item_fade_in);


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
    _version->runAction(CCFadeTo::create(delay,0));

    _send_item->runAction(CCFadeTo::create(delay,0));

    _x4enjoy_item->runAction(CCFadeTo::create(delay,0));
    _menu_name->runAction(CCFadeTo::create(delay, 0));
    _developers->runAction(CCFadeTo::create(delay, 0));

    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    ADCallFunc::create(callback),
                    NULL));
}
void Developers::onSendLetter()
{
    ADBrowser::sendMail("feedback@4enjoy.com", "Math Is Fun Feedback");
    //Browser::openURL(Browser::sendEmail);
}

void Developers::onSite()
{

    ADBrowser::openWebURL(GameInfo::SITE_URL);

}
