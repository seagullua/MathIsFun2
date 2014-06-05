#include "LevelKeyboard.h"
//#include <algorithm>
LevelKeyboard::LevelKeyboard(const CCSize &zone, PopUpWindowManager &pop_up_m) : _buttons(BUTTONS_COUNT), _mode(None),
    _listener(0), _zone(zone), _layout_mode(Horizontal), _pop_up_window_manager(pop_up_m)
{
    this->setContentSize(zone);
    this->setCascadeOpacityEnabled(true);
    //Detect widescreen and change the layout of keyboards
    if(_zone.width/_zone.height < 1.2)
    {
        _layout_mode = Vertical;
    }
    _col_spl = GraphicsManager::getLoaderFor(
                0,
                "level/keyboard.plist",
                "level/keyboard.png");

    //Create menu with buttons
    _collections_menu = MenuSpriteBatch::create(_col_spl);

    _pop_up_window_manager.addMenuToAutoDisable(_collections_menu->menu());

    //Create menu items
    _buttons[b0] = createButton("b0.png");
    _buttons[b1] = createButton("b1.png");
    _buttons[b2] = createButton("b2.png");
    _buttons[b3] = createButton("b3.png");
    _buttons[b4] = createButton("b4.png");
    _buttons[b5] = createButton("b5.png");
    _buttons[b6] = createButton("b6.png");
    _buttons[b7] = createButton("b7.png");
    _buttons[b8] = createButton("b8.png");
    _buttons[b9] = createButton("b9.png");
    _buttons[add] = createButton("add.png");
    _buttons[substract] = createButton("substract.png");
    _buttons[multiply] = createButton("multiply.png");
    _buttons[left_parenthesis] = createButton("left.png");
    _buttons[right_parenthesis] = createButton("right.png");

    _base_button_size = _buttons[b0]->getContentSize();


    //Add all buttons to the menu
    for(unsigned int i=0; i<BUTTONS_COUNT; ++i)
    {
        _collections_menu->menu()->addChild(_buttons[i]);
        _buttons[i]->setVisible(false);
    }

    this->addChild(_collections_menu);
    _collections_menu->setPosition(0,zone.height);
}
//void LevelKeyboard::setOpacity(GLubyte a)
//{
//    CCNodeWithOpacity::setOpacity(a);
//    _collections_menu->menu()->setOpacity(a);
//}

void LevelKeyboard::sleep()
{
    if(_collections_menu)
        _collections_menu->menu()->setTouchEnabled(false);
}

void LevelKeyboard::wakeup()
{
    if(_collections_menu)
        _collections_menu->menu()->setTouchEnabled(true);
}
CCPoint LevelKeyboard::getButtonPosition(const ButtonsCodes code)
{
    CCPoint a = _collections_menu->getPosition();
    CCPoint b = _buttons[code]->getPosition();
    return ccp(a.x + b.x, a.y + b.y);
}
CCSize LevelKeyboard::getButtonSizeScaled(const ButtonsCodes code)
{
    ADMenuItem* item = _buttons[code];
    float scale = item->getBaseScale();
    CCSize content_size = item->getContentSize();
    return CCSize(content_size.width * scale,
                  content_size.height * scale);
}

ADMenuItem* LevelKeyboard::createButton(const char* name)
{
    //Create menu item
    return ADMenuItem::create(
                _col_spl->loadSprite(name),
                this, menu_selector(LevelKeyboard::onKeyPressed));
}
/**
 * @brief Arranges the given list of item into the giving number of
 * columns to fit the target zone size.
 * If the last row has less items than others the row will be aligment
 * in the middle of given zone.
 * If the item is pointer to zero this item won't be displayed but will be counted
 * ALL ITEMS SHOULD HAVE THE SAME SIZE
 * @param items
 * @param columns
 */
void LevelKeyboard::arrangeButtons(const ItemsArr& items,
                                   const unsigned int &columns)
{

    //Calculate the number of rows and columns
    unsigned int items_number = items.size();
    unsigned int rows = items_number / columns;
    if(items_number % columns != 0)
        rows++;

    //Get the size of the buttons and spaces in the units
    const float base_spacing = 10;
    const float spacing_in_units = 1;
    const float button_width_in_units = _base_button_size.width / base_spacing;
    const float button_height_in_units = _base_button_size.height / base_spacing;

    //Calculate the maximum unit size to fit buttons in the zone
    float horizontal_units = columns * button_width_in_units + (columns-1) * spacing_in_units;
    float vertical_units = rows * button_height_in_units + (rows-1) * spacing_in_units;

    float pixels_in_unit = MIN(_zone.width/horizontal_units, _zone.height/vertical_units);

    //Calculate the real width and heiht of all buttons
    float width = horizontal_units * pixels_in_unit;
    float height = vertical_units * pixels_in_unit;

    float spacing_size = spacing_in_units * pixels_in_unit;

    //Get the scale factor
    float scale_factor = spacing_size / base_spacing;

    float horizontal_disp = 0;
    float vertical_disp = 0;

    //Put items one by one
    for(unsigned int y=0; y<rows; ++y)
    {
        if(y == (rows-1) && (items_number % columns != 0))
        {
            //Last row which has not enough buttons
            unsigned int in_last_row = items_number % columns;
            float last_row_width = in_last_row * button_width_in_units +
                    (columns-1) * spacing_in_units;
            last_row_width*=pixels_in_unit;

            horizontal_disp = (width - last_row_width) / 2;
        }
        else
            horizontal_disp = 0;

        for(unsigned int x=0; x<columns; ++x)
        {
            unsigned int index = y*columns+x;
            if(index < items_number)
            {
                ADMenuItem* item = items[y*columns+x];
                if(item)
                {
                    item->setVisible(true);
                    item->setBaseScale(scale_factor);
                    item->setPosition(ccp(horizontal_disp, -vertical_disp));
                }
                horizontal_disp += (button_width_in_units + spacing_in_units)
                        * pixels_in_unit;
            }
        }
        vertical_disp += (button_height_in_units + spacing_in_units)
                * pixels_in_unit;
    }

    _collections_menu->setPosition((_zone.width-width+
                                    button_width_in_units*pixels_in_unit)/2,
                                   (_zone.height+height-
                                    button_height_in_units*pixels_in_unit)/2);

}

void LevelKeyboard::onKeyPressed(CCObject* pSender)
{
    if(_listener)
    {
        unsigned int found_i = _buttons.size();
        for(unsigned int i=0; i<found_i; ++i)
        {
            if(_buttons[i] == pSender)
            {
                found_i = i;
            }
        }
        if(found_i < _buttons.size())
        {
            _listener->onKeyPressed(ButtonsCodes(found_i));
        }
    }
}


void LevelKeyboard::rearangeButtons(const Mode& mask)
{
    unsigned int new_mask = mask;

    //Hide all buttons
    for(unsigned int i=0; i<BUTTONS_COUNT; ++i)
    {
        _buttons[i]->setVisible(false);
    }


    _mode = mask;
    if((new_mask & Digits) != 0)
    {
        //Digits and parenthesis
        //Digits and parenthesis and operations
        //Digits
        //Digits and operations
        bool operations = (new_mask & Operations) != 0;
        bool parenthesis = (new_mask & Parenthesises) != 0;
        bool zero = (new_mask & Zero) != 0;
        ItemsArr to_arrange(0);
        to_arrange.reserve(BUTTONS_COUNT);
        if(operations && _layout_mode == Vertical)
        {
            to_arrange.push_back(_buttons[add]);
            to_arrange.push_back(_buttons[substract]);
            to_arrange.push_back(_buttons[multiply]);
        }
        to_arrange.push_back(_buttons[b1]);
        to_arrange.push_back(_buttons[b2]);
        to_arrange.push_back(_buttons[b3]);
        if(operations && _layout_mode == Horizontal)
            to_arrange.push_back(_buttons[add]);
        to_arrange.push_back(_buttons[b4]);
        to_arrange.push_back(_buttons[b5]);
        to_arrange.push_back(_buttons[b6]);
        if(operations && _layout_mode == Horizontal)
            to_arrange.push_back(_buttons[substract]);
        to_arrange.push_back(_buttons[b7]);
        to_arrange.push_back(_buttons[b8]);
        to_arrange.push_back(_buttons[b9]);
        if(operations && _layout_mode == Horizontal)
            to_arrange.push_back(_buttons[multiply]);
        if(parenthesis)
        {
            to_arrange.push_back(_buttons[left_parenthesis]);
            if(zero)
                to_arrange.push_back(_buttons[b0]);
            to_arrange.push_back(_buttons[right_parenthesis]);
            if(zero && operations && _layout_mode==Horizontal)
                to_arrange.push_back(0);
        }
        else if(zero)
        {
            if(operations)
            {
                to_arrange.push_back(0);
                to_arrange.push_back(_buttons[b0]);
                to_arrange.push_back(0);
                if(_layout_mode==Horizontal)
                    to_arrange.push_back(0);
            }
            else
            {
                to_arrange.push_back(0);
                to_arrange.push_back(_buttons[b0]);
                to_arrange.push_back(0);
            }
        }


        if(operations && _layout_mode==Horizontal)
            arrangeButtons(to_arrange,4);
        else
            arrangeButtons(to_arrange,3);

    }
    else if((new_mask & Operations) != 0)
    {

        //Operations and parenthesis
        if((new_mask & Parenthesises) != 0)
        {
            ItemsArr to_arrange(0);
            to_arrange.reserve(5);
            if(_layout_mode == Vertical)
            {
                to_arrange.push_back(_buttons[left_parenthesis]);
                to_arrange.push_back(_buttons[right_parenthesis]);
            }
            to_arrange.push_back(_buttons[add]);
            to_arrange.push_back(_buttons[substract]);
            to_arrange.push_back(_buttons[multiply]);
            if(_layout_mode == Horizontal)
            {
                to_arrange.push_back(_buttons[left_parenthesis]);
                to_arrange.push_back(_buttons[right_parenthesis]);
                arrangeButtons(to_arrange,3);
            }
            else
                arrangeButtons(to_arrange,2);



        }
        else
        {
            //Operations
            ItemsArr to_arrange(0);
            to_arrange.reserve(3);
            to_arrange.push_back(_buttons[add]);
            to_arrange.push_back(_buttons[substract]);
            to_arrange.push_back(_buttons[multiply]);
            arrangeButtons(to_arrange,2);
        }


    }
    else if((new_mask & Parenthesises) != 0)
    {
        //Only parenthesis
        ItemsArr to_arrange(0);
        to_arrange.reserve(2);
        to_arrange.push_back(_buttons[left_parenthesis]);
        to_arrange.push_back(_buttons[right_parenthesis]);
        arrangeButtons(to_arrange,2);
    }

}

LevelKeyboard* LevelKeyboard::create(const CCSize& zone, PopUpWindowManager &pop_up_m)
{
    LevelKeyboard *pRet = new LevelKeyboard(zone,pop_up_m);
    if (pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}
