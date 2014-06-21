#include "YesNoDialog.h"
#include "Logic/Language.h"
using namespace cocos2d;
YesNoDialog::YesNoDialog(CCNode* content,
            CCObject* yes_callback,
            SEL_CallFunc yes_fun,
            CCObject* no_callback,
            SEL_CallFunc no_fun)
    : _content(content), _yes_obj(yes_callback), _yes_fun(yes_fun),
      _no_obj(no_callback), _no_fun(no_fun)
{
    _content->retain();
}


void YesNoDialog::onCreate(CCNode *parent)
{
    CCSize size = parent->getContentSize();
    float scaled = ADScreen::getScaleFactor();

    parent->addChild(_content);
    _content->release();

    float x_middle = size.width / 2;

    _content->setPosition(ccp(x_middle, size.height*0.65));


    //Create menu items
    //Yes button
    ADMenuItem *yes_button = ADMenuItem::create(
                CCSprite::create("yes.png"));
    CONNECT(yes_button->signalOnClick,
            this, &YesNoDialog::yes);


    //Settings button
    ADMenuItem *no_button = ADMenuItem::create(
                CCSprite::create("no.png"));
    CONNECT(no_button->signalOnClick,
            this, &YesNoDialog::no);

    //Create menu
    CCMenu* menu = CCMenu::create();
    menu->setPosition(ccp(x_middle, size.height*0.3));
    menu->addChild(yes_button);
    menu->addChild(no_button);
    menu->alignItemsHorizontallyWithPadding(120/scaled);
    parent->addChild(menu);
}

void YesNoDialog::yes()
{
    if(_yes_obj && _yes_fun)
    {
        (_yes_obj->*_yes_fun)();
    }
    closeWindow();
}

void YesNoDialog::no()
{
    if(_no_obj && _no_fun)
    {
        (_no_obj->*_no_fun)();
    }
    closeWindow();
}
