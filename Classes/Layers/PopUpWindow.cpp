#include "PopUpWindow.h"
#include "cocos2d-A.h"
using namespace cocos2d;
void PopUpWindowStyle::initDesing(cocos2d::CCNode* sheet_menu)
{
    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = ADScreen::getOrigin();


    SpritesLoader spl = GraphicsManager::getLoaderFor(sheet_menu,
                                                      "level/level_end_menu.plist",
                                                      "level/level_end_menu.png");
    spl->inject(true);
    CCSprite* sheet = spl->loadSprite("sheet-horizontal.png");
    //const unsigned int ELEMENTS_LAYER=100;
    //sheet->setVisible(false);

    //Create the zone for menu
    CCSize menu_zone_size(sheet->getContentSize());

    _sheet_target_position = ccp(visibleSize.width/2+origin.x - menu_zone_size.width/2,
                                 visibleSize.height/2+origin.y - menu_zone_size.height/2);
    sheet_menu->setPosition(ccp(_sheet_target_position.x,
                                 _sheet_target_position.y-visibleSize.height));
    sheet_menu->setContentSize(menu_zone_size);
    sheet_menu->setAnchorPoint(ccp(0,0));


    //sheet->setRotation(-90);
    sheet->setAnchorPoint(ccp(0,0));
}

float PopUpWindowStyle::moveInAnimation(cocos2d::CCNode* sheet_menu)
{
    sheet_menu->stopAllActions();
    sheet_menu->runAction(CCMoveTo::create(0.2f, _sheet_target_position));
}

float PopUpWindowStyle::moveOutAnimation(cocos2d::CCNode* sheet_menu)
{
    sheet_menu->stopAllActions();
    sheet_menu->runAction(CCMoveTo::create(
                              0.3f,
                              ccp(_sheet_target_position.x,
                                  _sheet_target_position.y + visibleSize.height)));
}
