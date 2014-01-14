#ifndef EQUATIONSOLVER_H
#define EQUATIONSOLVER_H
#include "Equation.h"
#include "Solution.h"
#include <vector>
#include <string>
typedef std::vector<std::string> StringSolutions;
class EquationSolver
{
public:
    EquationSolver(const Equation& solve);
    const StringSolutions& getStringSolutions() const
    {
        return _string_solutions;
    }
    void solve()
    {
        solveFrom(_equation, 0);
    }
private:
    void solveFrom(Equation &_equation, const unsigned int& substitution_id);
    //void detachOnNeed(Equation &_equation, const unsigned int& substitution_id);
    bool shouldDetach(const unsigned int& substitution_id)
    {
        return _substitutions - substitution_id <= _detach_if;
    }

    void solveFromHelper(Equation& eq,
                         const unsigned int& substitution_id,
                         bool take_copy=false)
    {
        if(take_copy)
        {
            Equation eq_copy(eq);
            solveFrom(eq_copy, substitution_id);
        }
        else
        {
            solveFrom(eq, substitution_id);
        }
    }

    Equation _equation;
    Solutions _found_solutions;
    StringSolutions _string_solutions;
    unsigned int _substitutions;
    int _detached_thread;
    static const unsigned int _detach_if = 4;
};

#endif // EQUATIONSOLVER_H
