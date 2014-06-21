#include "UnlockWindow.h"
#include "cocos2d-A.h"
#include "Logic/Language.h"
using namespace cocos2d;

void UnlockWindow::onCreate(cocos2d::CCNode *parent)
{
    float scaled = ADScreen::getScaleFactor();

    CCSize size = parent->getContentSize();
    float x_middle = size.width / 2;
    CCSprite* text = CCSprite::create(Language::localizeFileName("select_collection/unlock_title.png").c_str());
    text->setPosition(ccp(x_middle, size.height * 0.76f));
    parent->addChild(text);

    CCSprite* stamp = CCSprite::create("select_collection/unlock_stamp.png");
    stamp->setPosition(ccp(0,size.height*0.48));
    stamp->setAnchorPoint(ccp(1, 0.5));

    CCNode* stamps_number = 0;


    std::stringstream ss;
    ss << "" << _stamp_to_unlock;
    ccColor3B label_color(ccc3(61,63,64));
    stamps_number = createLabel(ss.str(),  "font/mathisfun_digits.plist",
                                "font/mathisfun_digits.png",
                                label_color);

    stamps_number->setAnchorPoint(ccp(0,0.5));
    stamps_number->setPositionY(stamp->getPositionY());

    float stamp_width = stamp->getContentSize().width;
    float stamp_number_width = stamps_number->getContentSize().width;

    float max_width = size.width*0.8;
    float label_width = max_width - stamp_width;

    if(stamp_number_width > label_width)
    {
        float scale = label_width/stamp_number_width;
        stamps_number->setScale(scale);
        stamp_number_width *= scale;
    }

    float center = x_middle;
    float full_label_half = (stamp_width + stamp_number_width) / 2;
    float stamp_position = center - full_label_half + stamp_width;


    stamps_number->setPositionX(stamp_position);
    stamp->setPositionX(stamp_position);

    parent->addChild(stamp);
    parent->addChild(stamps_number);

    CCMenu* menu = CCMenu::create();
    menu->setPosition(ccp(0,0));
    menu->setAnchorPoint(ccp(0,0));
    menu->setContentSize(size);
    parent->addChild(menu);


    CCSprite* unlock_image = CCSprite::create("select_collection/unlock_button.png");
    CCSize image_size = unlock_image->getContentSize();


    CCSprite* play_more_image = CCSprite::create("select_collection/play_more_button.png");
    ADMenuItem *play_more_item = ADMenuItem::create(play_more_image);
    CONNECT(play_more_item->signalOnClick,
            this, &UnlockWindow::onPlayMoreClick);


    float design_scale = 1;
    play_more_item->setPosition(ccp(100*design_scale/scaled+image_size.width/2,
                                    53*design_scale/scaled+image_size.height/2));






    ADMenuItem *next_level = ADMenuItem::create(unlock_image);
    CONNECT(play_more_item->signalOnClick,
            this, &UnlockWindow::onUnlockClick);

    next_level->setPosition(ccp(600*design_scale/scaled,
                                53*design_scale/scaled+image_size.height/2));
    menu->addChild(next_level);

    menu->addChild(play_more_item);

}
