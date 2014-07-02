#include "EquationDrawer.h"
#include "DebugLayer.h"
#include "Logic/RW.h"
#include "LevelScenePopUp.h"
#include "Logic/Tutorial.h"
#include "Scenes/LevelScene.h"
#include "Logic/Language.h"
#include "GameInfo.h"

using namespace cocos2d;
const float EquationDrawer::_paper_cell_size = 35.5f;
const ccColor3B EquationDrawer::EquationColor_NewSolution = ccc3(79,213,6);
const ccColor3B EquationDrawer::EquationColor_OldSolution = ccc3(255,255,6);
const ccColor3B EquationDrawer::EquationColor_HintSolution = ccc3(213,6,26);

EquationDrawer* EquationDrawer::create(
        Level* level,
        const CCSize &zone,
        LevelScene *parent,
        ADPopUpWindowManager& pop_up_m)
{
    EquationDrawer *pRet = new EquationDrawer(level,zone,parent,pop_up_m);
    if (pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

EquationDrawer::KeyboardReactor::KeyboardReactor(EquationDrawer* parent)
    : _p(parent)
{
}
LevelKeyboard* EquationDrawer::getLevelKeyboard()
{
    return _keyboard;
}

void EquationDrawer::sleep()
{
    _keyboard->sleep();
    if(_substitutions)
        _substitutions->menu()->setTouchEnabled(false);
    this->stopAllActions();
    this->setOpacity(255);
    this->runAction(CCSequence::create(
                        CCFadeTo::create(0.3f, 0),
                        CCCallFunc::create(this, callfunc_selector(EquationDrawer::finishHide)),
                        NULL));

    Tutorial::getInstance()->hideLabels();

    hideMessageNode();
}
//void EquationDrawer::setOpacity(GLubyte op)
//{
//    CCNodeWithOpacity::setOpacity(op);
//    _substitutions->menu()->setOpacity(op);
//    _eq_lab->setOpacity(op);
//    _keyboard->setOpacity(op);
//    _inserted_label->setOpacity(op);
//    if(!RW::isExpertMode())
//        _draft_label->setOpacity(op);
//}

void EquationDrawer::finishHide()
{
    this->setVisible(false);
}
void EquationDrawer::finishShow()
{
    //this->setVisible(false);
    _keyboard->wakeup();
    if(_substitutions)
        _substitutions->menu()->setTouchEnabled(true);
}

void EquationDrawer::wakeup()
{
    this->setVisible(true);
    this->setOpacity(0);
    this->stopAllActions();
    Tutorial::getInstance()->showLabels();
    this->runAction(CCSequence::createWithTwoActions(
                        CCFadeTo::create(0.3f, 255),
                        CCCallFunc::create(this, callfunc_selector(EquationDrawer::finishShow))));
}

void EquationDrawer::KeyboardReactor::onKeyPressed(const LevelKeyboard::ButtonsCodes& b)
{
    switch(b)
    {
    case LevelKeyboard::b0:
        _p->substituteDigit(0,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b1:
        _p->substituteDigit(1,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b2:
        _p->substituteDigit(2,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b3:
        _p->substituteDigit(3,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b4:
        _p->substituteDigit(4,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b5:
        _p->substituteDigit(5,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b6:
        _p->substituteDigit(6,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b7:
        _p->substituteDigit(7,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b8:
        _p->substituteDigit(8,_p->_substituting_now_id);
        break;
    case LevelKeyboard::b9:
        _p->substituteDigit(9,_p->_substituting_now_id);
        break;
    case LevelKeyboard::add:
        _p->substituteOperation(Symbol::Add,_p->_substituting_now_id);
        break;
    case LevelKeyboard::multiply:
        _p->substituteOperation(Symbol::Multiply,_p->_substituting_now_id);
        break;
    case LevelKeyboard::substract:
        _p->substituteOperation(Symbol::Substract,_p->_substituting_now_id);
        break;
    case LevelKeyboard::left_parenthesis:
        _p->substituteParenthesis(Symbol::Left,_p->_substituting_now_id);
        break;
    case LevelKeyboard::right_parenthesis:
        _p->substituteParenthesis(Symbol::Right,_p->_substituting_now_id);
        break;
    }
}
EquationDrawer::~EquationDrawer()
{
    delete _keyboard_reactor;
}

void EquationDrawer::initKeyboard()
{

    //Put keyboard
    float from_top = _eq_lab->getContentSize().height *
            _eq_lab->getScaleY() + _zone.height/20;
    float height = _zone.height - from_top;
    _keyboard = LevelKeyboard::create(CCSize(_zone.width/5*3,height), _pop_up_manager);
    this->addChild(_keyboard);
    _keyboard_reactor = new KeyboardReactor(this);
    _keyboard->setListener(_keyboard_reactor);
    _keyboard->setAnchorPoint(ccp(0,1));
    _keyboard->setPosition(0, -from_top);
}
void EquationDrawer::onFoundSolutionClose()
{
    _parent->onFoundSolutionClose();
}

const std::vector<ADMenuItem*>& EquationDrawer::getSubstituteItems()
{
    return _substitutors;
}
float EquationDrawer::getEquaionLabelScale()
{
    return _eq_lab->getScale();
}

void EquationDrawer::initEquationLabel()
{
    //Put equation
    _raw_equation = _equation.toStringRaw();
    _eq_lab = CCLabelBMFont::create(_raw_equation.c_str(),
                                    "font/mathisfun.fnt");
    //CCSize box_size = this->getContentSize();
    _eq_lab->setColor(ccc3(70, 148, 255));
    _eq_lab->setAnchorPoint(ccp(0,1));
    _eq_lab->setPosition(ccp(0, 0));
    this->addChild(_eq_lab);

    float symbol_length = _eq_lab->getContentSize().width /
            _raw_equation.size();
    float symbol_height = _eq_lab->getContentSize().height;
    _font_symbol_height = symbol_height;
    float max_height = _zone.height / 4;

    float target_eq_length = _zone.width;
    float symbol_scale = target_eq_length / _eq_lab->getContentSize().width;
    symbol_height *= symbol_scale;

    if(symbol_height > max_height)
    {
        symbol_scale = max_height / _eq_lab->getContentSize().height;
        symbol_height = _eq_lab->getContentSize().height * symbol_scale;
    }

    _eq_lab->setScale(symbol_scale);
    symbol_length *= symbol_scale;



    SpritesLoader col_spl = GraphicsManager::getLoaderFor(
                0,
                "level/input_space.plist",
                "level/input_space.png");

//    _selected_sprite = col_spl->loadSprite("selected.png");
//    _selected_sprite->setVisible(false);

    _substitutions = MenuSpriteBatch::create(col_spl);
    this->addChild(_substitutions);

    _pop_up_manager.addMenuToAutoDisable(_substitutions->menu());

    _substitutions->setPosition(
                ccp(_eq_lab->getPositionX()+symbol_length/2,
                    _eq_lab->getPositionY()-symbol_height/2));

    _substitutors.reserve(_equation.substitutionsNumber());
    _substitutors_index.reserve(_equation.substitutionsNumber());
    unsigned int sub_id = 0;
    for(unsigned int i=0; i<_raw_equation.size(); ++i)
    {
        if(_raw_equation[i]=='_')
        {
            unsigned int next_id = _substitutors.size();
            Symbol::PossibleType type = _equation.getSubstitutedSymbol(next_id).getPossibleType();
            CCSprite* sp = 0;
            if(type == Symbol::PDigit)
                sp = col_spl->loadSprite("digit.png");
            else if(type == Symbol::PParenthesis)
                sp = col_spl->loadSprite("parenthesis.png");
            else if(type == Symbol::POperation)
                sp = col_spl->loadSprite("operation.png");
            else if(type == Symbol::PDigitOrOperation)
                sp = col_spl->loadSprite("dig_op.png");
            else if(type == Symbol::PParenthesisOrDigit)
                sp = col_spl->loadSprite("dig_par.png");
            else if(type == Symbol::PParenthesisOrOperation)
                sp = col_spl->loadSprite("op_par.png");
            else
                sp = col_spl->loadSprite("any.png");

            auto click_action = [this, sub_id](){
                this->onFreeSpacePressed(sub_id);
            };


            CCSprite* selected = col_spl->loadSprite("selected.png");
            selected->setVisible(false);



            CCLog("iTag: %d", selected->getTag());
            ADMenuItem* item = ADMenuItem::createWithSpriteSheetSprite(
                        sp,click_action);

            Pair p;
            p.normal = sp;
            p.selected = selected;
            _substitutions_background[item] = p;

            item->addNephew(selected);
            item->setPosition(ccp(symbol_length*i,0));
            item->setScaleBase(_eq_lab->getScale());

            _substitutions->menu()->addChild(item);
            _substitutors.push_back(item);
            _substitutors_index.push_back(i);
            sub_id++;

        }
    }


    _inserted_label_str = std::string(_raw_equation.size(), ' ');
    _hint_inserted_label_str = std::string(_raw_equation.size(), ' ');


    //Substitution
    _inserted_label = CCLabelBMFont::create(_inserted_label_str.c_str(),
                                            "font/mathisfun.fnt");
    this->addChild(_inserted_label);
    //_inserted_label->setColor(ccc3(255,99,73));
    _inserted_label->setColor(_eq_lab->getColor());
    _inserted_label->setPosition(_eq_lab->getPosition());
    _inserted_label->setAnchorPoint(_eq_lab->getAnchorPoint());
    _inserted_label->setScale(_eq_lab->getScale());


    _hint_inserted_label = CCLabelBMFont::create(_hint_inserted_label_str.c_str(),
                                                 "font/mathisfun.fnt");
    this->addChild(_hint_inserted_label);
    _hint_inserted_label->setColor(ccc3(255,1,1));
    //_inserted_label->setColor(_eq_lab->getColor());
    _hint_inserted_label->setPosition(_eq_lab->getPosition());
    _hint_inserted_label->setAnchorPoint(_eq_lab->getAnchorPoint());
    _hint_inserted_label->setScale(_eq_lab->getScale());
}
void EquationDrawer::initDraft()
{
    if(!RW::isExpertMode())
    {
        //Draft
        float scaled = ADScreen::getScaleFactor();
        _draft.reserve(500);
        _draft_label = CCLabelBMFont::create(_draft.c_str(),
                                             "font/mathisfun.fnt");
        this->addChild(_draft_label);

        _draft_label->setColor(_eq_lab->getColor());
        float keyboard_y = _keyboard->getPositionY();
        float from_top = - keyboard_y;
        unsigned int cells = static_cast<unsigned int>(from_top / _paper_cell_size * scaled);
        float new_from_top = cells * _paper_cell_size / scaled;
        if(new_from_top < from_top)
        {
            new_from_top+=_paper_cell_size / scaled;
        }


        _draft_label->setPosition(_keyboard->getContentSize().width +
                                  _paper_cell_size/scaled,
                                  - new_from_top);


        _draft_label->setScale(2*_paper_cell_size/scaled/_font_symbol_height);
        _draft_label->setAlignment(kCCTextAlignmentLeft);
        _draft_label->setAnchorPoint(ccp(0,1));

        _max_draft_size.width = 1110/scaled - _draft_label->getPositionX();
        _max_draft_size.height = _zone.height + _draft_label->getPositionY();
    }
}
LevelScene* EquationDrawer::getParentScene()
{
    return _parent;
}

EquationDrawer::EquationDrawer(
        Level *level,
        const CCSize& z,
        LevelScene *parent,
        ADPopUpWindowManager &pop_up_m)
    : _equation(level->getEquation()),
      _level(level),
      _keyboard(0),
      _keyboard_reactor(0),
      _substituting_now_id(level->getEquation().substitutionsNumber()),
      _zone(z),
      _parent(parent),
      _no_more_solutions_shown(false),
      _duplicate_solution_shown(false),
      _message_node(0),
      _pop_up_manager(pop_up_m)
{
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(_zone);
    //

    this->initEquationLabel();
    this->initKeyboard();
    this->initDraft();
    this->setOpacity(0);
    this->runAction(CCFadeTo::create(0.3f, 255));

    Tutorial::getInstance()->onLevelStart(this);

    recalculateEquation();

    this->initNoMoreSolutions();
    //#define DEBUG_LAYER
    //DEBUG ZONE START
#ifdef DEBUG_LAYER
    DebugLayer* debug = DebugLayer::create();
    this->addChild(debug);

    debug->addChild(_eq_lab);
    debug->addChild(_keyboard);
    debug->addChild(_draft_label);
    CCNode* test_rec = CCNode::create();
    test_rec->setPosition(ccp(0,0));
    test_rec->setAnchorPoint(ccp(0,1));
    test_rec->setContentSize(this->getContentSize());

    debug->addChild(test_rec);
#endif
    //DEBUG ZONE END


}
void EquationDrawer::updateDraft()
{
    if(!RW::isExpertMode())
    {
        float scaled = ADScreen::getScaleFactor();
        float x1_scale = _paper_cell_size/scaled/_font_symbol_height;
        float x2_scale = x1_scale*2;
        _draft_label->setCString(_draft.c_str());
        CCSize draft_size = _draft_label->getContentSize();
        draft_size.width *= x2_scale;
        draft_size.height *= x2_scale;


        float scale = MIN(x2_scale, x2_scale*_max_draft_size.width / draft_size.width);

        if(_draft_label->getScale() != scale)
            _draft_label->setScale(scale);

    }
}
void EquationDrawer::showUI()
{
    _parent->showMe();
    _parent->showPause();
}

void EquationDrawer::switchImages(ADMenuItem* item)
{
    auto it = _substitutions_background.find(item);
    if(it!=_substitutions_background.end())
    {
        Pair p = it->second;
        CCNode* normal = p.normal;
        CCNode* selected = p.selected;

        if(normal && selected)
        {
            if(normal->isVisible())
            {
                normal->setVisible(false);
                selected->setVisible(true);
            }
            else
            {
                normal->setVisible(true);
                selected->setVisible(false);
            }
        }
    }

}

void EquationDrawer::onFreeSpacePressed(unsigned int found_i)
{


    const Symbol& symb = _equation.getSubstitutedSymbol(found_i);
    unsigned int mode=0;

    if(symb.canSubstitute(Symbol::Digit))
    {
        if(_equation.isZeroSupportedForSubstitutedSymbol(found_i))
            mode = mode | LevelKeyboard::Digits | LevelKeyboard::Zero;
        else
            mode = mode | LevelKeyboard::Digits;
    }
    if(symb.canSubstitute(Symbol::Operation))
        mode = mode | LevelKeyboard::Operations;
    if(symb.canSubstitute(Symbol::Parenthesis))
        mode = mode | LevelKeyboard::Parenthesises;


    _keyboard->setMode(LevelKeyboard::Mode(mode));


    if(_substituting_now_id < _substitutors.size())
    {
        switchImages(_substitutors[_substituting_now_id]);
    }
    _substituting_now_id = found_i;

    switchImages(_substitutors[_substituting_now_id]);

    Tutorial::getInstance()->onEmptySpaceTouch(this);

    //_selected_sprite->setVisible(true);
}
void EquationDrawer::substituteAnything(const char& cur,
                                        const unsigned int substituting_now_id)
{
    //find out who is the char - digit, operation...?
    //Symbol symbol;
    if((cur >= '0' && cur <= '9'))
    {
        substituteDigit(cur-'0', substituting_now_id,true);
    }
    else if(cur == '(')
        substituteParenthesis(Symbol::Left,substituting_now_id,true);
    else if(cur == ')')
        substituteParenthesis(Symbol::Right,substituting_now_id,true);
    else if(cur == '+' || cur == '-' || cur == '*' || cur == '/')
        substituteOperation(Symbol::getOperationByChar(cur),substituting_now_id,true);
}

void EquationDrawer::substituteDigit(const unsigned int& digit,
                                     const unsigned int substituting_now_id,
                                     const bool is_hint_mode)
{
    CCAssert(substituting_now_id < _equation.substitutionsNumber(),"");
    const Symbol& symb = _equation.getSubstitutedSymbol(substituting_now_id);
    if(symb.canSubstitute(Symbol::Digit))
    {
        _equation.substituteDigit(substituting_now_id, digit);
        if (is_hint_mode)
        {
            _hint_inserted_label_str[_substitutors_index[substituting_now_id]] = '0' + digit;
            _inserted_label_str[_substitutors_index[substituting_now_id]]=' ';

            _hint_inserted_label->setCString(_hint_inserted_label_str.c_str());
            _inserted_label->setCString(_inserted_label_str.c_str());
        }
        else
        {
            _inserted_label_str[_substitutors_index[substituting_now_id]] = '0' + digit;;
            _hint_inserted_label_str[_substitutors_index[substituting_now_id]]=' ';

            _inserted_label->setCString(_inserted_label_str.c_str());
            _hint_inserted_label->setCString(_hint_inserted_label_str.c_str());
        }

        recalculateEquation();
    }
}
void EquationDrawer::substituteOperation(const Symbol::OperationType& op,
                                         const unsigned int substituting_now_id,
                                         const bool is_hint_mode)
{
    CCAssert(substituting_now_id < _equation.substitutionsNumber(),"");
    const Symbol& symb = _equation.getSubstitutedSymbol(substituting_now_id);
    if(symb.canSubstitute(Symbol::Operation))
    {
        _equation.substituteOperation(substituting_now_id, op);



        if (is_hint_mode)
        {
            char& c = _hint_inserted_label_str[_substitutors_index[substituting_now_id]];
            _inserted_label_str[_substitutors_index[substituting_now_id]]=' ';

            if(op == Symbol::Add)
                c = '+';
            else if(op == Symbol::Multiply)
                c = '*';
            else if(op == Symbol::Substract)
                c = '-';


            _hint_inserted_label->setCString(_hint_inserted_label_str.c_str());
            _inserted_label->setCString(_inserted_label_str.c_str());
        }
        else
        {
            char& c = _inserted_label_str[_substitutors_index[_substituting_now_id]];
            _hint_inserted_label_str[_substitutors_index[_substituting_now_id]]=' ';

            if(op == Symbol::Add)
                c = '+';
            else if(op == Symbol::Multiply)
                c = '*';
            else if(op == Symbol::Substract)
                c = '-';

            _inserted_label->setCString(_inserted_label_str.c_str());
            _hint_inserted_label->setCString(_hint_inserted_label_str.c_str());
        }

        recalculateEquation();
    }
}

void EquationDrawer::substituteParenthesis(const Symbol::ParenthesisType& op,
                                           const unsigned int substituting_now_id,
                                           const bool is_hint_mode)
{
    CCAssert(substituting_now_id < _equation.substitutionsNumber(),"");
    const Symbol& symb = _equation.getSubstitutedSymbol(substituting_now_id);
    if(symb.canSubstitute(Symbol::Parenthesis))
    {
        _equation.substituteParenthesis(substituting_now_id, op);

        //char& c = _inserted_label_str[_substitutors_index[substituting_now_id]];

        if (is_hint_mode)
        {
            char& c = _hint_inserted_label_str[_substitutors_index[substituting_now_id]];
            _inserted_label_str[_substitutors_index[substituting_now_id]]=' ';


            if(op == Symbol::Left)
                c = '(';
            else if(op == Symbol::Right)
                c = ')';

            //_inserted_label->setCString(_inserted_label_str.c_str());
            _hint_inserted_label->setCString(_hint_inserted_label_str.c_str());
            _inserted_label->setCString(_inserted_label_str.c_str());
        }
        else
        {
            char& c = _inserted_label_str[_substitutors_index[substituting_now_id]];
            _hint_inserted_label_str[_substitutors_index[substituting_now_id]]=' ';

            if(op == Symbol::Left)
                c = '(';
            else if(op == Symbol::Right)
                c = ')';

            _inserted_label->setCString(_inserted_label_str.c_str());
            _hint_inserted_label->setCString(_hint_inserted_label_str.c_str());
        }

        recalculateEquation();
    }
}
void EquationDrawer::recalculateEquation()
{
   // Tutorial::getInstance()->onSubstitutionMade(this);
    _draft.clear();
    //    ExpressionResults res;

    //    res = _equation.leftAllResults();
    //    for(unsigned int i=0; i<res.size(); ++i)
    //    {
    //        _draft += res[i].toString();
    //    }
    //    res = _equation.rightAllResults();
    //    for(unsigned int i=0; i<res.size(); ++i)
    //    {
    //        _draft += res[i].toString();
    //    }

    ExpressionResult left = _equation.leftResult();
    ExpressionResult right = _equation.rightResult();
    if(left.isEvaluated() && !left.isSimpleRes())
    {
        _draft += left.toString();
    }
    if(_draft.size())
        _draft += "\n";
    if(right.isEvaluated() && !right.isSimpleRes())
    {
        _draft += right.toString();
    }

    bool is_duplicate = false;

    if(left.isEvaluated() && right.isEvaluated())
    {
        //We can calculate both left and right side of equation
        if(left.result() == right.result())
        {
            RawSubstitutionsArr current_substitutions(_equation.substitutionsNumber());
            unsigned int substitutions_number = _equation.substitutionsNumber();
            for(unsigned int i=0; i<substitutions_number; ++i)
            {
                current_substitutions[i]=_equation.getSubstitutedSymbol(i).toString();
            }
            Solution current_solution(current_substitutions);
            //const Solutions& found_solutions = _level->getFoundSolutions();
            if(RW::registerSolution(_level, current_solution))
            {

                ADStatistics::logEvent("Finished level", levelToStatisticsParams(_level));
                if(_level->getFoundSolutions().size() == _level->getSolutions().size())
                {
                    ADStatistics::logEvent("Found all solutions", levelToStatisticsParams(_level));
                }

                //This is new solution
                _inserted_label->setColor(EquationColor_NewSolution);
                LevelScenePopUp* pop_up = LevelScenePopUp::create(this);
                _parent->addChild(pop_up);
                _parent->hideMe(true);
                _parent->onFoundSolutionOpen(pop_up);

                for (unsigned int i=0; i<_hint_inserted_label_str.size(); ++i)
                {
                    if (_hint_inserted_label_str[i]!=' ')
                    {
                        _inserted_label_str[i]=_hint_inserted_label_str[i];
                        _hint_inserted_label_str[i]=' ';
                    }
                }
                _inserted_label->setString(_inserted_label_str.c_str());
                _hint_inserted_label->setString(_hint_inserted_label_str.c_str());
            }
            else
            {
                //This is already found solution
                _inserted_label->setColor(EquationColor_OldSolution);
                is_duplicate = true;
            }
        }
        else
        {
            //Result do not fit an equation
            _inserted_label->setColor(_eq_lab->getColor());
        }
    }
    else
    {
        _inserted_label->setColor(_eq_lab->getColor());
    }

    if(_duplicate_solution_shown != is_duplicate && !_no_more_solutions_shown)
    {
        //duplication switch
        if(is_duplicate == false)
        {
            _duplicate_solution_shown = false;
            if(_message_node)
            {
                hideMessageNode();
            }
        }
        else
        {
            _duplicate_solution_shown = true;
            if(_message_node)
            {
                showMessageNode();
            }
            else
            {
                //Do not show it is already foind solution
                //show level end pop Up
                _inserted_label->setColor(EquationColor_NewSolution);
                LevelScenePopUp* pop_up = LevelScenePopUp::create(this);
                _parent->addChild(pop_up);
                _parent->hideMe(true);
                _parent->onFoundSolutionOpen(pop_up);



//                _message_node = CCNodeRGBA::create();

//                _message_node_text = CCLabelTTF::create(_("solution.already_found_solution"),
//                                                        ADLanguage::getFontName(),
//                                                        30);
//                //CCSprite::create(Language::localizeFileName("level/tutorial/already_found.png").c_str());
//                CCSize text_size = _message_node_text->getContentSize();
//                _message_node_text->setColor(GameInfo::COLOR_ORANGE);

//                _message_node->setContentSize(CCSize(text_size.width, text_size.height*1.75f));
//                _message_node_text->setAnchorPoint(ccp(0,0));
//                _message_node->addChild(_message_node_text);
//                _message_node_text->setPosition(ccp(0, text_size.height*0.75f));

//                auto click_action = [this](){
//                    _parent->restart();
//                };

//                CCLabelTTF* restart_tutorial = CCLabelTTF::create("",
//                                                        ADLanguage::getFontName(),
//                                                        30);
//                _message_node_button = ADMenuItem::create(
//                            restart_tutorial/*CCSprite::create("level/tutorial/restart_tut.png")*/,
//                            click_action);

//                CCMenu* menu = CCMenu::create();
//                menu->addChild(_message_node_button);
//                //skip_button->setAnchorPoint(ccp(0.5, 0.5));
//                CCSize skip_size = _message_node_button->getContentSize();
//                float target_height = text_size.height*0.75f;
//                float scale =target_height / skip_size.height;
//                _message_node_button->setScale(scale);
//                _message_node_button->setPosition(ccp(text_size.width/2, target_height/2));
//                _message_node->addChild(menu);


//                _message_node_text->setOpacity(0);
//                _message_node_button->setOpacity(0);




//                this->addChild(_message_node);
//                putMessageNodeInPlace();
//                showMessageNode();
            }
        }

    }

    updateDraft();

}

void EquationDrawer::showMessageNode()
{
    //Tutorial::getInstance()->hideLabels();
    _message_node_button->setEnabled(true);
    _message_node_button->runAction(CCFadeTo::create(0.3f, 255));
    _message_node_text->runAction(CCFadeTo::create(0.3f, 255));
}

void EquationDrawer::hideMessageNode()
{
    if(_message_node)
    {
        _message_node_button->setEnabled(false);
        _message_node_button->runAction(CCFadeTo::create(0.15f, 0));
        _message_node_text->runAction(CCFadeTo::create(0.15f, 0));
    }
}

void EquationDrawer::putMessageNodeInPlace()
{
    float y_move = 0;
    float scale = 1;
    if(RW::isExpertMode())
    {
        y_move = 6*_paper_cell_size;

    }
    else
    {
        y_move = -_draft_label->getPositionY();
        y_move += 6.5f*_paper_cell_size;

        float max_height = _zone.height - y_move + 0.5f * _paper_cell_size;
        scale = MIN(1.0, max_height / _message_node->getContentSize().height);


    }
    float scaled = ADScreen::getScaleFactor();
    _message_node->setAnchorPoint(ccp(0,1));
    _message_node->setPosition(_keyboard->getContentSize().width +
                               _paper_cell_size/scaled,
                               - y_move);
    _message_node->setScale(scale);
}

void EquationDrawer::initNoMoreSolutions()
{
    if(_level->getFoundSolutions().size() == _level->getSolutions().size())
    {
//        _no_more_solutions_shown = true;
//        _message_node = CCNodeRGBA::create();



//        CCLabelTTF* solutions_text = CCLabelTTF::create("",
//                                                ADLanguage::getFontName(),
//                                                30);
//        solutions_text->setColor(GameInfo::COLOR_LIGHT_BLUE);
//        _message_node_text = CCLabelTTF::create(_("solution.no_more_solution"),
//                                                ADLanguage::getFontName(),
//                                                35);
//        _message_node_text->setColor(GameInfo::COLOR_ORANGE);

//        CCSize text_size = _message_node_text->getContentSize();

//        _message_node->setContentSize(CCSize(text_size.width, text_size.height*1.75f));
//        _message_node_text->setAnchorPoint(ccp(0,0));
//        _message_node->addChild(_message_node_text);
//        _message_node_text->setPosition(ccp(0, text_size.height*0.75f));

//        auto click_action = [this](){
//            _parent->levels();
//        };

//        _message_node_button = ADMenuItem::create(
//                    solutions_text,
//                    click_action);

//        CCMenu* menu = CCMenu::create();

//        menu->addChild(_message_node_button);
//        //skip_button->setAnchorPoint(ccp(0.5, 0.5));
//        CCSize skip_size = _message_node_button->getContentSize();
//        float target_height = text_size.height*0.75f;
//        float scale =target_height / skip_size.height;
//        _message_node_button->setScale(scale);
//        _message_node_button->setPosition(ccp(text_size.width/2, target_height/2));
//        _message_node->addChild(menu);


//        //_message_node_text->setOpacity(0);
//        _message_node_button->setOpacity(0);




//        this->addChild(_message_node);
//        putMessageNodeInPlace();
//        showMessageNode();
    }
}

