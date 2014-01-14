#ifndef TERM_H
#define TERM_H
#include "Symbol.h"
#include <vector>
#include <string>
/**
 * @brief Term can be:
 * - number represented as Array of Symbols
 * - operations represented as Symbol
 * - parenthesis represented as Symbol
 */
typedef long long int MInt;
class Term
{
public:
    typedef std::vector<Symbol> SymbolArr;
    typedef std::vector<Symbol*> SubstitutionsArr;
    enum Type
    {
        Number,
        Operation,
        Parenthesis
    };
    Term(const SymbolArr& arr); //for creating numbers
    Term(const Type& type, const Symbol& symbol); //for creating operators
    Term(const Term& term);
    Term& operator=(const Term&);

    bool hasSubstitutions() const
    {
        return _has_substitutions;
    }
    const Type& getType() const
    {
        return _type;
    }
    const Symbol& getSymbol() const
    {
        return _symbol;
    }
    std::string toString() const;
    bool numberCanBeEvaluated() const;
    MInt toNumber() const;
private:
    //void updateSubstitutions();

    SymbolArr _digits;
    Symbol _symbol;
    //SubstitutionsArr _substitutions;
    Type _type;
    bool _has_substitutions;
};

#endif // TERM_H
