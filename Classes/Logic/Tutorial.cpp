#include "Tutorial.h"
#include "Level.h"
#include "Collection.h"
#include "Layers/EquationDrawer.h"
#include "Layers/LevelKeyboard.h"
#include "Layers/LevelScenePopUp.h"
#include "cocos2d-A.h"
#include "Logic/Language.h"
void Tutorial::onLevelStart(EquationDrawer* eqd)
{
    Level* level = eqd->getLevel();
    if(_initialized && level)
    {

        //First collection
        if(level->getLevelCollection()->getCollectionID() == 100)
        {
            unsigned int level_id = level->getLevelID();
            //First level, first try
            if(level_id <= 1
                    && level->getFoundSolutions().size() == 0)
            {

                LevelKeyboard* keyboard = eqd->getLevelKeyboard();
                CCSprite* label = 0;
                if(level_id == 0)
                    label = CCSprite::create("level/tutorial/touch_empty_space.png");
                else
                    label = CCSprite::create("level/tutorial/touch_empty_space2.png");
                eqd->addChild(label);
                ADMenuItem* empty_space = eqd->getSubstituteItems()[0];
                float empty_space_position = empty_space->getPositionX();
                float empty_space_width = empty_space->getContentSize().width;
                float empty_space_scale = eqd->getEquaionLabelScale();
                label->setPosition(
                            ccp(
                                (empty_space_position + empty_space_width*empty_space_scale*0.5f),
                                keyboard->getPositionY()));
                if(level_id == 0)
                    label->setAnchorPoint(ccp(0.5f, 1));
                else
                    label->setAnchorPoint(ccp(149.0f / 404.0f, 1));

                switchLabel1To(label);
                switchLabel2To(0);

            }
        }
    }
}
void Tutorial::hideLabels()
{
    if(_help_label1)
        _help_label1->runAction(CCFadeOut::create(0.3f));
    if(_help_label2)
        _help_label2->runAction(CCFadeOut::create(0.3f));
}

void Tutorial::showLabels()
{
    if(_help_label1)
        _help_label1->runAction(CCFadeIn::create(0.3f));
    if(_help_label2)
        _help_label2->runAction(CCFadeIn::create(0.3f));
}

void Tutorial::onEmptySpaceTouch(EquationDrawer* eqd)
{
    Level* level = eqd->getLevel();
    if(_initialized && level)
    {
        //First collection
        if(level->getLevelCollection()->getCollectionID() == 100)
        {
            unsigned int level_id = level->getLevelID();
            //First touch to the empty space
            if(_help_label1 && level_id <= 1
                    && level->getFoundSolutions().size() == 0)
            {
                _help_label1->setVisible(false);
                switchLabel1To(0);

                LevelKeyboard* keyboard = eqd->getLevelKeyboard();
                CCPoint button_pos;
                CCSize button_size;
                CCSprite* label;

                if(level_id == 0)
                {
                    button_pos = keyboard->getButtonPosition(LevelKeyboard::b3);
                    button_size = keyboard->getButtonSizeScaled(LevelKeyboard::b3);
                    label = CCSprite::create("level/tutorial/select_digit.png");
                }
                else
                {
                    button_pos = keyboard->getButtonPosition(LevelKeyboard::multiply);
                    button_size = keyboard->getButtonSizeScaled(LevelKeyboard::multiply);
                    label = CCSprite::create("level/tutorial/select_digit2.png");
                }
                label->setPositionX(button_pos.x + keyboard->getPositionX() + button_size.width*0.7f);
                label->setPositionY(button_pos.y + keyboard->getPositionY() - keyboard->getContentSize().height);
                label->setAnchorPoint(ccp(0.0f, 0.5f));
                eqd->addChild(label);

                switchLabel2To(label);
            }
        }
    }
}
void Tutorial::onSubstitutionMade(EquationDrawer* eqd)
{
    Level* level = eqd->getLevel();
    if(_initialized && level)
    {
        //First collection
        if(level->getLevelCollection()->getCollectionID() == 100)
        {
            //First substitution
            if(level->getLevelID() == 0
                    && level->getFoundSolutions().size() == 0)
            {
                //First substiotution
                if(_help_label2)
                {
                    _help_label2->setVisible(false);
                    switchLabel2To(0);
                }
            }
        }
    }
}
void Tutorial::onSolutionFound(LevelScenePopUp* pop_up)
{
    Level* level = pop_up->getEquationDrawer()->getLevel();
    if(_initialized && level)
    {
        unsigned int level_id = level->getLevelID();
        unsigned int found_solutions = level->getFoundSolutions().size();
        unsigned int col_id = level->getLevelCollection()->getCollectionID();
        //First collection
        if((col_id == 100
            && level_id == 0) ||
                (col_id == 100 && level_id == 1 && found_solutions == 2))
        {
            CCSprite* crown = pop_up->getCrown();
            if(crown)
            {
                CCSprite* label = CCSprite::create(Language::localizeFileName("level/tutorial/you_have_found_all.png").c_str());
                label->setAnchorPoint(ccp(0,1));
                CCNode* stamps_zone = crown->getParent()->getParent();
                CCPoint p_pos = stamps_zone->getPosition();
                CCPoint sz_anchor = stamps_zone->getAnchorPoint();
                CCSize sz_size = stamps_zone->getContentSize();
                p_pos.x -= sz_size.width * sz_anchor.x;
                //p_pos.y -= sz_size.height * sz_anchor.y;

                label->setPosition(ccp(crown->getPositionX() + p_pos.x + crown->getContentSize().width * 0.5f * crown->getScale(),
                                       crown->getPositionY() + p_pos.y));
                if(level_id == 1)
                    label->setScale(0.6f);
                pop_up->addChild(label);
            }

        }
        else if(col_id == 100 && level_id == 1 && found_solutions == 1)
        {
            float scaled = ADScreen::getScaleFactor();
            CCSprite* label = CCSprite::create(Language::localizeFileName("level/tutorial/final_step.png").c_str());
            label->setAnchorPoint(ccp(0,0));
            label->setPosition(ccp(85/scaled, 208/scaled));
            pop_up->addChild(label);

            if(_help_label2)
            {
                _help_label2->runAction(
                            CCSequence::createWithTwoActions(
                                CCFadeTo::create(0.3f, 0),
                                CCCallFunc::create(
                                    this,
                                    callfunc_selector(Tutorial::disposeLabel2)
                                    )
                                )
                            );
            }
        }
    }
}
void Tutorial::disposeLabel2()
{
    disposeLabel(_help_label2);
}

void Tutorial::disposeLabel1()
{
    disposeLabel(_help_label1);
}

void Tutorial::disposeLabel(CCSprite *& what)
{
    switchLabel(what, 0);
}

void Tutorial::switchLabel1To(CCSprite* l)
{
    switchLabel(_help_label1, l);
}

void Tutorial::switchLabel2To(CCSprite* l)
{
    switchLabel(_help_label2, l);
}

void Tutorial::switchLabel(CCSprite*& what, CCSprite* to)
{
    if(what)
    {
        //what->setVisible(false);
        what->release();
        what = 0;
    }

    if(to)
    {
        what = to;
        what->retain();
    }
}

Tutorial::Tutorial()
    : _initialized(false), _help_label1(0), _help_label2(0)
{

}
Tutorial Tutorial::_tutorial;
void Tutorial::init()
{
    _initialized = true;
}
