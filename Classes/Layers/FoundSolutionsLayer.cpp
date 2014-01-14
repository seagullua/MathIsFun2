#include "FoundSolutionsLayer.h"
#include "DisplaySolution.h"
#include "Logic/Language.h"
FoundSolutionsLayer::FoundSolutionsLayer(Level* level)
{


    CCSize visibleSize = Screen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = Screen::getOrigin();
    float scaled = Screen::getScaleFactor();

    CCNode* sheet_menu = this;
    SpritesLoader spl = GraphicsManager::getLoaderFor(sheet_menu,
                                                      "level/level_end_menu.plist",
                                                      "level/level_end_menu.png");
    spl->inject(true);
    CCSprite* sheet = spl->loadSprite("sheet-horizontal.png");



    CCSize menu_zone_size(sheet->getContentSize());

    _sheet_target_position = ccp(visibleSize.width/2+origin.x,
                                 visibleSize.height/2+origin.y);
    sheet_menu->setPosition(ccp(_sheet_target_position.x,
                                 _sheet_target_position.y-visibleSize.height));
    sheet_menu->setContentSize(menu_zone_size);
    sheet_menu->setAnchorPoint(ccp(0.5,0.5));
    sheet_menu->runAction(CCMoveTo::create(0.2f, _sheet_target_position));

    //sheet->setRotation(-90);
    sheet->setAnchorPoint(ccp(0,0));

    DisplaySolution* solutions = DisplaySolution::create(
                CCSize(640/scaled, 420/scaled),
                level,
                level->getFoundSolutions(),
                level->getSolutions(),
                true);
    solutions->setAnchorPoint(ccp(0,0));
    solutions->setPosition(ccp(95/scaled, 62/scaled));
    sheet_menu->addChild(solutions);

    /*SpritesLoader logo_spl = GraphicsManager::getLoaderFor(sheet_menu,
                                                      Language::localizeFileName("level/found_solutions.plist").c_str(),
                                                      Language::localizeFileName("level/found_solutions.png").c_str());
    CCSprite* logo = logo_spl->loadSprite("fs_logo.png");
    logo->setPosition(ccp(424/scaled, 445/scaled));*/
}

FoundSolutionsLayer* FoundSolutionsLayer::create(Level* level)
{
    FoundSolutionsLayer* pObj = new FoundSolutionsLayer(level);
    if(pObj)
        pObj->autorelease();
    return pObj;
}

void FoundSolutionsLayer::hideAndRemove()
{
    CCSize visibleSize = Screen::getVisibleSize();
    this->runAction(CCSequence::createWithTwoActions(
                        CCMoveTo::create(0.3f, ccp(_sheet_target_position.x,
                                                   _sheet_target_position.y+visibleSize.height)),
                        CCCallFunc::create(this, callfunc_selector(FoundSolutionsLayer::selfDestroy))
                        ));
}

void FoundSolutionsLayer::selfDestroy()
{
    this->removeFromParentAndCleanup(true);
}
