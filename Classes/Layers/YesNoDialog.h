#ifndef YESNODIALOG_H
#define YESNODIALOG_H
#include "PopUpWindow.h"
#include "cocos2d-A.h"
class YesNoDialog : public PopUpWindowStyle
{
public:
    YesNoDialog(cocos2d::CCNode* content,
                CCObject* yes_callback,
                cocos2d::SEL_CallFunc yes_fun,
                CCObject* no_callback=0,
                cocos2d::SEL_CallFunc no_fun=0);

    YesNoDialog(cocos2d::CCLabelTTF* title,
                CCObject* yes_callback,
                cocos2d::SEL_CallFunc yes_fun,
                CCObject* no_callback=0,
                cocos2d::SEL_CallFunc no_fun=0);
private:
    void onCreate(cocos2d::CCNode *parent);
    void yes();
    void no();
    cocos2d::CCNode* _content;
    cocos2d::CCLabelTTF* _title;

    CCObject* _yes_obj;
    cocos2d::SEL_CallFunc _yes_fun;
    CCObject* _no_obj;
    cocos2d::SEL_CallFunc _no_fun;
};

#endif // YESNODIALOG_H
