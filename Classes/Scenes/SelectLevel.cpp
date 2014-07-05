#include "SelectLevel.h"
#include "SelectCollection.h"
#include "Logic/Level.h"
#include "Scenes/LevelScene.h"
#include "Logic/Language.h"
#include "GameInfo.h"
#include "ADLib/Device/ADLanguage.h"

using namespace cocos2d;

SelectLevel::SelectLevel(Collection *colection):
    _menu_name(0),
    _collection(colection),
    _bottom_banner(0)
{
}
cocos2d::CCScene* SelectLevel::scene(Collection *colection)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SelectLevel *layer = SelectLevel::create(colection);

    // add layer as a child to scene
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(SceneStyle::simulateBackClick));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}
void SelectLevel::onBackClick()
{
    hideEverything([](){
        CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
    });
}

void SelectLevel::onLevelSelect(Level* level_to_open)
{
    if(level_to_open->getLevelState()==Level::Locked)
    {
        level_to_open = 0;
        //TODO: play sound of closed level
    }
    else
    {
        hideEverything([level_to_open](){
            CCDirector::sharedDirector()->replaceScene(LevelScene::scene(level_to_open));
        });
    }


}

bool SelectLevel::init()
{
    if ( !SceneStyle::init() )
    {
        return false;
    }

    //To trigger back button
    this->setKeypadEnabled(true);

    //Get the size of the screen we can see

    CCSize VISIBLE_SIZE = ADScreen::getVisibleSize();
     //Get the screen start of cordinates
    CCPoint ORIGIN = ADScreen::getOrigin();
    float SCALE = ADScreen::getScaleFactor();
    float x_middle_of_sheet = (VISIBLE_SIZE.width-133/SCALE)/2 + ORIGIN.x;


//    SpritesLoader menu_name_spl = GraphicsManager::getLoaderFor(
//                this,
//                Language::localizeFileName("select_level/menu_name.plist").c_str(),
//                Language::localizeFileName("select_level/menu_name.png").c_str());

    //Get the Select level label
   // _menu_name = menu_name_spl->loadSprite("select_levels.png");
    _menu_name = CCLabelTTF::create(_("select_levels.title"),
                                           ADLanguage::getFontName(),
                                           GameInfo::SIZE_MENU_TITLE);
    _menu_name->setAnchorPoint(ccp(0.5, 1));
    _menu_name->setPosition(ccp(x_middle_of_sheet,
                                         VISIBLE_SIZE.height + ORIGIN.y - 50/SCALE));
    _menu_name->setColor(GameInfo::COLOR_ORANGE);
    this->addChild(_menu_name);

    //Make it fade in slowly
    _menu_name->setOpacity(0);
    CCFadeTo* title_fade_in = CCFadeTo::create(0.6f, 255);
    _menu_name->runAction(title_fade_in);


    //Put this label at the top of the screen
 //   _menu_name->setAnchorPoint(ccp(0.5, 1));
//    float x_middle_of_sheet = (VISIBLE_SIZE.width-133/SCALE)/2 + ORIGIN.x;
 //   CCPoint logo_target_position = ccp(x_middle_of_sheet,
  //                                     VISIBLE_SIZE.height + ORIGIN.y - 50/SCALE);
 //   _menu_name->setPosition(logo_target_position);
    // _menu_name->setColor(ccc3(71,218,196));

    //Make it fade in slowly
//    _menu_name->setOpacity(0);
//    CCFadeTo* logo_fade_in = CCFadeTo::create(0.3f, 255);
//    _menu_name->runAction(logo_fade_in);

    buildLevelsTiles();
    return true;
}

void SelectLevel::hideEverything(const SceneStyle::Action &callback)
{
    if(_bottom_banner)
        _bottom_banner->removeFromParent();
    CCFadeTo* logo_fade = CCFadeTo::create(0.15f, 0);
    _menu_name->runAction(logo_fade);

    const float fade_out_duration = 0.15f;
    float delay = 0.10f;
    for(unsigned int i=0; i<_levels_arr.size(); ++i)
    {
        CCFadeTo* item_fade = CCFadeTo::create(fade_out_duration, 0);
        _levels_arr[i]->runAction(
                    item_fade);
        //delay += fade_out_duration/2;
    }

    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    ADCallFunc::create(callback),
                    NULL));
}


void SelectLevel::buildLevelsTiles()
{
    //Get the sprites loader
    std::stringstream col_id;
    col_id << _collection->getCollectionID();

    std::string col_id_str = col_id.str();

    std::string plist_name = "select_level/tile_" + col_id_str + ".plist";
    std::string texture_name = "select_level/tile_" + col_id_str + ".png";
    std::string sprite_name = "lt_"+ col_id_str +".png";
    SpritesLoader level_tiles = GraphicsManager::getLoaderFor(
                0,
                plist_name.c_str(),
                texture_name.c_str());


    /*{
        CCSprite* test_button = level_tiles->loadSprite(_current->getLevelButtonFileName().c_str());
        test_button->setPosition(ccp(visibleSize.width/2 + origin.x,
                                 visibleSize.height + origin.y - 60/scaled));
    }*/
    CCPoint origin = ADScreen::getOrigin();
    float scaled = ADScreen::getScaleFactor();
    CCSize visibleSize = ADScreen::getVisibleSize();

    //Create levels menu
    MenuSpriteBatch* levels_menu = MenuSpriteBatch::create(level_tiles);
    this->addChild(levels_menu);

    SpritesLoader locked_tile = GraphicsManager::getLoaderFor(
                levels_menu,
                "select_level/tile_locked.plist",
                "select_level/tile_locked.png");


    //Make the levels menu fill all the screen
    float banner_padding=15/scaled;

    CCSize zone_size(1020/scaled, (visibleSize.height - 170/scaled - banner_padding));


    CCSize banner_size(zone_size.width, zone_size.height*0.25 - banner_padding);
    _bottom_banner = nullptr;//ADAds::getBanner(banner_size);




    levels_menu->setAnchorPoint(ccp(0,1));
    levels_menu->setPosition(64/scaled + origin.x,(origin.y + visibleSize.height - 170/scaled));

    if(_bottom_banner)
    {
        zone_size.height -= _bottom_banner->getContentSize().height + banner_padding;
        this->addChild(_bottom_banner);
        _bottom_banner->setAnchorPoint(ccp(0.5, 0));
        _bottom_banner->setPosition(levels_menu->getPositionX() + zone_size.width/2,
                                    origin.y + banner_padding);
    }
    levels_menu->setContentSize(zone_size);
    //Load images to get the size
    CCSprite* test_button = level_tiles->loadSprite(sprite_name.c_str());
    test_button->removeFromParent();
    test_button->setVisible(false); //TODO: delete this image
    CCSize button_size = test_button->getContentSize();

    //Calculate the buttons size to place them
    const float space_size=1;
    const float units_space_size=1;
    const float units_image_height=button_size.height/space_size;
    const float units_image_width=button_size.width/space_size;

    const unsigned int num_levels=_collection->getLevels().size();
    unsigned int num_ell_in_row=5;
    float resolution = visibleSize.width / visibleSize.height;
    if(resolution < 1.5f)
        num_ell_in_row = 4;
    const unsigned int num_ell_in_col=num_levels/num_ell_in_row;

    float units_width=num_ell_in_row*units_image_width + (num_ell_in_row-1)*units_space_size;
    float units_height=num_ell_in_col*units_image_height + (num_ell_in_col-1)*units_space_size;

    //found what is our units
    float unit=MIN(zone_size.height/units_height,zone_size.width/units_width);

    //find real sizes of buttons and spaces
    float real_image_width=units_image_width*unit;
    float real_image_height=units_image_height*unit;
    float real_space=units_space_size*unit;

    const float scale_factor=real_space/space_size;

    //place buttons on the visible zone
    _levels_arr.reserve(num_levels);
    const float displacement_x=real_image_width/2 + (zone_size.width - units_width*unit)/2;
    const float displacement_y=real_image_height/2 + (zone_size.height - units_height*unit)/2;

    const LevelsArr& levels = _collection->getLevels();
    unsigned int i=0;

    SpritesLoader marks_loader = GraphicsManager::getLoaderFor(
                levels_menu,
                "select_level/marks.plist",
                "select_level/marks.png");
    CCSprite* test_mark = marks_loader->loadSprite("m_red.png");
    CCSize mark_size = test_mark->getContentSize();
    test_mark->setVisible(false);
    test_mark->removeFromParent();

    CCManualFont* font = CCManualFont::create(levels_menu,
                                              "font/mathisfun_digits.plist",
                                              "font/mathisfun_digits.png");

    for (int y=num_ell_in_col-1; y>=0; y--)
    {
        for (unsigned int x=0; x<num_ell_in_row; x++)
        {
            Level* level = levels[i++];

            CCSprite* tile_image = 0;
            if(level->getLevelState() == Level::Locked)
                tile_image = locked_tile->loadSprite("lt_locked.png");
            else
                tile_image =  level_tiles->loadSprite(sprite_name.c_str());

            auto click_action = [level, this](){
                onLevelSelect(level);
            };

            ADMenuItem* item=ADMenuItem::createWithSpriteSheetSprite(
                        tile_image,
                        click_action);

            _levels_arr.push_back(item);
            levels_menu->menu()->addChild(item);

            float item_x = x*(real_image_width+real_space);
            float item_y = y*(real_image_height+real_space);
            item->setPosition(ccp(item_x+displacement_x,item_y+displacement_y));




            Level::LevelState curr_level_state = level->getLevelState();
            //Level is locked
            if (curr_level_state!=Level::Locked)
            {
                //put on our level buttons score tiles or crown
                unsigned int level_id = level->getLevelID() + 1;
                CCSprite* char1 = font->getChar(level_id % 10 + '0');
                CCSprite* char2 = 0;


                //scale the label to fit the button
                float label_original_height=char1->getContentSize().height;
                float label_new_height = (button_size.height*4)/8;
                char1->setScale(scale_factor*label_new_height/label_original_height);
                float anchor_y = 0.2f;
                char1->setAnchorPoint(ccp(0.5f,anchor_y));
                CCPoint label_position(
                            ccp(item->getPositionX(),
                                item->getPositionY()));
                char1->setPosition(label_position);
                char1->setColor(ccc3(94,82,82));
                item->addNephew(char1);
                if(level_id / 10 != 0)
                {
                    char2 = font->getChar(level_id / 10 + '0');
                    item->addNephew(char2);
                    char2->setPosition(char1->getPosition());
                    char2->setColor(char1->getColor());
                    char2->setAnchorPoint(ccp(1, anchor_y));
                    char2->setScale(char1->getScale());

                    char1->setAnchorPoint(ccp(0,anchor_y));
                }
            }
            item->setScaleBase(scale_factor);
            //show on the level button blue, green , red stamps
            //CCSprite* level_status_image = 0;

            //Level is unlocked but has no stamps
            if(curr_level_state!=Level::Locked)
            {
                Stamps stamps = level->getLevelStamps();
                unsigned int stamps_all = level->getSolutions().size();

                float marks_zone_width = button_size.width * 0.725;
                float marks_zone_height = button_size.height * 0.25;
                float relative_y = button_size.height * 0.3;

                float target_width = mark_size.width * stamps_all;
                float target_height = mark_size.height;

                float stamps_scale = MIN(marks_zone_width/ target_width,
                                         marks_zone_height / target_height);

                //float stamp_width = mark_size.width*stamps_scale;
                float stamp_height = mark_size.height*stamps_scale;

                float y_anchor_point = 0.5f - (marks_zone_height/2 - relative_y) / stamp_height;
                float initial_x_anchor_point = 0.5f + (target_width - mark_size.width) / 2 / mark_size.width;

                if(curr_level_state == Level::Crown)
                {
                    CCSprite* crown = marks_loader->loadSprite("m_crown.png");
                    crown->setAnchorPoint(ccp(-0.7, -0.1));
                    crown->setPosition(item->getPosition());
                    //crown->setScale(scale_factor);
                    item->addNephew(crown);
                }

                for(unsigned int i=0; i<stamps_all; ++i)
                {
                    CCSprite* mark = 0;
                    if(i < stamps.size())
                    {
                        Stamp::Color st_color = stamps[i].getColor();
                        if(st_color == Stamp::Red)
                            mark = marks_loader->loadSprite("m_red.png");
                        else if(st_color == Stamp::Green)
                            mark = marks_loader->loadSprite("m_green.png");
                        else
                            mark = marks_loader->loadSprite("m_blue.png");
                    }
                    else
                        mark = marks_loader->loadSprite("m_none.png");


                    mark->setAnchorPoint(ccp(initial_x_anchor_point - i, y_anchor_point));
                    mark->setPosition(item->getPosition());
                    mark->setScale(stamps_scale);
                    item->addNephew(mark);
                }

            }


        }
    }

    levels_menu->menu()->setOpacity(0);
    levels_menu->menu()->runAction(CCFadeTo::create(0.3f, 255));

}
