#include "SelectCollection.h"
#include "MainMenu.h"
#include "Logic/RW.h"
#include "SelectLevel.h"
#include "Logic/Language.h"
#include "SelectCollection_UnlockAnimator.h"
#include "PopUp/BuyFullVersion.h"
#include "Store.h"
#include "GameInfo.h"
#include "ADLib/Device/ADLanguage.h"
#include "Logic/Language.h"
using namespace cocos2d;



SelectCollection::SelectCollection()
    : _title_select_collection(0),
      _bottom_banner(0)
{
}

CCScene* SelectCollection::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SelectCollection *layer = SelectCollection::create();

    // add layer as a child to scene
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(SceneStyle::simulateBackClick));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}

SelectCollection* SelectCollection::create()
{
    SelectCollection *pRet = new SelectCollection();
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
void SelectCollection::onBackClick()
{

    hideEverything([this](){
        CCDirector::sharedDirector()->replaceScene(
                    MainMenu::scene());
    });

}

void SelectCollection::onCollectionSelect(const CollectionTile& selected)
{

    if(!selected.isShopTile())
    {
        //We selected this collection
        Collection* current = selected.getCollection();

        if(current->getCollectionState()==Collection::Locked)
        {
            //open Window Buy Full Version
            _pop_up_manager.openWindow(new BuyFullVersion());
        }
        else
        {
            hideEverything([current](){
                CCDirector::sharedDirector()->replaceScene(
                            SelectLevel::scene(current));
            });

        }
    }

}
void toTranslation()
{
   _("collection_name.100");
   _("collection_name.200");
   _("collection_name.300");
   _("collection_name.400");
   _("collection_name.500");
   _("collection_name.600");

   _("collection_name.1000");
   _("collection_name.2000");
   _("collection_name.3000");
   _("collection_name.4000");

   _("collection_name.locked");
}
ADMenuItem* SelectCollection::createCollectionItem(const std::string& image_name,
                                                   const std::string& label_name,
                                                   const std::string& stamps_label_text,
                                                   const ccColor3B& color,
                                                   const SpritesLoader& papers_spl,
                                                   const std::string &stamp_file_name,
                                                   bool show_crown,
                                                   bool show_new,
                                                   bool more_level_tile)
{
    CCSprite* background = papers_spl->loadSprite("collection_background.png");
    ADMenuItem* paper_item = ADMenuItem::createWithSpriteSheetSprite(background);


    //create new collection
    SpritesLoader new_col_spl = GraphicsManager::getLoaderFor(
                paper_item,
                "select_collection/new_collection2.plist",
                "select_collection/new_collection2.png");
    new_col_spl->inject();

    CCSize paper_size = background->getContentSize();

    CCSprite* image = CCSprite::create(image_name.c_str());
    image->setAnchorPoint(ccp(0.5f,0.5f));
    float middle_width = paper_size.width/2;
    image->setPosition(ccp(middle_width, paper_size.height*0.75));
    paper_item->addChild(image);

    float scaled = ADScreen::getScaleFactor();
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


    //add collection name or locked title
    CCLabelTTF* label = CCLabelTTF::create(_(label_name),
                                               ADLanguage::getFontName(),
                                               35);

    //CCSprite* label = CCSprite::create(label_name.c_str());
    label->setPosition(line->getPosition());
    paper_item->addChild(label);

    if(!more_level_tile)
    {
        CCSprite* stamp = new_col_spl->loadSprite(stamp_file_name.c_str());
        stamp->setPosition(ccp(0,paper_size.height*0.16));
        stamp->setAnchorPoint(ccp(1, 0.5));

        CCNode* stamps_number = 0;

        ccColor3B label_color(ccc3(61,63,64));
        stamps_number = createLabel(stamps_label_text,  "font/mathisfun_digits.plist",
                                    "font/mathisfun_digits.png",
                                    label_color);

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
        if(stamps_label_text==" ")
        {
            stamp->setAnchorPoint(ccp(0.5, 0.5));
            stamp->setPositionX(center);
        }
        else
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

ADMenuItem* SelectCollection::createCollectionItem(Collection* collection, const SpritesLoader& spl)
{
    std::stringstream stamps_label;
    std::stringstream image_name;
    std::string collection_title="";
    //std::stringstream label_name;

    ccColor3B color;
    bool show_crown = false;
    bool show_new = false;
    stamps_label << " ";
    std::string stamps_label_str;
    if(collection->getCollectionState() == Collection::Locked)
    {
        image_name << "select_collection/coll_lock.png";
        collection_title="collection_name.locked";
        color = GameInfo::COLOR_LIGHT_GRAY;
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
        image_name << "select_collection/coll_"<< collection->getCollectionID() <<".png";
        //label_name << "select_collection/clabel_"<< collection->getCollectionID() <<".png";
        collection_title = "collection_name."+AD_to_string(collection->getCollectionID());
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

    //Language::localizeFileName(label_name.str())

    return createCollectionItem(image_name.str(),
                                collection_title,
                                stamps_label_str,
                                color,
                                spl,
                                stamp_file_name,
                                show_crown,
                                show_new,
                                false);
}

bool SelectCollection::init()
{
    if ( !SceneStyle::init() )
    {
        return false;
    }

    _last_scene_ptr = this;

    //To trigger back button
    this->setKeypadEnabled(true);

    //Get the size of the screen we can see

    CCSize VISIBLE_SIZE = ADScreen::getVisibleSize();
    CCPoint ORIGIN = ADScreen::getOrigin();
    float SCALE = ADScreen::getScaleFactor();
    float x_middle_of_sheet = (VISIBLE_SIZE.width-133/SCALE)/2 + ORIGIN.x;

    //Get the sprites loader
    SpritesLoader spl;

    //window title
    _title_select_collection = CCLabelTTF::create(_("select_collection.title"),
                                           ADLanguage::getFontName(),
                                           GameInfo::SIZE_MENU_TITLE);
    _title_select_collection->setAnchorPoint(ccp(0.5, 1));
    _title_select_collection->setPosition(ccp(x_middle_of_sheet,
                                         VISIBLE_SIZE.height + ORIGIN.y - 50/SCALE));
    _title_select_collection->setColor(GameInfo:: COLOR_ORANGE);
    this->addChild(_title_select_collection);

    //Make it fade in slowly
    _title_select_collection->setOpacity(0);
    CCFadeTo* title_fade_in = CCFadeTo::create(0.6f, 255);
    _title_select_collection->runAction(title_fade_in);




    //Put this label at the top of the screen
    _title_select_collection->setAnchorPoint(ccp(0.5, 1));
    //float x_middle_of_sheet = (VISIBLE_SIZE.width-133/SCALE)/2 + ORIGIN.x;
    CCPoint logo_target_position = ccp(x_middle_of_sheet,
                                       VISIBLE_SIZE.height + ORIGIN.y - 50/SCALE);
    _title_select_collection->setPosition(logo_target_position);

//    //Make it fade in slowly
//    _title_select_collection->setOpacity(0);
//    CCFadeTo* logo_fade_in = CCFadeTo::create(0.3f, 255);
//    _title_select_collection->runAction(logo_fade_in);


    //Create menu with collections


    _papers_spl = GraphicsManager::getLoaderFor(
                0,
                "select_collection/new_collection.plist",
                "select_collection/new_collection.png");
    _collections_menu = MenuSpriteBatch::create(_papers_spl);


    float padding_left = 330/SCALE;
    unsigned int i = 0;

    const CollectionsArr& arr = RW::getCollections();
    bool unlocked_one = false;
    for(CollectionsArr::const_iterator it=arr.begin(); it!=arr.end();++it)
    {
        Collection* a = it->second;


        ADMenuItem* paper_item = createCollectionItem(a, _papers_spl);
        _collections_menu->menu()->addChild(paper_item, 100);
        //paper_item->setBaseScale(0.7f);
        paper_item->setPositionX(i * padding_left);
        i++;

        CollectionTile tile(paper_item, a);
        _tiles[paper_item] = tile;
        paper_item->setClickAction([tile, this](){
            onCollectionSelect(tile);
        });

        if(a->isReadyToBeUnlocked())
        {

            CollectionUnlockAnimator* anim = new CollectionUnlockAnimator(_tiles[paper_item], this);
            anim->autorelease();
            anim->unlockItem();
            RW::unlockCollection(a);
            unlocked_one = true;
        }

    }

    if(unlocked_one)
    {
        RW::saveGame();
    }

    _pop_up_manager.addMenuToAutoDisable(_collections_menu->menu());
    newScrolling(_collections_menu, i*padding_left + 20/SCALE);

    return true;
}


void SelectCollection::newScrolling(MenuSpriteBatch* menu, float width)
{
    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();
    float scaled = ADScreen::getScaleFactor();

    //Get the screen start of cordinates
    CCPoint origin = ADScreen::getOrigin();

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
    _collections_scroll_view = ADScrollView::create(scroll_view_size,layer);


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
    _collections_scroll_view->setDirection(ADScrollView::Direction::Horizontal);
    this->addChild(_collections_scroll_view);

    //Add our tiles to scroll area
    _collections_scroll_view->addChild(menu);
    _collections_scroll_view->addHighPriorityTouchListener(menu->menu());

    _collections_scroll_view->setContentOffset(_last_scroll_view_offset, false);
    //_collections_menu->menu()->setTouchPriority(10);
    //collections->setTouchPriority(-500);
    //Place menu just in place
    _pop_up_manager.addTouchZoneToAutoDisable(_collections_scroll_view);
    //_pop_up_manager.addBannerToAutoDisable(_bottom_banner);
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

void SelectCollection::hideEverything(const Action& callback)
{
    CCFadeTo* logo_fade = CCFadeTo::create(0.15f, 0);
    _title_select_collection->runAction(logo_fade);


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
        ADMenuItem* item = it->second.getItem();
        item->stopAllActions();
        item->runAction(item_fade);
        //delay += fade_out_duration/2;
    }

    //_collections_scroll_view->runAction(CCFadeTo::create(fade_out_duration, 0));
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    ADCallFunc::create(callback),
                    NULL));

}

void SelectCollection::purchaseReload()
{
    if(_last_scene_ptr)
    {
        SelectCollection* col = _last_scene_ptr;
        col->hideEverything([col](){
            CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
        });
    }
}

void SelectCollection::purchaseOpenCollections()
{
    if(_last_scene_ptr)
    {
        SelectCollection* col = _last_scene_ptr;
        col->hideEverything([](){
            CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
        });
    }
}

CCPoint SelectCollection::_last_scroll_view_offset = ccp(0,0);
