#ifndef SYMBOL_H
#define SYMBOL_H
#include <cassert>
class Symbol
{
public:
    enum Type
    {
        Digit=1,
        Operation=2,
        Parenthesis=4,
        None=0
    };
    enum PossibleType
    {
        PNone=0,
        PDigit=1,
        POperation=2,
        PDigitOrOperation=3,
        PParenthesis=4,
        PParenthesisOrDigit=5,
        PParenthesisOrOperation=6,
        PAny=7
    };

    enum OperationType
    {
        Add,
        Substract,
        Multiply
    };
    enum ParenthesisType
    {
        Left,
        Right
    };
    Symbol();
    static const Symbol createSubstitutable(const PossibleType& possible_types);
    static const Symbol createSymbol(const Type& t, const unsigned int& value);
    static const Symbol::OperationType getOperationByChar(const char r)
    {
        if(r=='+')
            return Symbol::Add;
        else if(r=='-')
            return Symbol::Substract;
        else if(r=='*')
            return Symbol::Multiply;
            //throw std::exception("Unsuported operator");
        return Symbol::Multiply;
    }
    unsigned int getDigit() const
    {
        return _value;
    }


    OperationType getOperation() const
    {
        return OperationType(_value);
    }
    ParenthesisType getParenthesis() const
    {
        return ParenthesisType(_value);
    }
    Type getType() const;
    bool canSubstitute(const Type& t) const;
    PossibleType getPossibleType() const
    {
        return _possible_types;
    }

    bool isSubstitutable() const
    {
        return _substitutable;
    }
    bool hasValue() const
    {
        return _value >= 0;
    }
    char toString() const;
    void substituteDigit(const unsigned int& digit)
    {
        assert(isSubstitutable() && canSubstitute(Digit));
        _type = Digit;
        _value = digit;
    }

    void substituteOperation(const OperationType& operation)
    {
        assert(isSubstitutable() && canSubstitute(Operation));
        _type = Operation;
        _value = operation;
    }

    void substituteParenthesis(const ParenthesisType& parensesis)
    {
        assert(isSubstitutable() && canSubstitute(Parenthesis));
        _type = Parenthesis;
        _value = parensesis;
    }

private:
    Symbol(const Type& t,
           const int& value);
    Symbol(const PossibleType& possible_types);
    Type _type;
    int _value; //if == -1 then it needs substitution
    bool _substitutable;

    PossibleType _possible_types;

};

//bool operator==(const Symbol& a, const Symbol& b);
#endif // SYMBOL_H
