//#include "Ads.h"
//#include "RealLog.h"
//#include "Browser.h"
//namespace ads
//{

//ClickAction::ClickAction()
//{}

//BannerSize::BannerSize(const unsigned int width, const unsigned int height)
//    : _width(width), _height(height)
//{
//}

//const unsigned int BannerSize::getWidth() const
//{
//    return _width;
//}
//const unsigned int BannerSize::getHeight() const
//{
//    return _height;
//}

//const BannerSize BannerSize::BANNER_BIG(468, 60);
//const BannerSize BannerSize::BANNER_SMALL(320, 50);

//OwnAdItem::~OwnAdItem()
//{
//    if(_action)
//        _action->release();
//}

//OwnAdItem& OwnAdItem::operator=(const OwnAdItem& item)
//{
//    if(this != &item)
//    {
//        if(_action != 0)
//            _action->release();

//        _action = item._action;

//        if(_action != 0)
//            _action->retain();

//        _image_sizes = item._image_sizes;
//    }
//    return *this;
//}

//OwnAdItem::OwnAdItem(const OwnAdItem& item)
//    : _action(item._action), _image_sizes(item._image_sizes)
//{
//    if(_action)
//        _action->retain();
//}

//OwnAdItem::OwnAdItem(ClickAction* action)
//    : _action(action)
//{
//    if(_action)
//        _action->retain();
//}

//const std::string OwnAdItem::getImageName(const BannerSize& size) const
//{
//    BannerSizesMap::const_iterator it = _image_sizes.find(size);
//    if(it == _image_sizes.end())
//        return "";
//    return it->second;
//}
//bool OwnAdItem::hasImage(const BannerSize& size) const
//{
//    return _image_sizes.find(size) != _image_sizes.end();
//}

//OwnAdItem& OwnAdItem::addImage(const BannerSize& size, const std::string& file)
//{
//    _image_sizes[size] = file;
//    return *this;
//}

//ClickAction* OwnAdItem::getAction() const
//{
//    return _action;
//}
//void OwnAdItem::click() const
//{
//    if(_action)
//        _action->click();
//}
//AdsManager::AdsManager()
//    : _close_button_size(0,0), _last_own_index(0), _density(0), _isAds(true)
//{
//    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
//    CCSize visibleSize = Screen::getVisibleSize();
//    CCSize frameSize = pEGLView->getFrameSize();

//    nativeInit();
//    _density = getDeviceDensity();
//    _visible_to_frame_size_constant = float(frameSize.width) / visibleSize.width;

//    //realLog("Device size %dx%d", int(frameSize.width), int(frameSize.height));
//    //realLog("Visible size %dx%d", int(visibleSize.width), int(visibleSize.height));
//}

//AdsManager& AdsManager::getInstance()
//{
//    if(_instance == 0)
//    {
//        _instance = new AdsManager;
//    }
//    return *_instance;
//}
//void AdsManager::addOwnItem(const OwnAdItem& item)
//{
//    _own_items.push_back(item);
//}

//void AdsManager::stopAllAds(bool hide_house)
//{
//    std::vector<Banner*> bns;
//    bns.reserve(_shown_banners.size());
//    for(BannersMap::iterator it=_shown_banners.begin();
//        it != _shown_banners.end(); ++it)
//    {
//        bns.push_back(it->second);
//    }

//    for(unsigned int i=0; i<bns.size(); ++i)
//    {
//        Banner* b = bns[i];
//        b->hideAds(hide_house);
//        b->destroyNativeBanner();
//    }
//}

//void AdsManager::stopAllAdsCurrentSeanse()
//{
//    _isAds=false;
//    stopAllAds(true);
//}

//void AdsManager::addCloseButton(const std::string& sprite_name, CCObject* obj, SEL_CallFunc method)
//{
//    _close_callback_obj = obj;
//    _close_callback_method = method;
//    _close_button_sprite_name = sprite_name;
//    CCSprite* close_button = CCSprite::create(sprite_name.c_str());
//    _close_button_size = close_button->getContentSize();
//}

//void AdsManager::setBannerZoneKey(const std::string& key)
//{
//    _banner_key = key;
//}
//const std::string& AdsManager::getBannerZoneKey()
//{
//    return _banner_key;
//}

//void AdsManager::addTestDevice(const std::string& id)
//{
//    _test_devices_vector.push_back(id);
//}

//const AdsManager::StringVector& AdsManager::getTestDevices()
//{
//    return _test_devices_vector;
//}

//void AdsManager::addSupportedBannerSize(const BannerSize& size)
//{
//    _supported_banners_sizes.push_back(size);
//    CCSize real_size(deviceCordinatesToAppCordinates(size.getWidth()*_density),
//                     deviceCordinatesToAppCordinates(size.getHeight()*_density));
//    _supported_banners_real_sizes.push_back(real_size);

////    realLog("Added banner %dx%d Real Size %dx%d", size.getWidth(), size.getHeight(),
////            int(real_size.width), int(real_size.height));
//}

//void AdsManager::bannerShownResult(bool is_shown, const BannerID id)
//{
//    BannersMap::iterator it = _shown_banners.find(id);
//    if(it != _shown_banners.end())
//    {
//        Banner* b = it->second;
//        b->nativeIsReadyResult(is_shown);
//    }
//}

//Banner* AdsManager::getBanner(const CCSize& zone_max_size)
//{
//    if (_isAds==false)
//        return 0;
//    unsigned int banners_number = _supported_banners_real_sizes.size();
//    unsigned int max_banner_i = banners_number;
//    float max_banner_area = 0;

//    for(unsigned int i=0; i < banners_number; ++i)
//    {
//        CCSize real_size = _supported_banners_real_sizes[i];
//        float area = real_size.width * real_size.height;


//        if(area > max_banner_area)
//        {
//            //real_size.width += _close_button_size.width;
//            if(zone_max_size.width >= real_size.width &&
//                    zone_max_size.height >= real_size.height)
//            {
//                max_banner_i = i;
//                max_banner_area = area;
//            }
//        }
//    }

//    if(max_banner_i < banners_number)
//    {
//        CCSize real_size = _supported_banners_real_sizes[max_banner_i];
//        BannerSize size = _supported_banners_sizes[max_banner_i];
//        //We have banner to put in place
//        Banner* banner = new Banner(real_size.width, real_size.height, size);
//        if(banner)
//        {
//            banner->autorelease();
//        }
//        banner->retain();
//        _shown_banners[banner->getID()] = banner;
//        return banner;
//    }
//    return 0;
//}

//const OwnAdItem *AdsManager::getOwnItem(const BannerSize& size)
//{
//    if(_own_items.size() == 0)
//        return 0;

//    if(_own_items.size() == 1)
//    {
//        if(_own_items[0].hasImage(size))
//            return &_own_items[0];
//    }


//    unsigned int first = _last_own_index;


//    do
//    {
//        _last_own_index = (_last_own_index+1)%_own_items.size();
//    } while(_last_own_index != first &&  !_own_items[_last_own_index].hasImage(size));

//    if(_last_own_index != first)
//        return &_own_items[_last_own_index];

//    return 0;
//}


//const float AdsManager::appCordinatesToDeviceCordinates(float x)
//{
//    return x * _visible_to_frame_size_constant;
//}

//const float AdsManager::deviceCordinatesToAppCordinates(float x)
//{
//    return x / _visible_to_frame_size_constant;
//}

//void AdsManager::onCloseClick()
//{
//    CCLog("Ads close clicked!");
//    if(_close_callback_method != 0 && _close_callback_obj != 0)
//    {
//        (_close_callback_obj->*_close_callback_method)();
//    }
//}
//void AdsManager::removeFromShownList(const BannerID id)
//{
//    BannersMap::iterator it = _shown_banners.find(id);
//    if(it != _shown_banners.end())
//    {
//        Banner* b = it->second;
//        b->release();
//        _shown_banners.erase(it);
//    }
//}

//AdsManager* AdsManager::_instance=0;

//const std::string& AdsManager::getCloseButtonName()
//{
//    return _close_button_sprite_name;
//}

//void Banner::onBannerClick(CCObject*)
//{
//    if(_house_banner_info)
//        _house_banner_info->click();
//}
//void Banner::changeCloseButtonVisibility(bool visible)
//{
//    if(_close)
//    {
//        _close->setEnabled(visible);
//        _close->setVisible(visible);
//    }
//}
//const float Banner::ROTATE_OWN_EVERY=300;
//const float Banner::WAIT_NATIVE=3;
//const float Banner::PAUSE=10;
//void Banner::nativeIsReadyResult(bool is_ready)
//{
//    float next_update = 1000;

//    /*//If out banner is active
//    if(_house_ads_active)
//    {
//        //We will hide our banner
//        if(_house_banner)
//            _house_banner->setVisible(false);

//        _house_ads_active = false;
//        changeCloseButtonVisibility(false);

//        //We will wait 2 seconds and will try to show provider banner
//        next_update = WAIT_NATIVE;
//        AdsManager::getInstance().showNativeBanner(_id);
//    }
//    else
//    {*/
//        //If there are no out banner we check if the provider banner is ready
//    if(is_ready)
//    {
//        changeCloseButtonVisibility(true);
//        if(_house_ads_active)
//        {
//            if(_house_banner)
//            {
//                _house_banner->setVisible(false);
//                _house_banner->removeFromParent();
//            }
//            _house_banner = 0;
//            _house_banner_info = 0;
//            _house_ads_active = false;
//        }
//    }
//    else
//    {
//        //If no banner is ready we will check if we have our banners
//        if(_is_house_ads_supported)
//        {
//            //Obtain banner
//            const OwnAdItem* new_info = AdsManager::getInstance().getOwnItem(_device_size);

//            //If there are no banners then show nothing
//            if(new_info == 0)
//            {
//                _is_house_ads_supported = false;
//                next_update = PAUSE;

//            }
//            else
//            {
//                if(new_info != _house_banner_info)
//                {
//                    _house_banner_info = new_info;
//                    if(_house_banner)
//                    {
//                        _house_banner->removeFromParent();
//                        _house_banner = 0;
//                    }

//                    _house_ads_active = true;
//                    std::string name = _house_banner_info->getImageName(_device_size);
//                    realLog(name.c_str());
//                    CCSprite* image = CCSprite::create(name.c_str());
//                    _house_banner = CCMenuItemSprite::create(
//                                image, image,
//                                this, menu_selector(Banner::onBannerClick));
//                    changeCloseButtonVisibility(true);
//                    next_update = ROTATE_OWN_EVERY;

//                    float banner_width = image->getContentSize().width;
//                    float banner_height = image->getContentSize().height;
//                    float zone_width = _real_size.width;
//                    float zone_height = _real_size.height;

//                    float scale = zone_width / banner_width;
//                    float scale_height = zone_height / banner_height;
//                    _house_banner->setScale(scale);

//                    _house_banner->setPosition(ccp(0,0));
//                    _house_banner->setAnchorPoint(ccp(0,0));
//                    _banner_menu->addChild(_house_banner);

//                    //AdsManager::getInstance().hideNativeBanner(_id);
//                }
//            }
//        }
//        else
//        {
//            next_update = PAUSE;

//        }
//    }

//    this->schedule(schedule_selector(Banner::houseRotate), next_update);
//}

//void Banner::houseRotate(float)
//{
//    if(!_is_hidden)
//        AdsManager::getInstance().isNativeBannerShown(_id);


//}

//Banner::Banner(const float real_width, const float real_height, const BannerSize& banner_size)
//    : _real_size(real_width, real_height),
//      _device_size(banner_size),
//      _close(0), _id(0), _house_banner(0),
//      _house_ads_active(false), _is_house_ads_supported(true),
//      _banner_menu(0), _close_size(0), _created(false), _is_hidden(true)
//{
//    std::string close_button_name=AdsManager::getInstance().getCloseButtonName();
//    CCSize my_size(_real_size);

//    CCMenu* menu = CCMenu::create(NULL);
//    menu->setPosition(ccp(0,0));
//    this->addChild(menu);

//    _banner_menu = menu;

//    _id = AdsManager::getInstance().assignNewBannerId();

//    if (close_button_name!="")
//    {
//        CCSprite* close_button = CCSprite::create(close_button_name.c_str());
//        _close = CCMenuItemSprite::create(close_button,
//                                          close_button,
//                                          this,
//                                          menu_selector(Banner::onCloseClick));
//        _close_size = close_button->getContentSize().width;
//        //my_size.width += _close_size;


//        _banner_menu->addChild(_close);
//        //menu->setVisible();


//        _close->setAnchorPoint(ccp(1,1));
//        _close->setPosition(my_size.width+_close_size, my_size.height);
//        _close->setVisible(false);

//        changeCloseButtonVisibility(false);

////        CCSprite* test = CCSprite::create(close_button_name.c_str());
////        test->setAnchorPoint(ccp(1,1));
////        test->setPosition(ccp(my_size.width, my_size.height));
////        this->addChild(test);
//    }
//    this->setContentSize(my_size);
//}

//void Banner::showAds()
//{
//    AdsManager& manager = AdsManager::getInstance();

//    _is_hidden = false;
//    if(!_created)
//    {
//        _created = true;


//        CCPoint position = this->getPosition();
//        CCSize size = this->getContentSize();
//        CCPoint anchor = this->getAnchorPoint();

//        CCPoint origin = Screen::getOrigin();
//        CCSize frameSize = CCEGLView::sharedOpenGLView()->getFrameSize();


//        position.x -= size.width * anchor.x + origin.x;
//        position.y += size.height * (1 - anchor.y) - origin.y;

//        CCPoint screen_point(manager.appCordinatesToDeviceCordinates(position.x),
//                             frameSize.height - manager.appCordinatesToDeviceCordinates(position.y));

//        manager.createNativeBanner(_id, _device_size, screen_point);
//    }
//    else
//    {
//        manager.showNativeBanner(_id);
//    }
//    this->unscheduleAllSelectors();
//    this->schedule(schedule_selector(Banner::houseRotate), WAIT_NATIVE);
//}
//void Banner::showAdsLater(float interval)
//{
//    this->schedule(schedule_selector(Banner::showAdsLaterCallback), interval, 1, 0);
//}

//void Banner::showAdsLaterCallback(float)
//{
//    this->unschedule(schedule_selector(Banner::showAdsLaterCallback));
//    showAds();
//}

//void Banner::hideAds(bool hide_house)
//{
//    if(hide_house)
//    {
//        this->unscheduleAllSelectors();
//        _is_hidden = true;
//        if(_close)
//            _close->setVisible(false);

//        if(_house_banner)
//        {
//            _house_banner->setVisible(false);
//            _house_banner->removeFromParent();
//        }
//        _house_banner = 0;
//        _house_banner_info = 0;

//    }
//    //realLog("Banner::hideAds %d", _id);
//    if(_id != 0)
//        AdsManager::getInstance().hideNativeBanner(_id);
//}
//void Banner::destroyNativeBanner()
//{
//    if(_close)
//        _close->setVisible(false);
//    AdsManager::getInstance().destroyNativeBanner(_id);
//    AdsManager::getInstance().removeFromShownList(_id);
//    this->unscheduleAllSelectors();
//}

//void Banner::onExit()
//{
//    CCNode::onExit();
//    destroyNativeBanner();
//}
//BannerID Banner::getID() const
//{
//    return _id;
//}


//void Banner::onCloseClick(CCObject*)
//{
//    AdsManager::getInstance().onCloseClick();
//}

//UrlClickAction::UrlClickAction(const std::string& url)
//    : _url(url)
//{

//}

//void UrlClickAction::onClick()
//{
//    Browser::open(_url);
//}

//}

//namespace std
//{
//bool operator<(const ads::BannerSize& a, const ads::BannerSize& b)
//{
//    return (a.getWidth() < b.getWidth() ||
//            (a.getWidth() == b.getWidth() && a.getHeight() < b.getHeight()));
//}

//}
