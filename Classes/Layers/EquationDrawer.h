#ifndef EQUATIONDRAWER_H
#define EQUATIONDRAWER_H
#include "cocos2d-A.h"
#include "Logic/Level.h"
#include "Math/Equation.h"
#include "LevelKeyboard.h"
#include "Layers/PopUpWindow.h"
class LevelScene;
class EquationDrawer : public cocos2d::CCNodeRGBA
{
    EquationDrawer(Level* level, const cocos2d::CCSize& _zone,
                   LevelScene* parent, ADPopUpWindowManager& pop_up_m);
public:
    static EquationDrawer* create(Level *level,
                                  const cocos2d::CCSize& _zone,
                                  LevelScene* parent,
                                  ADPopUpWindowManager& pop_up_m);

    void useHint();
    Level* getLevel() const
    {
        return _level;
    }
    void sleep();
    void wakeup();
    LevelScene* getParentScene();
    //void setOpacity(GLubyte);
    void showUI();
    void onFoundSolutionClose();

    LevelKeyboard* getLevelKeyboard();
    const std::vector<ADMenuItem*>& getSubstituteItems();
    float getEquaionLabelScale();
    Equation& getEquation()
    {
        return _equation;
    }

    ~EquationDrawer();


    void substituteAnything(const char& cur, const unsigned int substituting_now_id);

private:
    class KeyboardReactor : public LevelKeyboard::KeyboardDelegate
    {
    public:
        KeyboardReactor(EquationDrawer* parent);
        void onKeyPressed(const LevelKeyboard::ButtonsCodes&);
    private:
        EquationDrawer* _p;
    };
    void finishHide();
    void finishShow();
    static const cocos2d::ccColor3B EquationColor_NewSolution;
    static const cocos2d::ccColor3B EquationColor_OldSolution;
    static const cocos2d::ccColor3B EquationColor_HintSolution;
    LevelScene* _parent;
    Equation _equation;
    Level* _level;
    LevelKeyboard* _keyboard;
    MenuSpriteBatch* _substitutions;
    KeyboardReactor* _keyboard_reactor;
    cocos2d::CCLabelBMFont* _inserted_label;
    cocos2d::CCLabelBMFont* _hint_inserted_label;
    std::string _inserted_label_str;
    std::string _hint_inserted_label_str;
    std::string _raw_equation;
    std::string _draft;
    cocos2d::CCLabelBMFont* _eq_lab;
    cocos2d::CCLabelBMFont* _draft_label;
    //cocos2d::CCSprite* _selected_sprite;
    //CCNode* _backup_sprite;
    CCNode* _message_node;
    //cocos2d::CCSprite* _message_node_text;
    //Changed
    cocos2d::CCLabelTTF* _message_node_text;
    MenuSpriteBatch* _message_node_menu;
    ADMenuItem* _message_node_button;

    ADPopUpWindowManager& _pop_up_manager;

    bool _no_more_solutions_shown;
    bool _duplicate_solution_shown;

    cocos2d::CCSize _zone;
    float _font_symbol_height;
    cocos2d::CCSize _max_draft_size;
    std::vector<ADMenuItem*> _substitutors;
    std::vector<unsigned int> _substitutors_index;
    unsigned int _substituting_now_id;

    void onKeyboardClick(LevelKeyboard::ButtonsCodes);
    void substituteDigit(const unsigned int& digit,
                         const unsigned int substituting_now_id,
                         const bool is_hint_mode=false);

    void substituteOperation(const Symbol::OperationType& op,
                             const unsigned int substituting_now_id,
                             const bool is_hint_mode=false);

    void substituteParenthesis(const Symbol::ParenthesisType& op,
                               const unsigned int substituting_now_id,
                               const bool is_hint_mode=false);
    void onFreeSpacePressed(unsigned int found_i);
    void recalculateEquation();
    void showMessageNode();
    void hideMessageNode();
    void putMessageNodeInPlace();


    void initKeyboard();
    void initEquationLabel();
    void initNoMoreSolutions();
    void initDraft();
    void updateDraft();
    static const float _paper_cell_size;
    struct Pair
    {
        cocos2d::CCSprite* selected;
        cocos2d::CCSprite* normal;
    };

    typedef std::map<ADMenuItem*, Pair> SubstitutionsMap;
    SubstitutionsMap _substitutions_background;

    void switchImages(ADMenuItem* item);
};

#endif // EQUATIONDRAWER_H
