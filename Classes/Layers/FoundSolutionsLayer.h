#ifndef FOUNDSOLUTIONSLAYER_H
#define FOUNDSOLUTIONSLAYER_H
#include "cocos2d-A.h"
#include "Logic/Level.h"
class FoundSolutionsLayer : public cocos2d::CCNode
{
public:
    FoundSolutionsLayer(Level*);
    static FoundSolutionsLayer* create(Level*level);
    void hideAndRemove();
private:
    void selfDestroy();
    CCPoint _sheet_target_position;
};

#endif // FOUNDSOLUTIONSLAYER_H
