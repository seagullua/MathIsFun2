#include "SelectCollection.h"
#include "MainMenu.h"
#include "Logic/RW.h"
#include "SelectLevel.h"
#include "Logic/Language.h"
#include "SelectCollection_UnlockAnimator.h"
#include "Store.h"
#include "Core/Statistics.h"
#include "Core/Ads.h"

class SelectCollection::UnlockWindow : public PopUpWindow::Content
{

public:
    UnlockWindow(const unsigned int stamp_to_unlock)
        : _stamp_to_unlock(stamp_to_unlock)
    {}
private:
    void onPlayMoreClick(CCObject*)
    {
        closeWindow();
    }
    void onUnlockClick(CCObject*)
    {
        Store::buyItem(Store::ITEM_UNLOCK_ALL);
    }



    void onCreate(CCNode *parent)
    {
        float scaled = Screen::getScaleFactor();

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;
        CCSprite* text = CCSprite::create(Language::localizeFileName("select_collection/unlock_title.png").c_str());
        text->setPosition(ccp(x_middle, size.height * 0.76f));
        parent->addChild(text);

        CCSprite* stamp = CCSprite::create("select_collection/unlock_stamp.png");
        stamp->setPosition(ccp(0,size.height*0.48));
        stamp->setAnchorPoint(ccp(1, 0.5));

        CCNode* stamps_number = 0;


        std::stringstream ss;
        ss << "" << _stamp_to_unlock;
        ccColor3B label_color(ccc3(61,63,64));
        stamps_number = createLabel(ss.str(),  "font/mathisfun_digits.plist",
                                    "font/mathisfun_digits.png",
                                    label_color);

        stamps_number->setAnchorPoint(ccp(0,0.5));
        stamps_number->setPositionY(stamp->getPositionY());

        float stamp_width = stamp->getContentSize().width;
        float stamp_number_width = stamps_number->getContentSize().width;

        float max_width = size.width*0.8;
        float label_width = max_width - stamp_width;

        if(stamp_number_width > label_width)
        {
            float scale = label_width/stamp_number_width;
            stamps_number->setScale(scale);
            stamp_number_width *= scale;
        }

        float center = x_middle;
        float full_label_half = (stamp_width + stamp_number_width) / 2;
        float stamp_position = center - full_label_half + stamp_width;


        stamps_number->setPositionX(stamp_position);
        stamp->setPositionX(stamp_position);

        parent->addChild(stamp);
        parent->addChild(stamps_number);

        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                               Language::localizeFileName("select_collection/unlock_buttons.plist").c_str(),
                                                               Language::localizeFileName("select_collection/unlock_buttons.png").c_str());
        MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
        menu->setPosition(ccp(0,0));
        menu->setAnchorPoint(ccp(0,0));
        menu->setContentSize(size);
        parent->addChild(menu);

#ifndef JUNIOR
        CCSprite* unlock_image = menu_spl->loadSprite("unlock_button.png");
        CCSize image_size = unlock_image->getContentSize();
#endif


        CCSprite* play_more_image = menu_spl->loadSprite("play_more_button.png");
        AnimatedMenuItem *play_more_item = AnimatedMenuItem::create(
                    play_more_image,
                    this, menu_selector(UnlockWindow::onPlayMoreClick));


#ifndef JUNIOR
        float design_scale = 1;
        play_more_item->setPosition(ccp(100*design_scale/scaled+image_size.width/2,
                                        53*design_scale/scaled+image_size.height/2));
#endif


#ifdef JUNIOR
        play_more_item->setPosition(ccp(400/scaled,
                                        120/scaled));
#endif

#ifndef JUNIOR
        AnimatedMenuItem *next_level = AnimatedMenuItem::create(
                    unlock_image,
                    this, menu_selector(UnlockWindow::onUnlockClick));
        next_level->setPosition(ccp(600*design_scale/scaled,
                                    53*design_scale/scaled+image_size.height/2));
        menu->menu()->addChild(next_level);
#endif
        menu->menu()->addChild(play_more_item);

    }
    unsigned int _stamp_to_unlock;
};


class SelectCollection::PurchaseWindow : public PopUpWindow::Content
{

public:
    PurchaseWindow(Collection* collection)
        : _collection(collection)
    {}
private:
    void onBuyOneClick(CCObject*)
    {
        std::stringstream ss;
        ss << Store::PREFIX_BUY_COLLECTION << _collection->getCollectionID();
        bool res = Store::buyItem(ss.str());
        if(!res)
        {
            CCLog(("Buy collection failed: "+ss.str()).c_str());
        }
    }
    void onBuyAllClick(CCObject*)
    {
        Store::buyItem(Store::ITEM_BUY_ALL);
    }



    void onCreate(CCNode *parent)
    {
        float scaled = Screen::getScaleFactor();

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;
        CCSprite* text = CCSprite::create(Language::localizeFileName("select_collection/buy_title.png").c_str());
        text->setPosition(ccp(x_middle, size.height * 0.65f));
        parent->addChild(text);


        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                               Language::localizeFileName("select_collection/buy_buttons.plist").c_str(),
                                                               Language::localizeFileName("select_collection/buy_buttons.png").c_str());
        MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
        menu->setPosition(ccp(0,0));
        menu->setAnchorPoint(ccp(0,0));
        menu->setContentSize(size);
        parent->addChild(menu);

        CCSprite* lamp = CCSprite::create("select_collection/zebra.png");
        parent->addChild(lamp);
        lamp->setScale(0.9f);
        lamp->setPosition(ccp(200/scaled, 280/scaled));


        CCSprite* unlock_image = menu_spl->loadSprite("buy_all_button.png");

        CCSize image_size = unlock_image->getContentSize();
        float design_scale = 1;


        CCSprite* buy_one_image = menu_spl->loadSprite("buy_one_button.png");
        AnimatedMenuItem *buy_one_item = AnimatedMenuItem::create(
                    buy_one_image,
                    this, menu_selector(PurchaseWindow::onBuyOneClick));

        buy_one_item->setPosition(ccp(100*design_scale/scaled+image_size.width/2,
                                        53*design_scale/scaled+image_size.height/2));

        AnimatedMenuItem *buy_all_item = AnimatedMenuItem::create(
                    unlock_image,
                    this, menu_selector(PurchaseWindow::onBuyAllClick));
        buy_all_item->setPosition(ccp(600*design_scale/scaled,
                                    53*design_scale/scaled+image_size.height/2));
        menu->menu()->addChild(buy_all_item);
        menu->menu()->addChild(buy_one_item);

    }
    Collection* _collection;
};

SelectCollection::SelectCollection(const Mode mode)
    : _title_select_collection(0), _last_selected_collection(0), _mode(mode), _pop_up_manager(this), _bottom_banner(0)
{
}

CCScene* SelectCollection::scene(const Mode mode)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SelectCollection *layer = SelectCollection::create(mode);

    // add layer as a child to scene
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(SelectCollection::onKeyBackClicked));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}

SelectCollection* SelectCollection::create(const SelectCollection::Mode mode)
{
    SelectCollection *pRet = new SelectCollection(mode);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = 0;
        return 0;
    }
}


SelectCollection* SelectCollection::_last_scene_ptr = 0;
void SelectCollection::keyBackClicked()
{
    if(!_pop_up_manager.backAction())
    {
        hideEverything(
                    CCCallFunc::create(
                        this,
                        callfunc_selector(SelectCollection::doGoBack)));
    }
}
void SelectCollection::doGoBack()
{
    if(_mode == Shop)
        CCDirector::sharedDirector()->replaceScene(SelectCollection::scene(Collections));
    else
        CCDirector::sharedDirector()->replaceScene(MainMenu::scene());
}
void SelectCollection::doOpenShop()
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene(Shop));
}
void SelectCollection::doReload()
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene(_mode));
}

void SelectCollection::doOpenCollection()
{
    if(_last_selected_collection)
    {
        CCDirector::sharedDirector()->replaceScene(
                    SelectLevel::scene(_last_selected_collection));
    }
}

void SelectCollection::doOpenCollections()
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene(Collections));
}

void SelectCollection::onCollectionSelect(CCObject* sender)
{
    AnimatedMenuItem* tile = dynamic_cast<AnimatedMenuItem*>(sender);

    TileMap::iterator it = _tiles.find(tile);
    if(it != _tiles.end())
    {
        CollectionTile selected = it->second;

        if(!selected.isShopTile())
        {
            //We selected this collection
            Collection* current = selected.getCollection();

            if(current->getCollectionState()==Collection::Locked)
            {
                //                //The collection locked;
                unsigned int more_stamps_needed = current->stampsToUnlock() -
                        RW::allStampsObtained();
                //                more_stamps_needed;
                //                CCLOG("More stamps needed");
                //                //TODO: show the window with number of stamps needed
                _pop_up_manager.openWindow(new UnlockWindow(more_stamps_needed));
            }
            else if(current->getCollectionState() == Collection::InShop)
            {
                if(_mode == Shop)
                {
                    _pop_up_manager.openWindow(new PurchaseWindow(current));
                }
                else
                {
                    CCLOG("Something wrong with collection");
                }
            }
            else
            {
                _last_selected_collection = current;
                hideEverything(
                            CCCallFunc::create(
                                this,
                                callfunc_selector(SelectCollection::doOpenCollection)));
            }
        }
        else
        {
            hideEverything(
                        CCCallFunc::create(
                            this,
                            callfunc_selector(SelectCollection::doOpenShop)));
        }
    }

}

AnimatedMenuItem* SelectCollection::createCollectionItem(const std::string& image_name,
                                                         const std::string& label_name,
                                                         const std::string& stamps_label_text,
                                                         const ccColor3B& color,
                                                         const SpritesLoader& papers_spl, const std::string &stamp_file_name, bool show_crown, bool show_new, bool more_level_tile)
{
    AnimatedMenuItem* paper_item = AnimatedMenuItem::create(
                papers_spl->loadSprite("collection_background.png"),
                this,
                menu_selector(SelectCollection::onCollectionSelect));


    //create new collection
    SpritesLoader new_col_spl = GraphicsManager::getLoaderFor(
                paper_item,
                "select_collection/new_collection.plist",
                "select_collection/new_collection.png");
    new_col_spl->inject();
    CCSprite* paper = new_col_spl->loadSprite("collection_background.png");
    CCSize paper_size = paper->getContentSize();
    paper->removeFromParent();

    CCSprite* image = CCSprite::create(image_name.c_str());
    image->setAnchorPoint(ccp(0.5f,0.5f));
    float middle_width = paper_size.width/2;
    image->setPosition(ccp(middle_width, paper_size.height*0.75));
    paper_item->addChild(image);

    float scaled = Screen::getScaleFactor();
    if(show_crown)
    {
        CCSprite* crown = new_col_spl->loadSprite("collection_crown.png");

        crown->setPosition(ccp(paper_size.width-14/scaled, paper_size.height-21/scaled));
    }
    if(show_new)
    {
        CCSprite* new_label = new_col_spl->loadSprite("collection_new.png");
        new_label->setPosition(ccp(paper_size.width-47/scaled, paper_size.height-48/scaled));

    }

    CCSprite* line = new_col_spl->loadSprite("collection_line.png");
    line->setAnchorPoint(ccp(0.5f, 0.5f));
    line->setColor(color);
    line->setPosition(ccp(middle_width,paper_size.height*0.4));

    CCSprite* label = CCSprite::create(label_name.c_str());
    label->setPosition(line->getPosition());
    paper_item->addChild(label);

    if(!more_level_tile)
    {
        CCSprite* stamp = new_col_spl->loadSprite(stamp_file_name.c_str());
        stamp->setPosition(ccp(0,paper_size.height*0.16));
        stamp->setAnchorPoint(ccp(1, 0.5));

        CCNode* stamps_number = 0;

        if(_mode == Collections)
        {
            ccColor3B label_color(ccc3(61,63,64));
            stamps_number = createLabel(stamps_label_text,  "font/mathisfun_digits.plist",
                                        "font/mathisfun_digits.png",
                                        label_color);
        }
        else
        {
            stamps_number = CCSprite::create(stamps_label_text.c_str());
        }
        stamps_number->setAnchorPoint(ccp(0,0.5));
        stamps_number->setPositionY(stamp->getPositionY());

        float stamp_width = stamp->getContentSize().width;
        float stamp_number_width = stamps_number->getContentSize().width;

        float max_width = paper_size.width*0.8;
        float label_width = max_width - stamp_width;

        //if(stamp_number_width > label_width)
        {
            float scale = MIN(label_width/stamp_number_width, 0.7f);
            stamps_number->setScale(scale);
            stamp_number_width *= scale;
        }

        float center = paper_size.width * 0.5f;
        float full_label_half = (stamp_width + stamp_number_width) / 2;
        float stamp_position = center - full_label_half + stamp_width;


        stamps_number->setPositionX(stamp_position);
        stamp->setPositionX(stamp_position);

        paper_item->addChild(stamps_number);
    }
    else
    {
        CCSprite* stamp = new_col_spl->loadSprite("more_levels_subimage.png");
        stamp->setPosition(ccp(paper_size.width * 0.5,paper_size.height*0.16));
        stamp->setAnchorPoint(ccp(0.5, 0.5));

    }




    return paper_item;
}

AnimatedMenuItem* SelectCollection::createCollectionItem(Collection* collection, const SpritesLoader& spl)
{
    std::stringstream stamps_label;
    std::stringstream image_name;
    std::stringstream label_name;

    ccColor3B color;
    bool show_crown = false;
    bool show_new = false;
    stamps_label << " ";
    std::string stamps_label_str;
    if(collection->getCollectionState() == Collection::Locked)
    {

        int to_unlock = collection->stampsToUnlock();
        int overal_stamps = RW::allStampsObtained();

        int stamps_needed = to_unlock - overal_stamps;
        if(stamps_needed < 0)
            stamps_needed = 0;

        stamps_label << stamps_needed;
        image_name << "select_collection/coll_lock.png";
        label_name << "select_collection/clabel_locked.png";
        color = ccc3(221,221,221);
        stamps_label_str = stamps_label.str();
    }
    else
    {
        if(collection->getCollectionState() != Collection::InShop)
        {
            show_crown = collection->stampsObtained() >= collection->stampsMax();
            show_new = collection->stampsObtained() == 0 && collection->getCollectionID() != 100;
            stamps_label << collection->stampsObtained() << "/" << collection->stampsMax();
            stamps_label_str = stamps_label.str();
        }
        else
        {
            Collection::Difficulty diff = collection->getDifficulty();
            if(diff == Collection::Easy)
                stamps_label_str = "select_collection/clabel_easy.png";
            else if(diff == Collection::Medium)
                stamps_label_str = "select_collection/clabel_medium.png";
            else if(diff == Collection::Hard)
                stamps_label_str = "select_collection/clabel_hard.png";
            else//(diff == Collection::UltraHard)
                stamps_label_str = "select_collection/clabel_ultrahard.png";
            stamps_label_str = Language::localizeFileName(stamps_label_str);
        }
        image_name << "select_collection/coll_"<< collection->getCollectionID() <<".png";
        label_name << "select_collection/clabel_"<< collection->getCollectionID() <<".png";
        color = collection->getColor();
    }

    std::string stamp_file_name;

    Collection::Difficulty diff = collection->getDifficulty();
    if(diff == Collection::Easy)
        stamp_file_name = "difstamp_easy.png";
    else if(diff == Collection::Medium)
        stamp_file_name = "difstamp_medium.png";
    else if(diff == Collection::Hard)
        stamp_file_name = "difstamp_hard.png";
    else if(diff == Collection::UltraHard)
        stamp_file_name = "difstamp_ultrahard.png";

    return createCollectionItem(image_name.str(),
                                Language::localizeFileName(label_name.str()),
                                stamps_label_str,
                                color,spl, stamp_file_name, show_crown, show_new, false);
}

bool SelectCollection::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    _last_scene_ptr = this;

    //To trigger back button
    this->setKeypadEnabled(true);

    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();

    //Get the screen start of cordinates
    CCPoint origin = Screen::getOrigin();
    float scaled = Screen::getScaleFactor();
    //Get the sprites loader
    SpritesLoader spl;

    if(_mode == Collections)
    {
//        spl = GraphicsManager::getLoaderFor(
//                    this,
//                    Language::localizeFileName("select_collection/images.plist").c_str(),
//                    Language::localizeFileName("select_collection/images.png").c_str());
    }
    else
    {

        ADStatistics::logEvent("OnMoreLevelsClick");
//        spl = GraphicsManager::getLoaderFor(
//                    this,
//                    Language::localizeFileName("select_collection/images2.plist").c_str(),
//                    Language::localizeFileName("select_collection/images2.png").c_str());
    }
    //Get the Select collection label

    if(_mode == Collections)
    {
        _title_select_collection =  CCSprite::create(
                    Language::localizeFileName("select_collection/select_collection.png").c_str());
        this->addChild(_title_select_collection);
    }
    else
    {
        _title_select_collection = CCSprite::create(
                    Language::localizeFileName("select_collection/game_store.png").c_str());
        this->addChild(_title_select_collection);
    }
    //Put this label at the top of the screen
    _title_select_collection->setAnchorPoint(ccp(0.5, 1));
    float x_middle_of_sheet = (visibleSize.width-133/scaled)/2 + origin.x;
    CCPoint logo_target_position = ccp(x_middle_of_sheet,
                                       visibleSize.height + origin.y - 50/scaled);
    _title_select_collection->setPosition(logo_target_position);

    //Make it fade in slowly
    _title_select_collection->setOpacity(0);
    CCFadeTo* logo_fade_in = CCFadeTo::create(0.3f, 255);
    _title_select_collection->runAction(logo_fade_in);


    //Create menu with collections


    _papers_spl = GraphicsManager::getLoaderFor(
                0,
                "select_collection/new_collection.plist",
                "select_collection/new_collection.png");
    _collections_menu = MenuSpriteBatch::create(_papers_spl);


    float padding_left = 330/scaled;
    unsigned int i = 0;

    const CollectionsArr& arr = RW::getCollections();
    bool in_shop = false;
    bool unlocked_one = false;
    for(CollectionsArr::const_iterator it=arr.begin(); it!=arr.end();++it)
    {
        Collection* a = it->second;

        if((a->getCollectionState() != Collection::InShop && _mode == Collections )
                || (a->getCollectionState() == Collection::InShop && _mode == Shop))
        {
            AnimatedMenuItem* paper_item = createCollectionItem(a, _papers_spl);
            _collections_menu->menu()->addChild(paper_item, 100);
            //paper_item->setBaseScale(0.7f);
            paper_item->setPositionX(i * padding_left);
            i++;

            _tiles[paper_item] = CollectionTile(paper_item, a);

            if(a->isReadyToBeUnlocked())
            {

                CollectionUnlockAnimator* anim = new CollectionUnlockAnimator(_tiles[paper_item], this);
                anim->autorelease();
                anim->unlockItem();
                RW::unlockCollection(a);
                unlocked_one = true;
            }
        }
        else
        {
            in_shop = true;
        }
    }

    if(unlocked_one)
    {
        RW::saveGame();
    }
    if(in_shop && _mode == Collections)
    {
        AnimatedMenuItem* paper_item = createCollectionItem(
                    "select_collection/more_levels_image.png",
                    Language::localizeFileName("select_collection/clabel_shop.png"),
                    "",
                    ccc3(246, 145, 255),
                    //ccc3(255,153,237),
                    _papers_spl,
                    "",
                    false,
                    false,
                    true);
        _collections_menu->menu()->addChild(paper_item, 100);
        //paper_item->setBaseScale(0.7f);
        paper_item->setPositionX(i * padding_left);
        i++;

        _tiles[paper_item] = CollectionTile(paper_item, 0, true);
    }

    _pop_up_manager.addMenuToAutoDisable(_collections_menu->menu());
    newScrolling(_collections_menu, i*padding_left + 20/scaled);

    return true;
}


void SelectCollection::newScrolling(MenuSpriteBatch* menu, float width)
{
    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();
    float scaled = Screen::getScaleFactor();

    //Get the screen start of cordinates
    CCPoint origin = Screen::getOrigin();

    float menu_left_padding = 0;
    float min_width = 1000/scaled;
    float collection_width = width;
    if(collection_width < min_width)
    {
        collection_width = min_width;
        menu_left_padding = (min_width - width) / 2;
    }
    //Get the scroll area size
    float scroll_view_width = MIN(visibleSize.width - 276/scaled,
                                  collection_width/*TODO: collection width*/);
    float scroll_view_height = visibleSize.height-150/scaled;

    CCSprite* paper = _papers_spl->loadSprite("collection_background.png");
    float collection_paper_height = paper->getContentSize().height;
    paper->removeFromParent();

    float banner_padding = 15/scaled;
    float overfit = 0;

    float fit_zone = (scroll_view_height - collection_paper_height*1.2f)/2;
    CCSize banner_zone(scroll_view_width, fit_zone*1.8 - banner_padding);
    _bottom_banner = ADAds::getBanner(banner_zone);
    float banner_height = 0;

    if(_bottom_banner)
    {
        overfit = 35/scaled;
        banner_height = _bottom_banner->getContentSize().height +banner_padding + overfit;
        this->addChild(_bottom_banner);
        _bottom_banner->setAnchorPoint(ccp(0.5,0));
    }

    scroll_view_height -= banner_height;
    float move_up = 0;
    if(banner_height > fit_zone)
        move_up = banner_height - fit_zone;

    CCSize scroll_view_size(scroll_view_width, scroll_view_height);

    //Create layer to fit all tiles
    CCLayer *layer = CCLayer::create();
    layer->setContentSize(CCSize(collection_width, scroll_view_size.height));

    //Create scroll view for this layer
    _collections_scroll_view = CCScrollView::create(scroll_view_size,layer);


    float logo_height = _title_select_collection->boundingBox().size.height;


    CCPoint collections_target_position(
                origin.x + visibleSize.width/2 -
                scroll_view_size.width/2 - 53/scaled,
                origin.y + visibleSize.height/2 -
                scroll_view_size.height/2-logo_height/2 + move_up);

    if(_bottom_banner)
    {
        _bottom_banner->setPosition(collections_target_position.x+scroll_view_size.width/2, origin.y+banner_padding);
        //_bottom_banner->showAds();
    }

    _collections_scroll_view->setPosition(2*visibleSize.width,
                                          collections_target_position.y);
    CCRect eat_zone(origin.x,origin.y+banner_height, visibleSize.width, scroll_view_height);
    _collections_scroll_view->setTouchEatZone(eat_zone);
    _collections_scroll_view->updateInset();
    _collections_scroll_view->setDirection(kCCScrollViewDirectionHorizontal);
    this->addChild(_collections_scroll_view);

    //Add our tiles to scroll area
    _collections_scroll_view->addChild(menu);
    _collections_scroll_view->setMenu(menu->menu());
    if(_mode == Collections)
        _collections_scroll_view->setContentOffset(_last_scroll_view_offset, false);
    //_collections_menu->menu()->setTouchPriority(10);
    //collections->setTouchPriority(-500);
    //Place menu just in place
    _pop_up_manager.addScrollViewToAutoDisable(_collections_scroll_view);
    _pop_up_manager.addBannerToAutoDisable(_bottom_banner);
    menu->setPosition(ccp(160/scaled + menu_left_padding,
                          scroll_view_size.height*0.52f-overfit));

    //Collections move in

    CCMoveTo* collections_move_1 = CCMoveTo::create(
                0.8f,
                ccp(collections_target_position.x - 150/scaled,
                    collections_target_position.y));

    CCMoveTo* collections_move_2 = CCMoveTo::create(
                0.2f,
                collections_target_position);
    _collections_scroll_view->runAction(
                CCSequence::create(
                    CCDelayTime::create(0.15f),
                    collections_move_1,
                    collections_move_2,
                    NULL));
}

void SelectCollection::hideEverything(CCCallFunc *callback)
{
    CCFadeTo* logo_fade = CCFadeTo::create(0.15f, 0);
    _title_select_collection->runAction(logo_fade);

    if(_mode == Collections)
        _last_scroll_view_offset = _collections_scroll_view->getContentOffset();

    if(_bottom_banner)
        _bottom_banner->removeFromParent();

    _pop_up_manager.closeWindow();
    _last_scene_ptr = 0;

    const float fade_out_duration = 0.15f;
    float delay = 0.5f;
    for(TileMap::iterator it = _tiles.begin();
        it != _tiles.end(); ++it)
    {
        CCFadeTo* item_fade = CCFadeTo::create(fade_out_duration, 0);
        AnimatedMenuItem* item = it->second.getItem();
        item->stopAllActions();
        item->runAction(item_fade);
        //delay += fade_out_duration/2;
    }

    //_collections_scroll_view->runAction(CCFadeTo::create(fade_out_duration, 0));
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    callback,
                    NULL));

}

void SelectCollection::purchaseReload()
{
    if(_last_scene_ptr)
    {
        SelectCollection* col = _last_scene_ptr;
        col->hideEverything(
                    CCCallFunc::create(
                        col,
                        callfunc_selector(SelectCollection::doReload)));
    }
}

void SelectCollection::purchaseOpenCollections()
{
    if(_last_scene_ptr)
    {
        SelectCollection* col = _last_scene_ptr;
        col->hideEverything(
                    CCCallFunc::create(
                        col,
                        callfunc_selector(SelectCollection::doOpenCollections)));
    }
}

CCPoint SelectCollection::_last_scroll_view_offset = ccp(0,0);
