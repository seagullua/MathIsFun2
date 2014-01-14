#include "Symbol.h"
#include <cassert>
Symbol::Symbol(const Type & type,
               const int & value)
    : _type(type), _value(value), _substitutable(false),
      _possible_types(PossibleType(type))
{
    assert(value >= 0);
}
Symbol::Symbol(const PossibleType &possible_types)
    : _type(None), _value(-1), _substitutable(true),
      _possible_types(possible_types)
{
    if(possible_types == PDigit ||
            possible_types == POperation ||
            possible_types == PParenthesis)
    {
        _type=Type(possible_types);
    }
}
Symbol::Symbol()
    : _type(None), _value(-1), _substitutable(false), _possible_types(PNone)
{

}

const Symbol Symbol::createSubstitutable(const PossibleType &possible_types)
{
    return Symbol(possible_types);
}

const Symbol Symbol::createSymbol(const Type& t, const unsigned int& value)
{
    return Symbol(t, value);
}

Symbol::Type Symbol::getType() const
{
    return _type;
}
bool Symbol::canSubstitute(const Type& t) const
{
    return (_possible_types & t) != 0;
}

char Symbol::toString() const
{
    if(isSubstitutable() && _value < 0)
        return '_';
    if(_type==Digit)
        return '0'+_value;
    if(_type==Parenthesis)
    {
        if(_value == Left)
            return '(';
        else
            return ')';
    }
    else if(_type==Operation)
    {
        if(_value == Add)
            return '+';
        else if(_value == Substract)
            return '-';
        else if(_value == Multiply)
            return '*';
    }
    return '?';
}

