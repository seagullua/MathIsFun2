#include "LevelStop.h"
#include "Scenes/SelectLevel.h"
#include "Scenes/SelectCollection.h"
#include "Scenes/LevelScene.h"
#include "Logic/RW.h"
#include "Logic/Language.h"

using namespace cocos2d;
bool LevelStop::init()
{
    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = ADScreen::getOrigin();
    float scaled = ADScreen::getScaleFactor();

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



    _menu.push_back(CCSprite::create("stop_level/Resume.png"));
    ADMenuItem* resume_button = ADMenuItem::create(
                _menu[_menu.size()-1]);
    CONNECT(resume_button->signalOnClick,
            this, &LevelStop::onResume);

    _menu.push_back(CCSprite::create("stop_level/Restart.png"));
    ADMenuItem* restart_button = ADMenuItem::create(
                _menu[_menu.size()-1]);
    CONNECT(restart_button->signalOnClick,
            this, &LevelStop::onRestart);

    _menu.push_back(CCSprite::create("stop_level/Skip_level.png"));
    ADMenuItem* skip_button = ADMenuItem::create(
                _menu[_menu.size()-1]);
    CONNECT(skip_button->signalOnClick,
            this, &LevelStop::onSkipLevel);


    _menu.push_back(CCSprite::create("stop_level/Found_Solutions.png"));
    ADMenuItem* found_solutions_button = ADMenuItem::create(
                _menu[_menu.size()-1]);
    CONNECT(found_solutions_button->signalOnClick,
            this, &LevelStop::onFoundSolutions);


    _menu.push_back(CCSprite::create("stop_level/Levels.png"));
    ADMenuItem* levels_button = ADMenuItem::create(
                _menu[_menu.size()-1]);
    CONNECT(levels_button->signalOnClick,
            this, &LevelStop::onLevels);


    CCMenu* menu = CCMenu::create();
    // menu->setPosition(ccp(visibleSize.width/2+origin.x,
    //visibleSize.height/2+origin.y+scaled/20));

    menu->addChild(resume_button);
    menu->addChild(restart_button);
    menu->addChild(skip_button);
    menu->addChild(found_solutions_button);
    menu->addChild(levels_button);

    this->addChild(menu);

    menu->alignItemsVerticallyWithPadding(1/scaled);
    menu->setPosition(ccp(visibleSize.width/2 + origin.x,
                          visibleSize.height/2 + origin.y - 20/scaled));




    this->setPosition(ccp(this->getPosition().x,
                          this->getPositionY()+visibleSize.height));

    //CCSize visibleSize = ADScreen::getVisibleSize();
    this->runAction(
                CCMoveTo::create(
                    0.15f,
                    ccp(this->getPositionX(),
                        this->getPositionY()- visibleSize.height))
                );
    return true;
}

void LevelStop::onResume()
{
    _parent->wakeup();
    _level_scene->showMe();
    this->hideMe([this](){
        this->selfDestroy();
    });
}
void LevelStop::onRestart()
{
    _level_scene->hidePauseAndPlay();
    hideMe([this](){
        CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_level));
    });
}


void LevelStop::onSkipLevel()
{
    ADStatistics::logEvent("Skip Level", levelToStatisticsParams(_level));

    _level_scene->hidePauseAndPlay();

    hideMe([this](){
        Level* next = RW::skipLevel(_parent->getLevel());
        if(next)
            CCDirector::sharedDirector()->replaceScene(LevelScene::scene(next));
        else
            CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
    });
}

void LevelStop::onFoundSolutions()
{
    this->hideMe([this](){
        this->selfDestroy();
        _level_scene->showFoundSolutions();
    });
}
void LevelStop::onLevels()
{
    _level_scene->hidePauseAndPlay();
    hideMe([this](){
        CCDirector::sharedDirector()->replaceScene(SelectLevel::scene
                                                   (_level->getLevelCollection()));
    });
}


//Action after all animation have finished
void LevelStop::hideMe(const ADCallFunc::Action & callback)
{
    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();
    this->runAction(
                CCSequence::create(
                    CCMoveTo::create(
                        0.3f,
                        ccp(this->getPositionX(),
                            this->getPositionY()-visibleSize.height)),
                    ADCallFunc::create(callback),
                    NULL));
}
void LevelStop::selfDestroy()
{
    this->removeFromParentAndCleanup(true);
}


