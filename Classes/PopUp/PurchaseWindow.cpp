#include "PurchaseWindow.h"
#include "cocos2d-A.h"
#include "Logic/Language.h"
using namespace cocos2d;
void PurchaseWindow::onCreate(CCNode *parent)
{
    float scaled = ADScreen::getScaleFactor();

    CCSize size = parent->getContentSize();
    float x_middle = size.width / 2;
    CCSprite* text = CCSprite::create(Language::localizeFileName("select_collection/buy_title.png").c_str());
    text->setPosition(ccp(x_middle, size.height * 0.65f));
    parent->addChild(text);


    CCMenu* menu = CCMenu::create();
    menu->setPosition(ccp(0,0));
    menu->setAnchorPoint(ccp(0,0));
    menu->setContentSize(size);
    parent->addChild(menu);

    CCSprite* lamp = CCSprite::create("select_collection/zebra.png");
    parent->addChild(lamp);
    lamp->setScale(0.9f);
    lamp->setPosition(ccp(200/scaled, 280/scaled));


    CCSprite* unlock_image = CCSprite::create("select_collection/buy_all_button.png");

    CCSize image_size = unlock_image->getContentSize();
    float design_scale = 1;


    CCSprite* buy_one_image = CCSprite::create("select_collection/buy_one_button.png");
    ADMenuItem *buy_one_item = ADMenuItem::create(buy_one_image);
    CONNECT(buy_one_item->signalOnClick,
            this, &PurchaseWindow::onBuyOneClick);

    buy_one_item->setPosition(ccp(100*design_scale/scaled+image_size.width/2,
                                    53*design_scale/scaled+image_size.height/2));

    ADMenuItem *buy_all_item = ADMenuItem::create(unlock_image);
    CONNECT(buy_one_item->signalOnClick,
            this, &PurchaseWindow::onBuyAllClick);
    buy_all_item->setPosition(ccp(600*design_scale/scaled,
                                53*design_scale/scaled+image_size.height/2));
    menu->addChild(buy_all_item);
    menu->addChild(buy_one_item);

}
