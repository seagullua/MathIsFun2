#include "UnlockWindow.h"
#include "cocos2d-A.h"
#include "Logic/Language.h"
#include "GameInfo.h"
#include "ADLib/Device/ADLanguage.h"

using namespace cocos2d;

void UnlockWindow::onCreate(cocos2d::CCNode *parent)
{
    float scaled = ADScreen::getScaleFactor();

    CCSize size = parent->getContentSize();
    float x_middle = size.width / 2;
  int size_font =45;
   int size_font_small =30;
    CCLabelTTF* text  = CCLabelTTF::create(_("select_collection.unlock_title"),
                                           ADLanguage::getFontName(),
                                           size_font);
    text->setAnchorPoint(ccp(0.5, 1));
    text->setPosition(ccp(x_middle, size.height * 0.95f));
    text->setColor(GameInfo:: COLOR_DARK);
    parent->addChild(text);

    CCLabelTTF* text_small  = CCLabelTTF::create(_("select_collection.unlock_title_small"),
                                           ADLanguage::getFontName(),
                                           size_font_small);
    text_small->setAnchorPoint(ccp(0.5, 1));
    text_small->setPosition(ccp(x_middle, size.height * 0.68f));
    text_small->setColor(GameInfo:: COLOR_DARK);
    parent->addChild(text_small);
    //CCSprite* text = CCSprite::create(Language::localizeFileName("select_collection/unlock_title.png").c_str());


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


    //CCSprite* unlock_image = CCSprite::create("select_collection/unlock_button.png");
    CCSprite* unlock_image = CCSprite::create("select_collection/button_background.png");
    unlock_image->setColor(GameInfo::COLOR_DARK_GREEN);

    ADMenuItem *unlock_item = ADMenuItem::create(unlock_image);
    CONNECT(unlock_item->signalOnClick,
            this, &UnlockWindow::onPlayMoreClick);




    CCLabelTTF* button_label_unlock_image  = CCLabelTTF::create(_("select_collection.unlock_image"),
                                           ADLanguage::getFontName(),
                                           size_font);
    button_label_unlock_image->setPosition(ccp(unlock_image->getContentSize().width*0.5f,
                                               unlock_image->getContentSize().height*0.68f));
    button_label_unlock_image->setColor(GameInfo::COLOR_DARK_GREEN);

    CCLabelTTF* button_label_unlock_image_price  = CCLabelTTF::create("$0.99",
                                           ADLanguage::getFontName(),
                                           size_font/2);
    button_label_unlock_image_price->setPosition(ccp(unlock_image->getContentSize().width*0.5f,
                                               unlock_image->getContentSize().height*0.3f));
    button_label_unlock_image_price->setColor(GameInfo::COLOR_DARK_GREEN);

    unlock_item->addChild(button_label_unlock_image);
    unlock_item->addChild(button_label_unlock_image_price);

    CCSprite* play_more_image = CCSprite::create("select_collection/button_background.png");
    play_more_image->setColor(GameInfo::COLOR_DARK_BLUE);


    ADMenuItem *play_more_item = ADMenuItem::create(play_more_image);
    CONNECT(play_more_item->signalOnClick,
            this, &UnlockWindow::onPlayMoreClick);

    CCLabelTTF* button_label_play_more_image  = CCLabelTTF::create(_("select_collection.play_more_image"),
                                           ADLanguage::getFontName(),
                                           size_font);
    button_label_play_more_image->setPosition(ccp(play_more_image->getContentSize().width*0.5f,
                                                  play_more_image->getContentSize().height*0.55f));
   button_label_play_more_image->setColor(GameInfo::COLOR_DARK_BLUE);
    play_more_item->addChild(button_label_play_more_image);

 CCSize image_size = unlock_image->getContentSize();
    float design_scale = 1;
    play_more_item->setPosition(ccp(100*design_scale/scaled+image_size.width/2,
                                    53*design_scale/scaled+image_size.height/2));






//    ADMenuItem *next_level = ADMenuItem::create(unlock_image);
//    CONNECT(play_more_item->signalOnClick,
//            this, &UnlockWindow::onUnlockClick);

    unlock_item->setPosition(ccp(250*design_scale/scaled,
                                53*design_scale/scaled+image_size.height/2));
    play_more_item->setPosition(ccp(600*design_scale/scaled,
                                53*design_scale/scaled+image_size.height/2));
    menu->addChild(unlock_item);

    menu->addChild(play_more_item);

}
