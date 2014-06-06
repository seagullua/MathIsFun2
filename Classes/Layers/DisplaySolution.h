#ifndef DISPLAYSOLUTION_H
#define DISPLAYSOLUTION_H
#include "cocos2d-A.h"
#include "Math/Equation.h"
#include "Math/Solution.h"
#include "Logic/Level.h"
class DisplaySolution : public cocos2d::CCNode
{
public:
    DisplaySolution(cocos2d::CCSize zone_size,
                    Level* level,
                    const Solutions& solutions_to_display,
                    const Solutions& all_solutions,
                    bool show_stamps=false);
    static DisplaySolution* create(cocos2d::CCSize zone_size,
                                   Level* level,
                                   const Solutions& solutions_to_display,
                                   const Solutions& all_solutions,
                                   bool show_stamps=false);
};

#endif // DISPLAYSOLUTION_H
