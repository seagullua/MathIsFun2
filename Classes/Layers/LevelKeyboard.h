#ifndef LEVELKEYBOARD_H
#define LEVELKEYBOARD_H
#include "cocos2d-A.h"
#include "Layers/PopUpWindow.h"

class LevelKeyboard : public CCNodeWithOpacity
{
    LevelKeyboard(const CCSize& zone, PopUpWindowManager& pop_up_m);
public:


    enum Mode
    {
        None=0,
        Digits=1,

        Zero=2,
        DigitsAndZero=3,

        Operations=4,
        DigitsAndOperations=5,
        ZeroAndOperations=6,
        DigitsZeroAndOperations=7,
        AllExceptParenthesises=7,

        Parenthesises=8,
        DigitsAndParenthesises=9,
        ZeroAndParenthesises=10,
        DigitsZeroAndParenthesises=11,
        OperationsAndParenthesises=12,
        DigitsOperationsAndParenthesises=13,
        ZeroOperationsAndParenthesises=14,
        DigitsZeroOperationsAndParenthesises=15,
        All=15

    };
    enum ButtonsCodes
    {
        b1,b2,b3,add,
        b4,b5,b6,substract,
        b7,b8,b9,multiply,
        left_parenthesis, b0, right_parenthesis,

        BUTTONS_COUNT
    };

    CCPoint getButtonPosition(const ButtonsCodes code);
    CCSize getButtonSizeScaled(const ButtonsCodes code);

    void setMode(const Mode& mask)
    {
        if(mask != _mode)
            rearangeButtons(mask);
    }

    void sleep();
    void wakeup();
    static LevelKeyboard* create(const CCSize& zone, PopUpWindowManager& pop_up_m);

    class KeyboardDelegate : public CCObject
    {
    public:
        virtual void onKeyPressed(const ButtonsCodes&)=0;
    };
    void setListener(KeyboardDelegate* listener)
    {
        if(_listener)
            _listener->release();
        _listener = listener;
        if(_listener)
            _listener->retain();
    }

    void setOpacity(GLubyte a);
private:
    enum LayoutMode
    {
        Horizontal,
        Vertical
    };

    typedef std::vector<AnimatedMenuItem*> ItemsArr;
    void rearangeButtons(const Mode& mask);
    void arrangeButtons(const ItemsArr&, const unsigned int& in_row);
    AnimatedMenuItem* createButton(const char* name);
    Mode _mode;
    KeyboardDelegate* _listener;
    std::vector<AnimatedMenuItem*> _buttons;
    MenuSpriteBatch* _collections_menu;
    MenuSpriteBatch* _menu;
    SpritesLoader _col_spl;
    CCSize _zone;
    CCSize _base_button_size;
    LayoutMode _layout_mode;
    PopUpWindowManager& _pop_up_window_manager;
    void onKeyPressed(CCObject* pSender);

    /*void hideZero();
    void showZero();
    void hideDigits();
    void showDigits();
    void hideOperations();
    void showOperations();
    void hideParenthesises();
    void showParenthesises();*/
};

#endif // LEVELKEYBOARD_H
