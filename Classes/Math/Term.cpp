#include "Term.h"
#include <cassert>
Term::Term(const Term::SymbolArr& arr)
    : _digits(arr), _symbol(), _type(Number), _has_substitutions(false)
{
    for(unsigned int i=0; i<_digits.size(); ++i)
    {
        if(_digits[i].isSubstitutable())
        {
            _has_substitutions=true;
            break;
        }
    }
}

Term::Term(const Term::Type& type, const Symbol& symbol)
    :_digits(0),_symbol(symbol),
       _type(type), _has_substitutions(symbol.isSubstitutable())
{
    assert(_type == Parenthesis || _type == Operation);
}
Term::Term(const Term& term)
    :_digits(term._digits), _symbol(term._symbol),
     _type(term._type), _has_substitutions(term._has_substitutions)
{
}


Term& Term::operator=(const Term& term)
{
    _digits = term._digits;
    _symbol = term._symbol;
    _type = term._type;
    _has_substitutions = term._has_substitutions;
    return *this;
}

std::string Term::toString() const
{
    if(_type == Number)
    {
        std::string result(_digits.size(), ' ');
        for(unsigned int i=0; i<_digits.size(); ++i)
        {
            result[i] = _digits[i].toString();
        }
        return result;
    }
    else
        return std::string(1, _symbol.toString());
}

bool Term::numberCanBeEvaluated() const
{
    bool res = true;
    for(unsigned int i=0; i<_digits.size(); ++i)
    {
        if(!_digits[i].hasValue())
        {
            res = false;
            break;
        }
    }
    return res;
}
MInt Term::toNumber() const
{
    assert(numberCanBeEvaluated());
    MInt res = 0;
    unsigned int num_size = _digits.size();
    for(unsigned int i=0; i<num_size; ++i)
    {
        unsigned int digit = _digits[i].getDigit();
        res*=10;
        res+=digit;
    }
    return res;
}
