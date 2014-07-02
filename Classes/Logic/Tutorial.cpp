#include "Tutorial.h"
#include "Level.h"
#include "Collection.h"
#include "Layers/EquationDrawer.h"
#include "Layers/LevelKeyboard.h"
#include "Layers/LevelScenePopUp.h"
#include "cocos2d-A.h"
#include "Logic/Language.h"
#include "GameInfo.h"
using namespace cocos2d;


void Tutorial::onLevelStart(EquationDrawer* eqd)
{
    Level* level = eqd->getLevel();
    if(_initialized && level)
    {

        LevelKeyboard* keyboard = eqd->getLevelKeyboard();
        ADMenuItem* empty_space = eqd->getSubstituteItems()[0];
        float empty_space_position = empty_space->getPositionX();
        float empty_space_width = empty_space->getContentSize().width;
        float empty_space_scale = eqd->getEquaionLabelScale();

        //First collection
        if(level->getLevelCollection()->getCollectionID() ==
                GameInfo::TUTORIAL_COLLECTIONID_FIRST &&
                level->getLevelID() == GameInfo::TUTORIAL_LEVELID_FIRST)
        {

            float x = empty_space_position + empty_space_width*empty_space_scale*0.5f;
            float y = keyboard->getPositionY();
            _help_arrow = CCSprite::create("level/tutorial/tutorial.png");
            _help_arrow->setAnchorPoint(ccp(0.5,1));
            _help_arrow->setPosition(ccp(x,y));
            eqd->addChild(_help_arrow);

            //start animation -up-down
            float SCALE = ADScreen::getScaleFactor();
            float duration = 0.2f;
            CCPoint delta_position = ccp(0,20/SCALE);
            _help_arrow->runAction(CCRepeatForever::create(
                                       CCSequence::create(
                                           CCMoveBy::create(duration,delta_position),
                                           CCMoveBy::create(duration*2,delta_position*(-2)),
                                           CCMoveBy::create(duration,delta_position),
                                           NULL)));


        }
        else if(level->getLevelCollection()->getCollectionID() ==
                GameInfo::TUTORIAL_COLLECTIONID_SECOND &&
                level->getLevelID() == GameInfo::TUTORIAL_LEVELID_SECOND)
        {
            float x = empty_space_position + empty_space_width*empty_space_scale*0.5f;
            float y = keyboard->getPositionY();
            _help_arrow = CCSprite::create("level/tutorial/tutorial.png");
            _help_arrow->setAnchorPoint(ccp(0.5,1));
            _help_arrow->setPosition(ccp(x,y));
            eqd->addChild(_help_arrow);

            //start animation -up-down
            float SCALE = ADScreen::getScaleFactor();
            float duration = 0.2f;
            CCPoint delta_position = ccp(0,20/SCALE);
            _help_arrow->runAction(CCRepeatForever::create(
                                       CCSequence::create(
                                           CCMoveBy::create(duration,delta_position),
                                           CCMoveBy::create(duration*2,delta_position*(-2)),
                                           CCMoveBy::create(duration,delta_position),
                                           NULL)));
        }
    }
}
void Tutorial::hideLabels()
{
//    if(_help_arrow)
//    {
//        _help_arrow->runAction(CCFadeOut::create(0.2f));
//    }
}

void Tutorial::showLabels()
{
//    if(_help_arrow)
//    {
//        _help_arrow->runAction(CCFadeIn::create(0.2f));
//    }
}

void Tutorial::onEmptySpaceTouch(EquationDrawer* eqd)
{


    //First collectin


    //add animation to '2'
    LevelKeyboard* keyboard = eqd->getLevelKeyboard();
    Level* level = eqd->getLevel();

    if(level->getLevelCollection()->getCollectionID() ==
            GameInfo::TUTORIAL_COLLECTIONID_FIRST &&
            level->getLevelID() == GameInfo::TUTORIAL_LEVELID_FIRST)
    {
        //hide labels
        _help_arrow->runAction(CCFadeOut::create(0.2f));

        keyboard->startNumberAnimation(LevelKeyboard::ButtonsCodes::b2);
    }
    else if(level->getLevelCollection()->getCollectionID() ==
            GameInfo::TUTORIAL_COLLECTIONID_SECOND &&
            level->getLevelID() == GameInfo::TUTORIAL_LEVELID_SECOND)
    {
        //hide labels
        _help_arrow->runAction(CCFadeOut::create(0.2f));

        keyboard->startNumberAnimation(LevelKeyboard::ButtonsCodes::b5);
    }
}

Tutorial::Tutorial()
    : _initialized(false), _help_arrow(0)
{

}
Tutorial Tutorial::_tutorial;
void Tutorial::init()
{
    _initialized = true;
}
