#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H
#include "cocos2d.h"
#include <ADLib/PopUp/ADPopUpWindow.h>

class PopUpWindowStyle : public ADPopUpWindow::Content
{
protected:
    PopUpWindowStyle();
private:
    virtual void initDesing(cocos2d::CCNode* window_node);
    virtual float moveInAnimation(cocos2d::CCNode* window_node);
    virtual float moveOutAnimation(cocos2d::CCNode* window_node);
    cocos2d::CCPoint _sheet_target_position;
};

#endif // POPUPWINDOW_H
