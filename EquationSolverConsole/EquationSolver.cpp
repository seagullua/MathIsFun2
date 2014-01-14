#include "EquationSolver.h"
#ifdef USE_OMP
#include <omp.h>
#endif
#include <iostream>
EquationSolver::EquationSolver(const Equation& solve)
    : _equation(solve), _substitutions(solve.substitutionsNumber()),
      _detached_thread(-1)
{
}

void EquationSolver::solveFrom(Equation& equation,
                               const unsigned int& substitution_id)
{
    if(substitution_id == _substitutions)
    {
        //Check the equation
        ExpressionResult left = equation.leftResult();
        ExpressionResult right = equation.rightResult();
        if(left.isEvaluated() && right.isEvaluated())
        {
            //We can calculate both left and right side of equation
            if(left.result() == right.result())
            {
                RawSubstitutionsArr current_substitutions(equation.substitutionsNumber());
                unsigned int substitutions_number = equation.substitutionsNumber();
                for(unsigned int i=0; i<substitutions_number; ++i)
                {
                    current_substitutions[i]=equation.getSubstitutedSymbol(i).toString();
                }
                Solution current_solution(current_substitutions);
                #pragma omp critical
                {
                    _found_solutions.push_back(current_solution);
                    _string_solutions.push_back(equation.toString());
                    std::cout.write(&current_substitutions[0], current_substitutions.size());
                    std::cout << std::endl;
                }
            }
        }
    }
    else
    {
        Symbol symb = equation.getSubstitutedSymbol(substitution_id);
        Symbol::PossibleType p_type = symb.getPossibleType();

        if(p_type & Symbol::PDigit )
        {
            //#pragma omp parallel for
            if(_detached_thread < 0)
            {
                _detached_thread=substitution_id;
                #pragma omp parallel
                {
                    #pragma omp for nowait
                    for(unsigned int i=0; i<=9; ++i)
                    {
                        equation.substituteDigit(substitution_id, i);
                        solveFromHelper(equation, substitution_id+1, true);
                    }
                }
            }
            else
            {
                for(unsigned int i=0; i<=9; ++i)
                {
                    equation.substituteDigit(substitution_id, i);
                    solveFrom(equation, substitution_id+1);
                }
            }

        }
        if(p_type & Symbol::POperation)
        {
            equation.substituteOperation(substitution_id, Symbol::Add);
            solveFrom(equation, substitution_id+1);
            equation.substituteOperation(substitution_id, Symbol::Substract);
            solveFrom(equation, substitution_id+1);
            equation.substituteOperation(substitution_id, Symbol::Multiply);
            solveFrom(equation, substitution_id+1);
        }
        if(p_type & Symbol::PParenthesis)
        {
            equation.substituteParenthesis(substitution_id, Symbol::Left);
            solveFrom(equation, substitution_id+1);
            equation.substituteParenthesis(substitution_id, Symbol::Right);
            solveFrom(equation, substitution_id+1);
        }
    }
}

/*void EquationSolver::detachOnNeed(Equation &equation, const unsigned int& substitution_id)
{
    if(_substitutions - substitution_id <= _detach_if)
    {
        #pragma omp single nowait
        solveFromHelper(equation, substitution_id, true);
    }
    else
    {
        solveFromHelper(equation, substitution_id, false);
    }
}*/
