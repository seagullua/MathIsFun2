#include "RateGame.h"
#include "cocos2d-A.h"
#include "GameInfo.h"
using namespace cocos2d;

void RateGame::onCreate(CCNode *parent)
{

    CCSize size = parent->getContentSize();
    float x_middle = size.width / 2;

    //create window title
    CCLabelTTF* window_title = CCLabelTTF::create(_("rate_game.title"),
                                                  ADLanguage::getFontName(),
                                                  55);
    window_title->setColor(GameInfo::COLOR_LIGHT_BLUE);
    window_title->setPosition(ccp(x_middle, size.height * 0.85f));
    parent->addChild(window_title);

    /////////////////////////////////////////////////////////

    //add full version description
    CCLabelTTF* description = CCLabelTTF::create(_("rate_game.description"),
                                                  ADLanguage::getFontName(),
                                                  40);
    description->setColor(GameInfo::COLOR_LIGHT_GRAY);
    description->setPosition(ccp(x_middle,
                                 size.height * 0.55f));
    parent->addChild(description);

    ////////////////////////////////////////////////////

    //create menu for buttons
    CCMenu* menu = CCMenu::create();
    menu->setPosition(ccp(0,0));
    menu->setAnchorPoint(ccp(0,0));
    menu->setContentSize(size);
    parent->addChild(menu);

    CCSprite* button1 = CCSprite::create("select_collection/background2.png");

    //////////////////////////////////////////////////

    //buy now
    CCSprite* button2 = CCSprite::create("select_collection/background1.png");
    CCLabelTTF* rate_now_title = CCLabelTTF::create(_("pop_up.rate.button.rate_now"),
                                                   ADLanguage::getFontName(),
                                                   45);
    rate_now_title->setColor(GameInfo::COLOR_DARK_GREEN);

    ADMenuItem *rate_now_item = ADMenuItem::create(button2);
    rate_now_item->addChild(rate_now_title);
    CONNECT(rate_now_item->signalOnClick,
            this, &RateGame::onRateNowClick);


    rate_now_title->setPosition(rate_now_item->getContentSize()*0.5);
    rate_now_item->setPosition(ccp(parent->getContentSize().width*0.3,
                              parent->getContentSize().height*0.2));


    ///////////////////////////////////////////////////////////////

    //buy later
    ADMenuItem *rate_later_item = ADMenuItem::create(button1);
    CCLabelTTF* rate_later_title = CCLabelTTF::create(_("pop_up.rate.button.later"),
                                                   ADLanguage::getFontName(),
                                                   45);
    rate_later_title->setColor(GameInfo::COLOR_RED);
    CONNECT(rate_later_item->signalOnClick,
            this, &RateGame::onRateLaterClick);

    rate_later_item->setPosition(ccp(parent->getContentSize().width*0.75,
                                    parent->getContentSize().height*0.2));
    rate_later_item->addChild(rate_later_title);
    rate_later_title->setPosition(rate_later_item->getContentSize()*0.5);

    ///////////////////////////////////////////////////////////////

    menu->addChild(rate_now_item);
    menu->addChild(rate_later_item);

}
