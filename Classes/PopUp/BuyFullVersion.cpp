#include "BuyFullVersion.h"
#include "cocos2d-A.h"
#include "GameInfo.h"
using namespace cocos2d;

void BuyFullVersion::onCreate(CCNode *parent)
{
    float scaled = ADScreen::getScaleFactor();
    CCSize size = parent->getContentSize();
    float x_middle = size.width / 2;

    //create window title
    CCLabelTTF* window_title = CCLabelTTF::create(_("buy_full_version.title"),
                                                  ADLanguage::getFontName(),
                                                  55);
    window_title->setColor(GameInfo::COLOR_LIGHT_BLUE);
    window_title->setPosition(ccp(x_middle, size.height * 0.85f));
    parent->addChild(window_title);


    //create menu for buttons
    CCMenu* menu = CCMenu::create();
    menu->setPosition(ccp(0,0));
    menu->setAnchorPoint(ccp(0,0));
    menu->setContentSize(size);
    parent->addChild(menu);

    //create zebra image
    CCSprite* zebra = CCSprite::create("select_collection/zebra.png");
    parent->addChild(zebra);
    zebra->setScale(0.9f);
    zebra->setPosition(ccp(200/scaled, 280/scaled));


    CCSprite* button1 = CCSprite::create("select_collection/background2.png");

    //////////////////////////////////////////////////

    //buy now
    CCSprite* button2 = CCSprite::create("select_collection/background1.png");
    CCLabelTTF* buy_now_title = CCLabelTTF::create(_("pop_up.buy_full_version.button.buy_now"),
                                                   ADLanguage::getFontName(),
                                                   45);
    buy_now_title->setColor(GameInfo::COLOR_DARK_GREEN);

    ADMenuItem *buy_now_item = ADMenuItem::create(button2);
    buy_now_item->addChild(buy_now_title);
    CONNECT(buy_now_item->signalOnClick,
            this, &BuyFullVersion::onBuyFullVersionClick);


    buy_now_title->setPosition(buy_now_item->getContentSize()*0.5);
    buy_now_item->setPosition(ccp(parent->getContentSize().width*0.3,
                              parent->getContentSize().height*0.2));


    ///////////////////////////////////////////////////////////////

    //buy later
    ADMenuItem *buy_later_item = ADMenuItem::create(button1);
    CCLabelTTF* buy_later_title = CCLabelTTF::create(_("pop_up.buy_full_version.button.later"),
                                                   ADLanguage::getFontName(),
                                                   45);
    buy_later_title->setColor(GameInfo::COLOR_RED);
    CONNECT(buy_later_item->signalOnClick,
            this, &BuyFullVersion::onBuyLaterClick);

    buy_later_item->setPosition(ccp(parent->getContentSize().width*0.75,
                                    parent->getContentSize().height*0.2));
    buy_later_item->addChild(buy_later_title);
    buy_later_title->setPosition(buy_later_item->getContentSize()*0.5);

    ///////////////////////////////////////////////////////////////

    menu->addChild(buy_now_item);
    menu->addChild(buy_later_item);

}
