#ifndef BANNER_H
#define BANNER_H
#include "cocos2d-A.h"

namespace ads
{
typedef int BannerID;
class ClickAction : public CCObject
{
public:
    ClickAction();
    virtual ~ClickAction()
    {}

    void click()
    {
        onClick();
    }
private:
    virtual void onClick()=0;
};

typedef std::string BannerType;





class OwnAdItem
{
    //USAGE:
    //OwnAdItem(action).addImage(BANNER_BIG, "big.png").addImage(BANNER_SMALL, "small.png")
public:
    ~OwnAdItem();
    OwnAdItem& operator=(const OwnAdItem&);
    OwnAdItem(const OwnAdItem&);
    OwnAdItem(ClickAction*);
    const std::string getImageName(const BannerType&) const;
    bool hasImage(const BannerType&) const;

    OwnAdItem& addImage(const BannerSize&, const std::string&);
    ClickAction* getAction() const;

    void click() const;

private:
    ClickAction* _action;
    typedef std::map<BannerSize, std::string> BannerSizesMap;
    BannerSizesMap _image_sizes;
};
typedef std::vector<OwnAdItem> OwnAdsItems;



class Banner: public cocos2d::CCNode
{
public:
    void showAds();
    void showAdsLater(float interval);
    void hideAds(bool hide_house=true);

    BannerID getID() const;
    void destroyNativeBanner();
    void onExit();
private:
    static const float ROTATE_OWN_EVERY;
    static const float WAIT_NATIVE;
    static const float PAUSE;
    void onCloseClick(CCObject*);
    void onBannerClick(CCObject*);


    void nativeIsReadyResult(bool is_ready);
    void houseRotate(float);
    void showAdsLaterCallback(float);

    void changeCloseButtonVisibility(bool visible);

    friend class AdsManager;
    bool _is_hidden;
    Banner(const float real_width, const float real_height, const BannerSize&);
    CCMenuItem* _close;
    CCMenuItem* _house_banner;

    CCMenu* _banner_menu;
    const OwnAdItem* _house_banner_info;
    CCSize _real_size;
    BannerSize _device_size;
    bool _house_ads_active;
    bool _is_house_ads_supported;
    float _close_size;
    bool _created;
    BannerID _id;
};

class AdsManager
{
public:
    typedef std::vector<std::string> StringVector;
    static AdsManager& getInstance();
    void addOwnItem(const OwnAdItem&);

    void stopAllAds(bool hide_house=false);
    void stopAllAdsCurrentSeanse();

    void addCloseButton(const std::string& sprite_name, CCObject* obj, SEL_CallFunc method);
    void addSupportedBannerSize(const BannerSize&);

    void setBannerZoneKey(const std::string&);
    const std::string& getBannerZoneKey();
    const float appCordinatesToDeviceCordinates(float);
    const float deviceCordinatesToAppCordinates(float);


    void addTestDevice(const std::string&);
    const StringVector& getTestDevices();

    Banner* getBanner(const CCSize& zone_max_size);
    const std::string& getCloseButtonName();
private:



    AdsManager();
    AdsManager& operator=(const AdsManager&);

    friend class Banner;
    friend class IsBannerShownResult;
    const OwnAdItem* getOwnItem(const BannerSize&);

    void onCloseClick();

    void nativeInit();
    void createNativeBanner(const BannerID, const BannerSize& size, const CCPoint& position);
    BannerID assignNewBannerId();
    void showNativeBanner(const BannerID);
    void hideNativeBanner(const BannerID);
    void destroyNativeBanner(const BannerID);
    float getDeviceDensity();

    void isNativeBannerShown(const BannerID);

    void bannerShownResult(bool is_shown, const BannerID);


    //Delete banner from the list of shown banners
    void removeFromShownList(const BannerID);

    static AdsManager* _instance;

    typedef std::vector<BannerSize> BannerSizeVector;
    typedef std::vector<CCSize> RealBannerSizeVector;
    typedef std::map<BannerID, Banner*> BannersMap;

    BannerSizeVector _supported_banners_sizes;
    RealBannerSizeVector _supported_banners_real_sizes;
    BannersMap _shown_banners;

    OwnAdsItems _own_items;
    std::string _banner_key;

    CCObject* _close_callback_obj;
    SEL_CallFunc _close_callback_method;

    CCSize _close_button_size;
    std::string _close_button_sprite_name;
    StringVector _test_devices_vector;

    unsigned int _last_own_index;

    float _visible_to_frame_size_constant;
    float _density;
    bool _isAds;
};

class UrlClickAction : public ClickAction
{
public:
    UrlClickAction(const std::string& url);
private:
    void onClick();
    std::string _url;
};

}
#endif // BANNER_H
