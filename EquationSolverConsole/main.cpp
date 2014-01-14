#include <iostream>
#include <string>
#include <ctime>
#include "Equation.h"
#include "EquationSolver.h"
#ifdef USE_OMP
#include <omp.h>
#endif
using namespace std;
int omp_thread_count() {
    int n = 0;
    #pragma omp parallel reduction(+:n)
    n += 1;
    return n;
}
int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        cout << "Wrong syntax. Pass the equation as the parameter" << endl;
        return 1;
    }
    //omp_set_num_threads(8);
#ifdef BENCHMARK
    cout << "Threads: " << omp_get_num_threads() <<' ' << omp_thread_count() << endl;
#endif
    string input = argv[1];
    string left("");
    string right("");
    left.reserve(input.size());
    right.reserve(input.size());

    bool middle_found = false;
    for(unsigned int i=0; i<input.size(); ++i)
    {
        char ch = input[i];
        if(ch == '=' && middle_found)
            return 1;
        else if(ch == '=')
            middle_found=true;
        else if(middle_found)
            right.push_back(ch);
        else
            left.push_back(ch);
    }

    if(left.size() && right.size())
    {

        clock_t miliseconds = clock();

        Equation eq(left, right);
        EquationSolver solver(eq);
        solver.solve();

        // do something..
        double nMilliseconds = clock() - miliseconds;

        double substitutions_number = 1;
        for(unsigned int i=0; i<eq.substitutionsNumber(); ++i)
        {
            unsigned int digit_comb = 0;
            Symbol symb = eq.getSubstitutedSymbol(i);
            Symbol::PossibleType p_type = symb.getPossibleType();

            if(p_type & Symbol::PDigit )
                digit_comb += 10;
            if(p_type & Symbol::POperation)
                digit_comb += 3;
            if(p_type & Symbol::PParenthesis)
                digit_comb += 2;
            substitutions_number *= digit_comb;
        }
        #ifdef BENCHMARK
        StringSolutions solutions = solver.getStringSolutions();
        for(unsigned int i=0; i<solutions.size(); ++i)
        {
            cout << solutions[i] << endl;
        }

        cout << "Substitutions per second: " <<  (substitutions_number/
                                                  (nMilliseconds/1000)) << endl;
        #endif

    }

    return 0;




}

