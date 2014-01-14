#include "DisplaySolution.h"

DisplaySolution::DisplaySolution(CCSize zone_size,
                                 Level *current_level,
                                 const Solutions &solutions_to_display,
                                 const Solutions& all_solutions, bool show_stamps)
{
    const Equation& eq = current_level->getEquation();
    this->setContentSize(zone_size);
    std::string raw_equation(eq.toStringRaw());
    std::string equations(raw_equation);
    unsigned int eq_to_display = all_solutions.size();
    unsigned int with_solutions = solutions_to_display.size();
    std::string empty_space(raw_equation.size(), ' ');
    std::string solution_layer(empty_space);
    equations.reserve((raw_equation.size()+1)*eq_to_display);
    for(unsigned int i=1; i<eq_to_display; ++i)
    {
        equations += "\n";
        equations += raw_equation;
        solution_layer += "\n";
        solution_layer += empty_space;
    }

    CCNode* main = CCNode::create();

    CCLabelBMFont* eq_lab = CCLabelBMFont::create(equations.c_str(),
                                                  "font/mathisfun.fnt");
    main->addChild(eq_lab);
    eq_lab->setAnchorPoint(ccp(0,0));
    eq_lab->setColor(ccc3(70, 148, 255));
    this->addChild(main);


    CCSize label_size = eq_lab->getContentSize();
    float symbol_length = label_size.width / raw_equation.size();
    float symbol_height = label_size.height / eq_to_display;

    float stamps_margin = 0;
    if(show_stamps)
        stamps_margin = symbol_height;
    eq_lab->setPosition(ccp(stamps_margin, 0));

    Stamps level_stamps;
    unsigned int stamps_number=0;
    if(show_stamps)
    {
        level_stamps = current_level->getLevelStamps();
        stamps_number = current_level->getSolutions().size();

        SpritesLoader stamps_spl = GraphicsManager::getLoaderFor(main,
                                                                 "level/stamps.plist",
                                                                 "level/stamps.png");
        for(unsigned int idx=0; idx<stamps_number; ++idx)
        {
            CCSprite* sp = 0;
            unsigned int i=stamps_number - idx - 1;
            if(i < level_stamps.size())
            {
                const Stamp& stamp = level_stamps[i];
                sp = stamps_spl->loadSprite(stamp.getFileName().c_str());
                sp->setRotation(stamp.getRotation());
            }
            else
            {
                sp = stamps_spl->loadSprite("empty_stamp.png");
            }

            if(sp)
            {
                sp->setPositionY(idx*symbol_height+symbol_height/2);
                sp->setPositionX(symbol_height/2);
                sp->setAnchorPoint(ccp(0.5,0.5));
                sp->setScale(symbol_height/sp->getContentSize().height*0.8);
            }

        }
    }

    SpritesLoader col_spl = GraphicsManager::getLoaderFor(
                main,
                "level/input_space.plist",
                "level/input_space.png");
    col_spl->inject();
    unsigned int next_id = 0;
    static const char SP_DIGIT[] = "digit.png";
    static const char SP_PARENTHESIS[] = "parenthesis.png";
    static const char SP_OPERATION[] = "operation.png";
    static const char SP_DIG_OP[] = "dig_op.png";
    static const char SP_DIG_PAR[] = "dig_par.png";
    static const char SP_OP_PAR[] = "op_par.png";
    static const char SP_ANY[] = "any.png";
    for(unsigned int i=0; i<raw_equation.size(); ++i)
    {
        if(raw_equation[i]=='_')
        {
            Symbol::PossibleType type = eq.getSubstitutedSymbol(next_id).getPossibleType();
            const char* sp_name = 0;
            if(type == Symbol::PDigit)
                sp_name = SP_DIGIT;
            else if(type == Symbol::PParenthesis)
                sp_name = SP_PARENTHESIS;
            else if(type == Symbol::POperation)
                sp_name = SP_OPERATION;
            else if(type == Symbol::PDigitOrOperation)
                sp_name = SP_DIG_OP;
            else if(type == Symbol::PParenthesisOrDigit)
                sp_name = SP_DIG_PAR;
            else if(type == Symbol::PParenthesisOrOperation)
                sp_name = SP_OP_PAR;
            else
                sp_name = SP_ANY;

            for(unsigned int row=0; row < eq_to_display; ++row)
            {
                CCSprite* sp = col_spl->loadSprite(sp_name);
                sp->setPosition(ccp(symbol_length*i+stamps_margin,symbol_height*row));
                sp->setAnchorPoint(ccp(0,0));

                if(row < with_solutions)
                {
                    unsigned int str_idx = row * (raw_equation.size() + 1) + i;
                    solution_layer[str_idx] = solutions_to_display[row].getSubstitutions()[next_id];
                }
            }
            next_id++;
        }
    }
    CCLabelBMFont* solutions = CCLabelBMFont::create(solution_layer.c_str(),
                                                  "font/mathisfun.fnt");
    main->addChild(solutions);
    solutions->setAnchorPoint(ccp(0,0));
    solutions->setColor(ccc3(79,213,6));
    solutions->setPosition(eq_lab->getPosition());

    float scale = MIN((zone_size.width)/(label_size.width+stamps_margin), zone_size.height/label_size.height);
    main->setScale(scale);

    main->setPosition(ccp((zone_size.width - (label_size.width+stamps_margin)*scale)/2,
                          (zone_size.height - label_size.height*scale)/2));
}

DisplaySolution* DisplaySolution::create(CCSize zone_size,
                                   Level *level,
                                   const Solutions& solutions_to_display,
                                   const Solutions& all_solutions, bool show_stamps)
{
    DisplaySolution* pObj = new DisplaySolution(zone_size, level, solutions_to_display, all_solutions, show_stamps);
    if(pObj)
        pObj->autorelease();
    return pObj;
}
