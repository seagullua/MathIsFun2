#include "LevelScene.h"
#include "Scenes/SelectLevel.h"

#include "Layers/EquationDrawer.h"
#include "Layers/LevelStop.h"
#include "Math/Equation.h"
#include "Layers/FoundSolutionsLayer.h"
#include "Scenes/SelectCollection.h"
#include "Logic/RW.h"
#include "Store.h"
#include "Logic/Language.h"

using namespace cocos2d;

class LevelScene::BuyHints : public PopUpWindowStyle
{

public:
    BuyHints()

    {}
private:
    void buyHints(const unsigned int num)
    {
        std::stringstream ss;
        ss << Store::PREFIX_BUY_HINTS << num;
        Store::buyItem(ss.str());
    }

    void onBuy10()
    {
        buyHints(10);
    }
    void onBuy50()
    {
        buyHints(50);
    }
    void onBuy1000()
    {
        buyHints(1000);
    }

    void onOk(CCObject*)
    {
        closeWindow();
    }

    void onCreate(CCNode *parent)
    {
        float scaled = ADScreen::getScaleFactor();

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;
        CCSprite* text = CCSprite::create(Language::localizeFileName("level_scene/hint_title.png").c_str());
        text->setPosition(ccp(x_middle, size.height * 0.65f));
        parent->addChild(text);


        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                               "level_scene/hint_buttons.en.plist",
                                                               "level_scene/hint_buttons.en.png");
        MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
        menu->setPosition(ccp(0,0));
        menu->setAnchorPoint(ccp(0,0));
        menu->setContentSize(size);
        parent->addChild(menu);


        float vertical_pos = 110/scaled;
        CCSprite* lamp = CCSprite::create("level_scene/lamp.png");
        parent->addChild(lamp);
        lamp->setScale(0.8f);

        lamp->setPosition(ccp(200/scaled, 310/scaled));


        ADMenuItem *hint_10_item = ADMenuItem::createWithSpriteSheetSprite(
                    menu_spl->loadSprite("hint_10_button.png"));
        CONNECT(hint_10_item->signalOnClick, this, &BuyHints::onBuy10);

        hint_10_item->setPosition(ccp(200/scaled,
                                        vertical_pos));

        ADMenuItem *hint_50_item = ADMenuItem::createWithSpriteSheetSprite(
                    menu_spl->loadSprite("hint_50_button.png"));
        CONNECT(hint_50_item->signalOnClick, this, &BuyHints::onBuy50);

        hint_50_item->setPosition(ccp(425/scaled,
                                   vertical_pos));

        ADMenuItem *hint_1000_item = ADMenuItem::createWithSpriteSheetSprite(
                    menu_spl->loadSprite("hint_1000_button.png"));
        CONNECT(hint_1000_item->signalOnClick, this, &BuyHints::onBuy1000);

        hint_1000_item->setPosition(ccp(650/scaled,
                                    vertical_pos));

        float scale = 0.95f;
        hint_1000_item->setScaleBase(scale);
        hint_50_item->setScaleBase(scale);
        hint_10_item->setScaleBase(scale);
        menu->menu()->addChild(hint_1000_item);
        menu->menu()->addChild(hint_50_item);
        menu->menu()->addChild(hint_10_item);


    }
};

//TODO: implement starting and the ending animation of the level
LevelScene::LevelScene(Level *level)
    : _level(level),
      _levelStop(0),
      _screenEllements(0),
      _last_pop_up(0),
      _found_solution_is_opened(false),
      _found_solutions(0),
      _top_banner(0)
{
    _last_scene = this;
}
CCScene* LevelScene::scene(Level *level)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    LevelScene *layer = LevelScene::create(level);

    BackgroundHolder::backgroundSwitchTo(scene,0,false);
    scene->addChild(layer);

    // return the scene
    return scene;
}
#include "Layers/LevelScenePopUp.h"
void LevelScene::onBackClick()
{
    if(_found_solutions)
    {
        _found_solutions->hideAndRemove();
        _found_solutions = 0;
        this->runAction(CCSequence::createWithTwoActions(
                            CCDelayTime::create(0.3f),
                            CCCallFunc::create(this, callfunc_selector(LevelScene::onKeyPauseClicked))));
    }
    else if (_levelStop)
    {

        //Just go back to level
        this->showMe();
        _levelStop->onResume();
    }
    else
    {
        //If we in pause state
        if(_found_solution_is_opened)
        {
            //We should hide the found solutions
            //        if(_last_pop_up)
            //        {
            //            _last_pop_up->hideMe(0);
            //        }
            //        else
            //        {
            //TODO: make animation
            do_goToLevelSelect();
            //}
        }
        else
        {
            //Else just go to the collection
            hideMe(true);
            this->runAction(CCSequence::createWithTwoActions(
                                CCDelayTime::create(0.3f),
                                CCCallFunc::create(
                                    this,
                                    callfunc_selector(LevelScene::do_goToLevelSelect)
                                    )
                                )
                            );
        }
    }
}

void LevelScene::do_goToLevelSelect()
{
    CCDirector::sharedDirector()->replaceScene(
                SelectLevel::scene(this->_level->getLevelCollection()));
}

bool LevelScene::init()
{
    if ( !SceneStyle::init() )
    {
        return false;
    }

    //TODO: to change back button appearance
    //To trigger back button
    this->setKeypadEnabled(true);

    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();

    //Get the screen start of cordinates
    CCPoint origin = ADScreen::getOrigin();
    float scaled = ADScreen::getScaleFactor();

    //Calculate the position and the size of the eqution zone
    CCPoint eq_pos = ccp(origin.x+84/scaled,
                         visibleSize.height+origin.y-146/scaled);
    CCSize eq_size = CCSize(967/scaled, eq_pos.y-origin.y-36/scaled);

    //Create equation drawer
    _equation = EquationDrawer::create(_level,eq_size, this, _pop_up_manager);

    //Put equation drawer just in place
    this->addChild(_equation);
    _equation->setAnchorPoint(ccp(0,0));
    _equation->setPosition(eq_pos);

    _equation->setOpacity(0);
    CCFadeTo* hide_equation = CCFadeTo::create(0.9f, 255);
    _equation->runAction(hide_equation);



    _play = ADMenuItem::create(
                CCSprite::create("level_scene/play_button.png"));
    CONNECT(_play->signalOnClick, this, &LevelScene::keyPauseClicked);

    _pause = ADMenuItem::create(
                CCSprite::create("level_scene/stop_button.png"));
    CONNECT(_pause->signalOnClick, this, &LevelScene::keyPauseClicked);

    _restart= ADMenuItem::create(
                CCSprite::create("level_scene/restart_button.png"));
    CONNECT(_restart->signalOnClick, this, &LevelScene::keyRestartClicked);

    _hint= ADMenuItem::create(
                CCSprite::create("level_scene/lamp_hint.png"));
    CONNECT(_hint->signalOnClick, this, &LevelScene::keyHintClicked);


    _screenEllements = CCMenu::create();
    _screenEllements->addChild(_play);
    _screenEllements->addChild(_pause);
    _screenEllements->addChild(_restart);
    _screenEllements->addChild(_hint);
    _pop_up_manager.addMenuToAutoDisable(_screenEllements);
    this->addChild(_screenEllements);

    _screenEllements->setPosition(ccp(visibleSize.width/2,
                                      visibleSize.height/2 ));
    _play->setPosition(ccp(origin.x - visibleSize.width/2 +120/scaled,
                           origin.y + visibleSize.height/2 -100/scaled ));
    _pause->setPosition(ccp(origin.x - visibleSize.width/2 +120/scaled,
                            origin.y + visibleSize.height/2 -100/scaled ));
    _restart->setPosition(ccp(origin.x + visibleSize.width/2 -500/scaled,
                              origin.y + visibleSize.height/2 -100/scaled ));

//Ads
    float top_zone_end = origin.x + visibleSize.width - 530/scaled - _restart->getContentSize().width / 2;
    float top_zone_start = origin.x + 150/scaled + _play->getContentSize().width/2;
    float top_zone_height = _play->getContentSize().height * 1.3f;

    CCSize top_zone(top_zone_end - top_zone_start, top_zone_height);
    _top_banner = ADAds::getBanner(top_zone);
    if(_top_banner != 0)
    {
        this->addChild(_top_banner);
        _top_banner->setAnchorPoint(ccp(0.5, 0.5));
        _top_banner->setPositionX(top_zone_start + top_zone.width/2);
        _top_banner->setPositionY(origin.y + visibleSize.height - 98.5f/scaled);
        //_top_banner->showAds();
        //_pop_up_manager.addBannerToAutoDisable(_top_banner);
    }

//Ads end
//#ifndef TEMPORARY_HIDE
    _hint->setPosition(ccp(origin.x + visibleSize.width/2 -120/scaled,
                           origin.y + visibleSize.height/2 -100/scaled ));
//#endif

    //we create the play button and now show it
    _play->setVisible(false);


    _pause->setOpacity(0);
    CCFadeTo* pause_show = CCFadeTo::create(0.3f, 255);
    _pause->runAction(pause_show);

    _restart->setOpacity(0);
    CCFadeTo* restart_show = CCFadeTo::create(0.5f, 255);
    _restart->runAction(restart_show);
//#ifndef TEMPORARY_HIDE
    _hint->setOpacity(0);
    CCFadeTo* hint_show = CCFadeTo::create(0.5f, 255);
    _hint->runAction(hint_show);
//#endif


    //get the full number of collection levels
    Collection* col =_level->getLevelCollection();
    std::stringstream col_num;
    col_num << col->getLevels().size();
    std::string real_col_num(col_num.str());

    //show number of the level
    std::stringstream level_num;
    level_num << static_cast<unsigned int>((_level->getLevelID()+1));
    std::string cur_level_num(level_num.str());
    cur_level_num+="/";
    cur_level_num+=real_col_num;

    //show the quantity of hints
    std::stringstream hint_num;
    hint_num << (RW::getHintCount());
    std::string cur_hint_num(hint_num.str());

    _hint_quantity = CCLabelBMFont::create(cur_hint_num.c_str(),
                                           "font/mathisfun.fnt");
    _hint_quantity->setColor(ccc3(189, 110, 53));
    float hint_quantity_target_hights=50/scaled;
    float hint_quantity_real_hights= _hint_quantity->getContentSize().height;
    _hint_quantity->setAnchorPoint(ccp(0.5,0.5));
    _hint_quantity->setPosition(ccp(origin.x + visibleSize.width - 120/scaled,
                                 origin.y + visibleSize.height - 180/scaled));
    // _level_from->setPosition(ccp(50, 50));
    _hint_quantity->setScale(hint_quantity_target_hights/hint_quantity_real_hights);

    _hint_quantity->setOpacity(0);
    CCFadeTo* hint_quantity_show = CCFadeTo::create(0.5f, 255);
    _hint_quantity->runAction(hint_quantity_show);
    this->addChild(_hint_quantity);




    //show the current number of the level we are playing from X levels of the Collection
    _level_from = CCLabelBMFont::create(cur_level_num.c_str(),
                                        "font/mathisfun.fnt");
    _level_from->setColor(ccc3(112, 106, 107));
    float label_target_width=230/scaled;
    float label_real_width = _level_from->getContentSize().width;
    _level_from->setAnchorPoint(ccp(1,0.5));
    _level_from->setPosition(ccp(origin.x + visibleSize.width - 220/scaled,
                                 origin.y + visibleSize.height - 100/scaled));
    // _level_from->setPosition(ccp(50, 50));
    _level_from->setScale(label_target_width/label_real_width);

    _level_from->setOpacity(0);
    CCFadeTo* level_from_show = CCFadeTo::create(0.5f, 255);
    _level_from->runAction(level_from_show);


    this->addChild(_level_from);


    return true;
}
void LevelScene::removeChild(CCNode* child, bool cleanup)
{
    CCNode::removeChild(child, cleanup);
    if (_levelStop==child)
    {
        _levelStop=0;
    }
}
void LevelScene::keyPauseClicked()
{
    onKeyPauseClicked();
}
void LevelScene::onKeyPauseClicked()
{
    if(_found_solutions)
    {
        _found_solutions->hideAndRemove();
        _found_solutions = 0;
        this->showMe();
        _equation->wakeup();
    }
    else if (!_levelStop)
    {
        this->hideMe();

        _levelStop = LevelStop::create(_equation,_level,this);
        this->addChild(_levelStop);
    }
    else
    {
        this->showMe();
        _levelStop->onResume();

    }
}
void LevelScene::keyHintClicked()
{

    ADStatistics::logEvent("Hint used",
                         ADStatistics::Params()
                         .add("Hints left", RW::getHintCount()));

    bool found = false;
    if (RW::getHintCount()==0)
    {
        //show shop
        _pop_up_manager.openWindow(new BuyHints);
    }
    else
    {
        Solutions all_solution = _level->getSolutions();
        for (unsigned int i=0; i<all_solution.size()&& found==false; ++i)
        {
            if (!_level->isSolutionFound(all_solution[i]))
            {
                _hint_solution = all_solution[i];
                found = true;
                break;

            }
        }
        if (found==false)
            return;


        found=false;
        unsigned int substitution_number=0;
        for (unsigned int i=0; i<_hint_solution.getSubstitutions().size() && found==false; ++i)
        {
            //look for place where the user puts uncorrect digit
            //get the user symbol of zero`s, first... place
            const Symbol current_symbol = _equation->getEquation().getSubstitutedSymbol(i);

            if (current_symbol.toString()!=_hint_solution.getSubstitutions()[i])
            {
                //if the user puts uncorrect symbol - we change it to the correct one
                substitution_number=i;
                found=true;
                break;
            }
        }


        //show solution
        _equation->substituteAnything(_hint_solution.getSubstitutions()[substitution_number],
                                   substitution_number);

        //--hints
        RW::useOneHint();
        renewOneHint();
    }
}

void LevelScene::renewOneHint()
{
    //show the quantity of hints
    std::stringstream hint_num;
    hint_num << (RW::getHintCount());
    std::string cur_hint_num(hint_num.str());

    _hint_quantity->setString(cur_hint_num.c_str());
}

void LevelScene::keyRestartClicked()
{
    hideMe(true);
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(0.3f),
                    CCCallFunc::create(this, callfunc_selector(LevelScene::onkeyRestartClicked)),
                    NULL));

    //onkeyRestartClicked();
}
void LevelScene::onkeyRestartClicked()
{
    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_level));
    //_levelStop = LevelStop::create(_equation,_level,this);
    //_levelStop->onRestart(0);
    //_levelStop->selfDestroy();
}
void LevelScene::showPause()
{
    _play->setOpacity(255);
    _play->setEnabled(true);
    _play->setVisible(false);

    _pause->setEnabled(true);
    _pause->runAction(CCFadeTo::create(0.3f, 255));
}

void LevelScene::hidePauseAndPlay()
{
    _play->runAction(CCFadeTo::create(0.3f, 0));
    _play->setEnabled(false);
    _pause->runAction(CCFadeTo::create(0.3f, 0));
    _pause->setEnabled(false);
}

void LevelScene::hideMe(bool hide_all)
{
    if(_top_banner)
    {
        _top_banner->hideAds();
    }
    _last_scene = 0;
    _equation->sleep();
    if(!hide_all)
    {
        //CCFadeTo* pause_hide = CCFadeTo::create(0.3f, 0);
        //_pause->runAction(pause_hide);
        _pause->setVisible(false);

        //CCFadeTo* play_show = CCFadeTo::create(0.3f, 255);
        //_play->runAction(play_show);
        _play->setVisible(true);
    }
    else
    {
        hidePauseAndPlay();
    }

    CCFadeTo* restart_hide = CCFadeTo::create(0.3f, 0);
    _restart->stopAllActions();
    _restart->runAction(restart_hide);
    _restart->setEnabled(false);
    //_restart->setVisible(false);

    CCFadeTo* level_from_hide = CCFadeTo::create(0.3f, 0);
    _level_from->stopAllActions();
    _level_from->runAction(level_from_hide);
    //_level_from->setVisible(false);
//#ifndef TEMPORARY_HIDE
    CCFadeTo* hint_hide = CCFadeTo::create(0.3f, 0);
    _hint->stopAllActions();
    _hint->runAction(hint_hide);
    _hint->setEnabled(false);
//#endif
    //_hint->setVisible(false);
    CCFadeTo* hint_quantity_hide = CCFadeTo::create(0.3f, 0);
    _hint_quantity->stopAllActions();
    _hint_quantity->runAction(hint_quantity_hide);


}
void LevelScene::showMe()
{
    if(_top_banner)
    {
        this->runAction(CCSequence::createWithTwoActions(
                            CCDelayTime::create(0.3f),
                            CCCallFunc::create(_top_banner,
                                               callfunc_selector(
                                                   ADAds::Banner::showAds))));
    }
    _last_scene = this;
    //CCFadeTo* play_hide = CCFadeTo::create(0.3f, 0);
    //_play->runAction(play_hide);
    _play->setVisible(false);

    CCFadeTo* restart_show = CCFadeTo::create(0.5f, 255);
    _restart->stopAllActions();
    _restart->runAction(restart_show);
    _restart->setEnabled(true);
    //_restart->setVisible(true);

    CCFadeTo* level_from_show = CCFadeTo::create(0.5f, 255);
    _level_from->stopAllActions();
    _level_from->runAction(level_from_show);
    //_level_from->setVisible(true);
//#ifndef TEMPORARY_HIDE
    CCFadeTo* hint_show = CCFadeTo::create(0.5f, 255);
    _hint->stopAllActions();
    _hint->runAction(hint_show);
    _hint->setEnabled(true);
//#endif

    CCFadeTo* hint_quantity_show = CCFadeTo::create(0.5f, 255);
    _hint_quantity->stopAllActions();
    _hint_quantity->runAction(hint_quantity_show);

    //_hint->setVisible(true);

    //CCFadeTo* pause_show = CCFadeTo::create(0.3f, 255);
    //_pause->runAction(pause_show);
    _pause->setVisible(true);

}
void LevelScene::skipLevel(CCObject*)
{    

    hideMe(true);
    this->runAction(CCSequence::createWithTwoActions(
                        CCDelayTime::create(0.3f),
                        CCCallFunc::create(this, callfunc_selector(LevelScene::do_skipLevel))
                        ));
}

void LevelScene::levels()
{
    hideMe(true);
    this->runAction(CCSequence::createWithTwoActions(
                        CCDelayTime::create(0.3f),
                        CCCallFunc::create(this, callfunc_selector(LevelScene::do_levels))
                        ));
}
void LevelScene::restart()
{
    hideMe(true);
    this->runAction(CCSequence::createWithTwoActions(
                        CCDelayTime::create(0.3f),
                        CCCallFunc::create(this, callfunc_selector(LevelScene::do_restart))
                        ));
}

void LevelScene::do_restart()
{
    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_level));
}

void LevelScene::do_skipLevel()
{
    Level* next = RW::skipLevel(_level);
    if(next)
        CCDirector::sharedDirector()->replaceScene(LevelScene::scene(next));
    else
        CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
}

void LevelScene::do_levels()
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene
                                               (_level->getLevelCollection()));
}

void LevelScene::showFoundSolutions()
{

    _found_solutions = FoundSolutionsLayer::create(_level);
    this->addChild(_found_solutions);
}
LevelScene* LevelScene::_last_scene = 0;
void LevelScene::purchaseUpdateHints()
{
    if(_last_scene)
    {
        _last_scene->renewOneHint();
        _last_scene->_pop_up_manager.closeWindow();
    }
}
