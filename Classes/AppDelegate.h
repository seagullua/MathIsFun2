#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "PurchaseHandler.h"
#ifdef CC_WIN8_METRO
#include "CCApplication.h"

#endif
/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();
#ifdef CC_WIN8_METRO
	virtual bool initInstance();
	virtual void applicationViewStateChanged(int newState, int oldState){};
#endif
    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    class AdsOffListener : public cocos2d::CCObject
    {
    public:
        void wasClickedDisableAllAds();
    };
#ifndef JUNIOR
    static PurchaseHandler* getPurchaseHandler();
private:
    static PurchaseHandler _purchase_handler;

#endif
private:
    bool _loaded;
};

#endif // _APP_DELEGATE_H_

