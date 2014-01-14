#ifndef DEFINELEVELS_H
#define DEFINELEVELS_H
#include "Logic/RW.h"
#include "Logic/Collection.h"
#include "Math/Solution.h"
#include <fstream>
#include <string>
#include "Binary/Binary.h"
#include "Math/Equation.h"
#include <algorithm>
#include <set>
#include <sstream>
#ifdef RW_BUILD_LEVELS



class RWNotationReader
{
public:
    RWNotationReader(const std::string& save_file, unsigned int collections) :
        _error(false), COLLECTIONS(collections), _save_file(save_file)
    {
        RW::prepareForLevelBuild();
    }
    ~RWNotationReader()
    {
        if(RW::_rw->_collections.size() == 0)
        {
            setError("No collections defined", "END", 0);
        }
        else if(!_error && RW::_rw->_collections.size() < COLLECTIONS)
        {
            setWarning("Not enough collections defined. Duplication 1-st collection", "END", 0);
            Collection* base_col = RW::_rw->_collections.begin()->second;
            Level* base = base_col->_levels[0];
            for(unsigned int i=RW::_rw->_collections.size(); i<COLLECTIONS; ++i)
            {

                Collection* a = createCollection("END", 0);
                //a->_stamps_max = 60;

                Level* l = createLevel(a, "END", 0);
                std::string eq = base->_equation->getUntouchedLhsString() + "=" +
                        base->_equation->getUntouchedRhsString();
                addEquation(l, eq.c_str(), "END", 0);
                l->_existing_solutions = base->_existing_solutions;
                validateCollection(a, "END", 0);
            }
        }

        std::ofstream output(_save_file.c_str(), std::ios::out | std::ios::binary);
        if(!_error)
        {
            OutputBinaryStream os(output, 1);
            RW::writeLevelsInformation(os);
            output.close();
        }
        else
        {
            std::cout << "Open file error" << std::endl;
        }
    }
    void setError(const std::string& error,
                  const std::string& file,
                  unsigned int line)
    {
        std::cerr << "ERROR: " << error << std::endl;
        std::cerr << "\tLine: " << line << std::endl;
        std::cerr << "\tFile: " << file << std::endl;
        _error = true;
    }
    void setWarning(const std::string& error,
                  const std::string& file,
                  unsigned int line)
    {
        std::cerr << "WARNING: " << error << std::endl;
        std::cerr << "\tLine: " << line << std::endl;
        std::cerr << "\tFile: " << file << std::endl;
    }
    void addEquation(Level* level, const char* expr, const char* file, unsigned int line, bool check=true)
    {
        std::string equation(expr);

        if(check && isDublicateEquation(equation))
        {
            setError("Equation duplicate!", file, line);
        }

        unsigned int first_i=equation.size();
        unsigned int found=0;
        for(unsigned int i=0; i<equation.size(); ++i)
        {
            if(equation[i] == '=')
            {
                if(found==0)
                    first_i = i;
                found++;
            }
        }
        if(found != 1)
            setError("Wrong equation", file, line);
        else if(level->_equation)
            setError("Level already has an equation", file, line);
        else
        {
            std::string lhs = equation.substr(0, first_i);
            std::string rhs = equation.substr(first_i+1, equation.size()-first_i-1);
            level->_equation = new Equation(lhs, rhs);
        }
    }
    void validateCollection(Collection* a, const char* file, unsigned int line)
    {
//        if(a->_stamps_max == 0)
//            setError("Stamps max for collection is not defined", file, line);
        if(a->_levels.size() == 0)
            setError("No levels in collection", file, line);
        if(a->_collection_id == 0)
            setError("Wrong collection ID", file, line);
        else if(a->_levels.size() < LEVELS_IN_COLLECTION)
        {
            Level* base = a->_levels[0];
            std::stringstream ss;
            ss << "Not enough levels in collection. Duplication 1-st level. NEED MORE: "  << (LEVELS_IN_COLLECTION - a->_levels.size());

            setWarning(ss.str().c_str(), file, line);
            for(unsigned int i=a->_levels.size(); i<LEVELS_IN_COLLECTION; ++i)
            {
                Level* l = createLevel(a, file, line);
                std::string eq = base->_equation->getUntouchedLhsString() + "=" +
                        base->_equation->getUntouchedRhsString();
                addEquation(l, eq.c_str(), file, line, false);
                l->_existing_solutions = base->_existing_solutions;
            }
        }
        RW::_rw->_collections[a->_collection_id] = a;

    }

    void validateSolution(Level* level, const Solution& s, const char* file, unsigned int line)
    {
        if(std::find(level->_existing_solutions.begin(),
                     level->_existing_solutions.end(),
                     s) == level->_existing_solutions.end())
        {
            bool ok = true;
            for(unsigned int i=0; i<s.getSubstitutions().size(); ++i)
            {

                char sol = s.getSubstitutions()[i];
                if(sol == '0')
                {
                    const Symbol& symb = level->_equation->getSubstitutedSymbol(i);
                    if(symb.canSubstitute(Symbol::Digit) && !level->_equation->isZeroSupportedForSubstitutedSymbol(i))
                    {
                        ok = level->_equation->isZeroSupportedForSubstitutedSymbol(i);
                    }

                }
            }
            if(ok)
                level->_existing_solutions.push_back(s);
            else
                setError("Wrong ZERO in solution", file, line);
        }
        else
            setError("Solution duplication", file, line);
    }

    void addSolution(Level* level, const char* expr, const char* file, unsigned int line)
    {
        std::string sol(expr);
        if(!level->_equation)
            setError("Equation for level not defined", file, line);
        else if(level->_equation->substitutionsNumber() != sol.size())
            setError("Equation does not fit the solution", file, line);
        RawSubstitutionsArr arr(sol.begin(), sol.end());
        Solution s(arr);

        validateSolution(level,s,file,line);
    }
    void validateLevel(Level* level, const char* file, unsigned int line)
    {
        if(level->_existing_solutions.size() == 0)
            setError("Level without solutions", file, line);

    }
    Collection* createCollection(const char* file, unsigned int line)
    {
        if(RW::_rw->_collections.size() >= COLLECTIONS)
        {
            setError("To much collections", file, line);
        }
        Collection* a = new Collection;
        //RW::_rw->_collections.push_back(a);
        return a;
    }
    Level* createLevel(Collection* col, const char* file, unsigned int line)
    {
        if(col->_levels.size() >= LEVELS_IN_COLLECTION)
        {
            setError("To much levels in collection", file, line);
        }
        Level* l = new Level;
        l->_parent = col;
        l->_level_id = col->_levels.size();
        col->_levels.push_back(l);
        return l;
    }
private:
    bool isDublicateEquation(const std::string& level)
    {
        Set::iterator it = _equations.find(level);
        if(it != _equations.end())
            return true;

        _equations.insert(level);
        return false;
    }
    typedef std::set<std::string> Set;

    Set _equations;
    bool _error;
    static const unsigned int LEVELS_IN_COLLECTION = 20;
    unsigned int COLLECTIONS;
    std::string _save_file;
};

#define START_DEFINITION(PROJ, SAVE_FILE, COLLECTIONS) void PROJ (){RWNotationReader r(SAVE_FILE, COLLECTIONS);
#define END_DEFINITION  }

#define END }
#define COLLECTION { Collection* a = r.createCollection(__FILE__, __LINE__);

#define COLLECTION_STAMPS_TO_UNLOCK(EXPR) a->_stamps_to_unlock = EXPR;
#define COLLECTION_ID(EXPR) a->_collection_id = EXPR;
#define COLLECTION_COLOR(R,G,B) a->_collection_color = ccc3(R,G,B);
#define COLLECTION_FREE(EXPR) a->_collection_free = EXPR;
#define COLLECTION_DIFFICULTY(EXPR) a->_collection_difficulty = Collection::EXPR;

#define END_COLLECTION r.validateCollection(a, __FILE__, __LINE__);}

#define Q(x) #x
#define QUOTE(x) Q(x)

#define LEVEL { Level* l = r.createLevel(a, __FILE__, __LINE__);
#define END_LEVEL r.validateLevel(l, __FILE__, __LINE__);}
#define EQUATION(AA) r.addEquation(l, QUOTE(AA), __FILE__, __LINE__);
#define ADD_SOLUTION(EXPR) r.addSolution(l, QUOTE(EXPR), __FILE__, __LINE__);
#endif
#endif // DEFINELEVELS_H
