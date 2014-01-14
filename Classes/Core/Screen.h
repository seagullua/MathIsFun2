#ifndef SCREEN_H
#define SCREEN_H
#include "cocos2d.h"

class Screen
{
public:
    static float getScaleFactor();
    static cocos2d::CCPoint getOrigin();
    static cocos2d::CCSize getVisibleSize();

    static void setDesignScale(float scale);
private:
    static float _design_scale;
};

#endif // SCREEN_H
