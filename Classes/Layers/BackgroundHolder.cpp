#include "BackgroundHolder.h"
#include <ADLib/Device/ADSoundManager.h>
#include <ADLib/Device.h>
#include "GameInfo.h"
#include "Logic/SavesManager.h"
using namespace cocos2d;
bool BackgroundHolder::_firstly=true;

BackgroundHolder::BackgroundHolder()
    : _background_layer(BackgroundLayer::create()), _store_opened(false)
{
    if(_background_layer)
        _background_layer->retain();
}
void BackgroundHolder::resetMusic()
{
    _firstly = true;
}

BackgroundHolder::~BackgroundHolder()
{
    if(_background_layer)
        _background_layer->release();
}

BackgroundHolder& BackgroundHolder::getHolder()
{
    static BackgroundHolder holder;
    return holder;
}
void BackgroundHolder::backgroundSwitchTo(cocos2d::CCNode* node,
                                          cocos2d::CCCallFunc* back_button_callback,
                                          const bool play_music)
{
    BackgroundHolder& bgh = getHolder();
    bgh.storeClose();
    bgh.do_backgroundSwitchTo(node, back_button_callback, play_music);
}

void BackgroundHolder::do_backgroundSwitchTo(cocos2d::CCNode* node,
                                             cocos2d::CCCallFunc* back_button_callback,
                                             const bool play_music)
{
    //turn on the audio if we just open the game
    if (play_music && SavesManager::getInstance()->isMusicOn())
    {
        ADSoundManager::turnOnMusic();
        ADSoundManager::playMusic(GameInfo::BACKGROUND_MUSIC);
    }
    else
    {
        ADSoundManager::turnOffMusic();
    }

    //If we has layer
    if(_background_layer)
    {
        _background_layer->removeFromParentAndCleanup(false);

        node->addChild(_background_layer);
        _background_layer->setBackButtonCallback(back_button_callback);

    }
}

BackgroundHolder::BackgroundLayer::BackgroundLayer()
    : _back_button_callback(0),
      _back_button_position(),
      _back_button(0),
      _back_button_menu(0)
{

}
BackgroundHolder::BackgroundLayer::~BackgroundLayer()
{

}
void BackgroundHolder::storeClose()
{

}

void BackgroundHolder::storeOpen()
{

}

void BackgroundHolder::BackgroundLayer::triggerAnimations()
{
    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();
    CCPoint origin = ADScreen::getOrigin();
    float scaled = ADScreen::getScaleFactor();

    SpritesLoader spl = GraphicsManager::getLoaderFor(
                this,
                "animations.plist",
                "animations.png");

    _animated_sprite = spl->loadSprite("cloud_0.png");

    _animated_sprite->setPosition(ccp(origin.x+visibleSize.width/10*9+10/scaled,
                                      origin.y+visibleSize.height/7*6));

    CCArray* animFrames = CCArray::createWithCapacity(6);

    char str[100] = {0};
    for(int i = 1; i < 7; ++i)
    {
        sprintf(str, "cloud_%d.png", i);
        CCSpriteFrame* frame = spl->loadSpriteFrame(str);
        animFrames->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(
                animFrames,0.1f);

    CCRepeatForever* repeater = CCRepeatForever::create(CCAnimate::create(animation));
    _animated_sprite->runAction(repeater);
    //CCSpriteBatchNode* spriteBatchNode = CCSpriteBatchNode::create("numbers.png");
    //spriteBatchNode->addChild(sprite);
    //addChild(spriteBatchNode);
}

bool BackgroundHolder::BackgroundLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = ADScreen::getOrigin();
    float scaled = ADScreen::getScaleFactor();

    //Place background
    CCSprite* pSprite = CCSprite::create("background/sheet-background.jpg");
    CCPoint sp_origin = pSprite->boundingBox().origin;
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x,
                             origin.y + visibleSize.height + sp_origin.y));
    this->addChild(pSprite);


    auto back_action = [this](){
        this->onBackClicked();
    };

    _back_button = ADMenuItem::create(
                CCSprite::create("back_button.png"),
                back_action);

    _back_button_menu = CCMenu::create();
    _back_button_menu->addChild(_back_button);


    _back_button_position = _back_button->getPosition();
    //_back_button->setPosition(ccp(0, visibleSize.height));
    //_back_button->setCascadeOpacityEnabled(true);
    _back_button->setOpacity(0);

    this->addChild(_back_button_menu);
    //CCSize bb_size = _back_button->boundingBox().size;
    _back_button_menu->setPosition(ccp(origin.x +120/scaled,
                                       origin.y + visibleSize.height -100/scaled ));

    //triggerAnimations();
    return true;
}
void BackgroundHolder::BackgroundLayer::onBackClicked()
{
    if(_back_button_callback)
        _back_button_callback->execute();
    //if(_back_button_callback)
    //    this->runAction(_back_button_callback);
}

void BackgroundHolder::BackgroundLayer::setBackButtonCallback(cocos2d::CCCallFunc* callback)
{
    bool should_show = false;
    bool should_hide = false;

    if(callback == 0)
    {
        if(_back_button_callback)
            should_hide = true;
    }
    else
    {
        if(_back_button_callback == 0)
            should_show = true;
    }
    if(_back_button_callback)
        _back_button_callback->release();

    _back_button_callback = callback;

    if(_back_button_callback)
        _back_button_callback->retain();

    //Get the size of the screen we can see
    //CCSize visibleSize = ADScreen::getVisibleSize();
    if(should_show)
    {
        CCFadeTo* move = CCFadeTo::create(0.3f, 255);
        //_back_button->setOpacity(0);
        _back_button->stopAllActions();
        _back_button->runAction(move);
        _back_button_menu->setTouchEnabled(true);
    }
    else if(should_hide)
    {
        CCFadeTo* move = CCFadeTo::create(0.3f, 0);
        _back_button->stopAllActions();
        _back_button->runAction(move);
        _back_button_menu->setTouchEnabled(false);
    }
}
