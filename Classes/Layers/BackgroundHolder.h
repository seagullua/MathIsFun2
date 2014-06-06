#ifndef BACKGROUNDLAYER_H
#define BACKGROUNDLAYER_H
#include <cocos2d.h>
#include <ADLib/Rendering/ADMenuItem.h>
#include "Core/MenuSpriteBatch.h"



class BackgroundHolder
{
public:
    static void backgroundSwitchTo(cocos2d::CCNode*,
                                   cocos2d::CCCallFunc* back_button_callback=0,
                                   const bool play_music=true);
    static void storeOpen();
    static void storeClose();
    static void resetMusic();
private:
    static BackgroundHolder& getHolder();
    BackgroundHolder();
    ~BackgroundHolder();
    void do_backgroundSwitchTo(cocos2d::CCNode*,
                               cocos2d::CCCallFunc* back_button_callback,
                               const bool play_music);
    class BackgroundLayer : public cocos2d::CCLayer
    {
    public:
        BackgroundLayer();
        ~BackgroundLayer();
        virtual bool init();
        void setBackButtonCallback(cocos2d::CCCallFunc* back_button_callback);

        CREATE_FUNC(BackgroundLayer)
    private:
        void onBackClicked();
        void triggerAnimations();
        cocos2d::CCCallFunc* _back_button_callback;
        ADMenuItem* _back_button;
        cocos2d::CCPoint _back_button_position;
        cocos2d::CCSprite* _animated_sprite;
        cocos2d::CCMenu* _back_button_menu;
        friend class BackgroundHolder;
    };
    BackgroundLayer* _background_layer;
    bool _store_opened;
    static bool _firstly;

};

#endif // BACKGROUNDLAYER_H
