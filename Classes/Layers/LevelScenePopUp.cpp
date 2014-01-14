#include "LevelScenePopUp.h"
#include "DebugLayer.h"
#include "Logic/RW.h"
#include "EquationDrawer.h"
#include "Scenes/SelectCollection.h"
#include "Scenes/SelectLevel.h"
#include "Scenes/LevelScene.h"
#include "Logic/Tutorial.h"
#include "DisplaySolution.h"
#include "Logic/Language.h"

CCAction* getNewStampAction(float stamps_scale)
{
    return CCSequence::create(
                CCScaleTo::create(0,0),
                CCDelayTime::create(0.3f),
                CCScaleTo::create(0.2f, stamps_scale*1.2f),
                CCScaleTo::create(0.18f, stamps_scale*0.8f),
                CCScaleTo::create(0.16f, stamps_scale*1.0f),
                NULL
                );
}

bool LevelScenePopUp::init()
{
    /*if ( !CCLayer::init() )
    {
        return false;
    }*/
    Level* current_level = _parent->getLevel();
    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = Screen::getOrigin();
    float scaled = Screen::getScaleFactor();

    //Create the sheet
    //_sheet_menu = CCNode::create();
    //this->addChild(_sheet_menu);
    _sheet_menu = this;
    SpritesLoader spl = GraphicsManager::getLoaderFor(_sheet_menu,
                                                      "level/level_end_menu.plist",
                                                      "level/level_end_menu.png");
    spl->inject(true);
    CCSprite* sheet = spl->loadSprite("sheet-horizontal.png");
    //const unsigned int ELEMENTS_LAYER=100;
    //sheet->setVisible(false);

    //Create the zone for menu
    float banner_padding = 15/scaled;
    float zone_margin = 0;

    CCSize menu_zone_size(sheet->getContentSize());
    float free_space = (visibleSize.height - menu_zone_size.height*1.1)/2;
    float max_space = free_space * 1.8;
    float move_up = 0;
    CCSize banner_size(visibleSize.width*0.9, max_space - banner_padding);

    _pause_banner = ads::AdsManager::getInstance().getBanner(banner_size);
    if(_pause_banner)
    {
        _parent->getParentScene()->addChild(_pause_banner);
        _pause_banner->setAnchorPoint(ccp(0.5,0));
        _pause_banner->setPosition(origin.x + visibleSize.width/2,
                                   origin.y + banner_padding);
        _pause_banner->showAds();

        float banner_height = _pause_banner->getContentSize().height + banner_padding;
        if(banner_height > free_space)
            move_up = banner_height - free_space;
        zone_margin = move_up;
    }


    _sheet_target_position = ccp(visibleSize.width/2+origin.x,
                                (visibleSize.height-zone_margin)/2+zone_margin+origin.y);
    _sheet_menu->setPosition(ccp(_sheet_target_position.x,
                                 _sheet_target_position.y-visibleSize.height));
    _sheet_menu->setContentSize(menu_zone_size);
    _sheet_menu->setAnchorPoint(ccp(0.5,0.5));
    _sheet_menu->runAction(CCMoveTo::create(0.2f, _sheet_target_position));

    //sheet->setRotation(-90);
    sheet->setAnchorPoint(ccp(0,0));
    //sheet->removeFromParent();

    //Place buttons
    SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                           "level/level_end.plist",
                                                           "level/level_end.png");
    MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
    menu->setPosition(sheet->getPosition());
    menu->setAnchorPoint(ccp(0,0));
    menu->setContentSize(menu_zone_size);
    _sheet_menu->addChild(menu);

    CCSprite* next_level_image = menu_spl->loadSprite("next_level.png");

    CCSize image_size = next_level_image->getContentSize();
    float design_scale = 1;

    AnimatedMenuItem *first_button = 0;
    if(current_level->getLevelState() == Level::Crown)
    {
        CCSprite* levels = menu_spl->loadSprite("levels.png");
        first_button = AnimatedMenuItem::create(
                    levels,
                    this, menu_selector(LevelScenePopUp::onLevels));
    }
    else
    {
        CCSprite* find_more_image = menu_spl->loadSprite("find_more.png");
        first_button = AnimatedMenuItem::create(
                    find_more_image,
                    this, menu_selector(LevelScenePopUp::onFindMoreSolutions));
    }
    //TODO: change if it is necessary x coordinate to 102
    first_button->setPosition(ccp(100*design_scale/scaled+image_size.width/2,
                                  53*design_scale/scaled+image_size.height/2));

    AnimatedMenuItem *next_level = AnimatedMenuItem::create(
                next_level_image,
                this, menu_selector(LevelScenePopUp::onNextLevel));
    next_level->setPosition(ccp(600*design_scale/scaled,
                                53*design_scale/scaled+image_size.height/2));
    menu->menu()->addChild(next_level);
    menu->menu()->addChild(first_button);

    //Add label
    //    CCSprite* title = 0;
    //    if(current_level->getLevelState() == Level::OneStamp)
    //        title = spl->loadSprite("good.png");
    //    else if(current_level->getLevelState() == Level::TwoStamps)
    //        title = spl->loadSprite("perfect.png");
    //    else
    //        title = spl->loadSprite("excellent.png");

    //    title->setPosition(ccp(409*design_scale/scaled, 445*design_scale/scaled));
    const Solutions& all_found_solutions = _parent->getLevel()->getFoundSolutions();
    Solutions last_solution(1, all_found_solutions.back());
    DisplaySolution* solution = DisplaySolution::create(CCSize(371/scaled, 90/scaled),
                                                        _parent->getLevel(),
                                                        last_solution,
                                                        last_solution);
    this->addChild(solution);
    solution->setAnchorPoint(ccp(0.5f,0.5f));
    solution->setPosition(ccp(409*design_scale/scaled, 445*design_scale/scaled));


    std::vector<CCSprite*> stamps;


    const Stamps level_stamps = current_level->getLevelStamps();
    unsigned int stamps_number = current_level->getSolutions().size();
    CCNode* stamps_zone = 0;
    float max_stamps_zone_width = 605*design_scale/scaled;
    float max_stamps_zone_height = 160*design_scale/scaled;

    if(stamps_number)
    {
        stamps.reserve(stamps_number);
        stamps_zone = CCNode::create();
        SpritesLoader stamps_spl = GraphicsManager::getLoaderFor(stamps_zone,
                                                                 "level/stamps.plist",
                                                                 "level/stamps.png");

        stamps_zone->setContentSize(CCSize(max_stamps_zone_width,
                                           max_stamps_zone_height));
        stamps_zone->setPosition(ccp(414*design_scale/scaled, 289*design_scale/scaled));
        stamps_zone->setAnchorPoint(ccp(0.5f, 0.5f));
        _sheet_menu->addChild(stamps_zone);


        unsigned int level_max_solutions = stamps_number;
        CCSize stamp_size;
        //stamp_size.width *= 0.7f;
        float stamps_scale = 0;

        CCSize new_stamp_size;

        float new_width = new_stamp_size.width * level_max_solutions;
        float new_height = new_stamp_size.height;

        float h_disp = 0;
        float v_dist = 0;

        _crown = 0;

        for(unsigned int i=0; i<stamps_number; ++i)
        {
            CCSprite* sp = 0;
            if(i < level_stamps.size())
            {
                const Stamp& stamp = level_stamps[i];
                sp = stamps_spl->loadSprite(stamp.getFileName().c_str());
                stamps.push_back(sp);
                sp->setRotation(stamp.getRotation());

                if(i == 0)
                {
                    stamp_size = stamps[0]->getContentSize();
                    //stamp_size.width *= 0.7f;
                    stamps_scale = MIN(max_stamps_zone_height / stamp_size.height,
                                             max_stamps_zone_width / stamp_size.width / level_max_solutions);

                    new_stamp_size = CCSize(stamp_size.width * stamps_scale,
                                          stamp_size.height * stamps_scale);

                    new_width = new_stamp_size.width * level_max_solutions;
                    new_height = new_stamp_size.height;

                    h_disp = (max_stamps_zone_width - new_width + new_stamp_size.width) / 2;
                    v_dist = (max_stamps_zone_height - new_height + new_stamp_size.height) / 2;
                }



                sp->setPosition(ccp(h_disp + i * new_stamp_size.width,
                                    v_dist));

                if(stamp.newStamp())
                {
                    /*CCSequence* seq2 = CCSequence::createWithTwoActions(
                                CCFadeTo::create(0.5f, 130),
                                CCFadeTo::create(0.5f, 255));*/

                    //CCSequence* seq2 = ;

                    sp->runAction(getNewStampAction(stamps_scale));

                    if(current_level->getLevelState() == Level::Crown)
                    {
                        //Add crown to last stamp
                        CCSprite* spc = stamps_spl->loadSprite("crown_st.png");
                        spc->setScale(stamps_scale);
                        spc->setPosition(sp->getPosition());

                        spc->setAnchorPoint(ccp(0.5, 0));
                        spc->runAction(getNewStampAction(stamps_scale));
                        _crown = spc;
                    }
                }
            }
            else
            {
                sp = stamps_spl->loadSprite("empty_stamp.png");
                sp->setPosition(ccp(h_disp + i * new_stamp_size.width,
                                    v_dist));

                stamps.push_back(sp);
            }

            sp->setScale(stamps_scale);

        }

    }
    //_parent->setVisible(false);
    _parent->sleep();

    Tutorial::getInstance()->onSolutionFound(this);
#ifdef DEBUG_LAYER
    DebugLayer* debug = DebugLayer::create();
    this->addChild(debug,300);
    debug->addChild(_sheet_menu);
    DebugLayer* debug2 = DebugLayer::create();
    _sheet_menu->addChild(debug2,300);
    //debug2->addChild(sheet);
    // debug2->addChild(menu);
    // debug2->addChild(next_level);
    if(stamps_zone)
    {
        debug2->addChild(stamps_zone);
        DebugLayer* debug3 = DebugLayer::create();
        stamps_zone->addChild(debug3);
        for(unsigned int i=0; i<stamps_number; ++i)
        {
            CCSprite* sp = stamps[i];
            debug3->addChild(sp);
        }
    }
#endif
    return true;
}

void LevelScenePopUp::hideMe(CCCallFunc* callback)
{
    if(_pause_banner)
        _pause_banner->hideAds();

    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();
    this->runAction(
                CCSequence::create(
                    CCMoveTo::create(
                        0.3f,
                        ccp(_sheet_target_position.x,
                            _sheet_target_position.y + visibleSize.height)),
                    callback,
                    NULL));
}
void LevelScenePopUp::onLevels(CCObject*)
{
    this->hideMe(CCCallFunc::create(
                     this,
                     callfunc_selector(LevelScenePopUp::do_onLevels)));
}
void LevelScenePopUp::onNextLevel(CCObject*)
{
    this->hideMe(CCCallFunc::create(
                     this,
                     callfunc_selector(LevelScenePopUp::do_onNextLevel)));
}

void LevelScenePopUp::do_onLevels()
{
    CCDirector::sharedDirector()->replaceScene(
                SelectLevel::scene(_parent->getLevel()->getLevelCollection()));

}

void LevelScenePopUp::do_onNextLevel()
{
    Level* next = RW::getNextLevel(_parent->getLevel());
    if(next)
        CCDirector::sharedDirector()->replaceScene(LevelScene::scene(next));
    else
        CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());

}

void LevelScenePopUp::onFindMoreSolutions(CCObject*)
{
    _parent->wakeup();
    _parent->showUI();
    _parent->onFoundSolutionClose();
    this->hideMe(CCCallFunc::create(
                     this,
                     callfunc_selector(LevelScenePopUp::selfDestroy)));
}
void LevelScenePopUp::selfDestroy()
{
    this->removeFromParentAndCleanup(true);
}
CCSprite* LevelScenePopUp::getCrown()
{
    return _crown;
}
