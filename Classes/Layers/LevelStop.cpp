#include "LevelStop.h"
#include "Scenes/SelectLevel.h"
#include "Scenes/SelectCollection.h"
#include "Scenes/LevelScene.h"
#include "Logic/RW.h"
#include "Logic/Language.h"
#include "Core/Statistics.h"
#define TEMPORARY_HIDE
bool LevelStop::init()
{
    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = Screen::getOrigin();
    float scaled = Screen::getScaleFactor();

    _sheet_target_position = ccp(visibleSize.width/2+origin.x,
                                 visibleSize.height/2+origin.y);

    _sheet_menu=this;
    SpritesLoader spl = GraphicsManager::getLoaderFor(
                this,
                "sheet_menu.plist",
                "sheet_menu.png");
    spl->inject(true);
    CCSprite* sheet = spl->loadSprite("pop_up_paper.png");

    //Create the zone for menu
    //CCSize menu_zone_size(sheet->getContentSize());

    sheet->setPosition(ccp(visibleSize.width/2+origin.x,
                           visibleSize.height/2+origin.y));
    //sheet->setPosition(ccp(_sheet_target_position.x,
    //_sheet_target_position.y-visibleSize.height));
    //_sheet_menu->setContentSize(menu_zone_size);
    //_sheet_menu->setAnchorPoint(ccp(0.5,0.5));
    //_sheet_menu->runAction(CCMoveTo::create(0.2f, _sheet_target_position));

    //sheet->setRotation(-90);
    // sheet->setAnchorPoint(ccp(0,0));



    SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                           Language::localizeFileName("stop_level/stop_level_menu.plist").c_str(),
                                                           Language::localizeFileName("stop_level/stop_level_menu.png").c_str());
    //CCSprite* level = menu_spl->loadSprite("Resume.png");
    _menu.push_back(menu_spl->loadSprite("Resume.png"));
    ADMenuItem* resume_button = ADMenuItem::create(
                _menu[_menu.size()-1],
            this, menu_selector(LevelStop::onResume));

    _menu.push_back(menu_spl->loadSprite("Restart.png"));
    ADMenuItem* restart_button = ADMenuItem::create(
                _menu[_menu.size()-1],
            this, menu_selector(LevelStop::onRestart));

    _menu.push_back(menu_spl->loadSprite("Skip_level.png"));
    ADMenuItem* skip_button = ADMenuItem::create(
                _menu[_menu.size()-1],
            this, menu_selector(LevelStop::onSkipLevel));


    _menu.push_back(menu_spl->loadSprite("Found_Solutions.png"));
    ADMenuItem* found_solutions_button = ADMenuItem::create(
                _menu[_menu.size()-1],
            this, menu_selector(LevelStop::onFoundSolutions));


    _menu.push_back(menu_spl->loadSprite("Levels.png"));
    ADMenuItem* levels_button = ADMenuItem::create(
                _menu[_menu.size()-1],
            this, menu_selector(LevelStop::onLevels));


    MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
    // menu->setPosition(ccp(visibleSize.width/2+origin.x,
    //visibleSize.height/2+origin.y+scaled/20));

    menu->menu()->addChild(resume_button);
    menu->menu()->addChild(restart_button);
    menu->menu()->addChild(skip_button);
    menu->menu()->addChild(found_solutions_button);
    menu->menu()->addChild(levels_button);

    this->addChild(menu);

    menu->menu()->alignItemsVerticallyWithPadding(1/scaled);
    menu->setPosition(ccp(visibleSize.width/2 + origin.x,
                          visibleSize.height/2 + origin.y - 20/scaled));




    this->setPosition(ccp(this->getPosition().x,
                          this->getPositionY()+visibleSize.height));

    //CCSize visibleSize = Screen::getVisibleSize();
    this->runAction(
                CCMoveTo::create(
                    0.15f,
                    ccp(this->getPositionX(),
                        this->getPositionY()- visibleSize.height))
                );
    return true;
}

void LevelStop::onResume(CCObject*)
{
    _parent->wakeup();
    _level_scene->showMe();
    this->hideMe(CCCallFunc::create(
                     this,
                     callfunc_selector(LevelStop::selfDestroy)));
}
void LevelStop::onRestart(CCObject*)
{
    _level_scene->hidePauseAndPlay();
    CCCallFunc* after_hide = CCCallFunc::create(this,
                                                callfunc_selector(LevelStop::doRestart));
    hideMe(after_hide);
}
void LevelStop::doRestart()
{
    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_level));
}

void LevelStop::onSkipLevel(CCObject*)
{
    ADStatistics::logEvent("Skip Level", levelToStatisticsParams(_level));

    _level_scene->hidePauseAndPlay();
    CCCallFunc* after_hide = CCCallFunc::create(this,
                                                callfunc_selector(LevelStop::doSkipPressed));
    hideMe(after_hide);
}
void LevelStop::doSkipPressed()
{
    Level* next = RW::skipLevel(_parent->getLevel());
    if(next)
        CCDirector::sharedDirector()->replaceScene(LevelScene::scene(next));
    else
        CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
}
void LevelStop::onFoundSolutions(CCObject*)
{
    this->hideMe(CCCallFunc::create(
                     this,
                     callfunc_selector(LevelStop::do_onFindMoreSolutions)));
    //CCDirector::sharedDirector()->replaceScene(FoundSolutions::scene(_level->getSolutions()));


}
void LevelStop::onLevels(CCObject*)
{
    _level_scene->hidePauseAndPlay();
    CCCallFunc* after_hide = CCCallFunc::create(this,
                                                callfunc_selector(LevelStop::doLevelsPressed));
    hideMe(after_hide);
}


void LevelStop::doLevelsPressed()
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene
                                               (_level->getLevelCollection()));
}
//Action after all animation have finished
void LevelStop::hideMe(CCCallFunc* callback)
{
    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();
    this->runAction(
                CCSequence::create(
                    CCMoveTo::create(
                        0.3f,
                        ccp(this->getPositionX(),
                            this->getPositionY()-visibleSize.height)),
                    callback,
                    NULL));
}
void LevelStop::selfDestroy()
{
    this->removeFromParentAndCleanup(true);
}
void LevelStop::do_onFindMoreSolutions()
{
    this->selfDestroy();
    _level_scene->showFoundSolutions();
}

