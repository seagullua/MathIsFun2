#include "Equation.h"
bool NodeHolder::hasStructuralProblems() const
{
    bool structural_problems_found = false;
    if(_mode == PreSubstitution)
    {
        for(unsigned int i=0; i<_parsed_expression.size(); ++i)
        {
            if(_parsed_expression[i].getType() == Symbol::Operation
                    && _parsed_expression[i].isSubstitutable()
                    && !_parsed_expression[i].hasValue())
            {
                //This operation symbol don't have value, we can't build structure
                structural_problems_found = true;
                break;
            }
        }
    }
    return structural_problems_found;
}
NodeHolder::NodeHolder(const std::string& expression, bool calculate_all)
    : _raw_expression(expression),
      _all_results(0),
      _result(),
      _calculate_all(calculate_all),
      _mode(PreSubstitution),
      _parsed_expression(ExpressionParser::parse(expression)),
      _pre_substitution(0),
      _untouched_expression(expression)
{
    updateSubstitutions();
    checkStructure();
}
void NodeHolder::updateSubstitutions()
{
    _pre_substitution.clear();
    //We should create substitution buffer
    for(unsigned int i=0; i<_parsed_expression.size(); ++i)
    {
        Symbol* p = &_parsed_expression[i];
        if(p->isSubstitutable())
        {
            _pre_substitution.push_back(p);
            _raw_expression[i] = '_'; //Any substitution looks the same
        }
    }
}

NodeHolder::NodeHolder(const NodeHolder& h)
    : _raw_expression(h._raw_expression),
      _all_results(h._all_results),
      _result(h._result),
      _calculate_all(h._calculate_all),
      _mode(h._mode),
      _parsed_expression(h._parsed_expression),
      _pre_substitution(h._pre_substitution),
      _untouched_expression(h._untouched_expression)
{
    updateSubstitutions();
}

NodeHolder& NodeHolder::operator=(const NodeHolder& n)
{
    this->~NodeHolder();
    new (this) NodeHolder(n);
    return *this;
}

NodeHolder::~NodeHolder()
{
}

unsigned int NodeHolder::substitutionsNumber() const
{
    //if(_mode == PreSubstitution)
    return _pre_substitution.size();
    //else
    //    return _structured_expression->substitutions().size();
}
void NodeHolder::switchMode()
{
    _mode = PostSubstitution;
    //We can use structured expression
    rebuild();
}
bool NodeHolder::isZeroSupportedForSubstitutedSymbol(const unsigned int i) const
{
    assert(i < substitutionsNumber());
    Symbol* cur = _pre_substitution[i];

    const Symbol* last = &_parsed_expression[_parsed_expression.size()-1];
    if(cur == last)
        return true;

    const Symbol* first = &_parsed_expression[0];


    Symbol* next = cur + 1;




    if(cur == first)
    {
        if(next->canSubstitute(Symbol::Digit))
            return false;
        return true;
    }
    else
    {
        Symbol* prev = cur - 1;

        if(!next->canSubstitute(Symbol::Digit))
            return true;

        if(prev->canSubstitute(Symbol::Digit))
        {
            if(prev->canSubstitute(Symbol::Parenthesis) || prev->canSubstitute(Symbol::Operation))
                return false;
            else
                return true;
        }
        else
        {
            if(next->canSubstitute(Symbol::Digit))
                return false;

        }

    }
    return true;

}
void NodeHolder::checkStructure()
{
    if(!hasStructuralProblems())
    {
        switchMode();
    }
}
void NodeHolder::substituteDigit(const unsigned int& i, const unsigned int& new_digit)
{
    assert(i < substitutionsNumber());
    _pre_substitution[i]->substituteDigit(new_digit);
    if(_mode == PreSubstitution)
        checkStructure();
    else
    {
        //_structured_expression->substitutions()[i]->substituteDigit(new_digit);
        rebuild();
    }
}
void NodeHolder::substituteOperation(const unsigned int& i,
                                     const Symbol::OperationType& op)
{
    assert(i < substitutionsNumber());
    _pre_substitution[i]->substituteOperation(op);
    if(_mode == PreSubstitution)
        checkStructure();
    else
    {
        //_structured_expression->substitutions()[i]->substituteOperation(op);
        rebuild();
    }
}

void NodeHolder::substituteParenthesis(const unsigned int& i,
                                       const Symbol::ParenthesisType& p)
{
    assert(i < substitutionsNumber());
    _pre_substitution[i]->substituteParenthesis(p);
    if(_mode == PreSubstitution)
        checkStructure();
    else
    {
        //_structured_expression->substitutions()[i]->substituteParenthesis(p);
        rebuild();
    }
}

const Symbol& NodeHolder::getSymbol(const unsigned int& i) const
{
    assert(i < substitutionsNumber());
    return *_pre_substitution[i];
}
const std::string NodeHolder::toString() const
{
    std::string res(_parsed_expression.size(), ' ');
    for(unsigned int i=0; i<_parsed_expression.size(); ++i)
    {
        res[i] = _parsed_expression[i].toString();
    }
    return res;
}
void NodeHolder::rebuild()
{
    assert(_mode == PostSubstitution);
    NodePtr tmp_node(ExpressionParser::parseExpression(_parsed_expression));
    _all_results.clear();
    if(_calculate_all && tmp_node.get())
    {
        _all_results = ExpressionParser::getAllEvaluations(*tmp_node);
    }
    if(tmp_node.get())
        _result = ExpressionParser::evaluate(*tmp_node);
    else
        _result = ExpressionResult();

}

Equation::Equation(const std::string &lhs, const std::string &rhs,
                   bool calculate_all)
    : _left(lhs, calculate_all), _right(rhs, calculate_all)
{
}



unsigned int Equation::substitutionsNumber() const
{
    return _left.substitutionsNumber() + _right.substitutionsNumber();
}
void Equation::substituteDigit(const unsigned int& i, const unsigned int& new_digit)
{
    assert(i < substitutionsNumber());

    if(i < _left.substitutionsNumber())
        _left.substituteDigit(i, new_digit);
    else
        _right.substituteDigit(i - _left.substitutionsNumber(),new_digit);

}
void Equation::substituteOperation(const unsigned int& i,
                                   const Symbol::OperationType& op)
{
    assert(i < substitutionsNumber());

    if(i < _left.substitutionsNumber())
        _left.substituteOperation(i, op);
    else
        _right.substituteOperation(i - _left.substitutionsNumber(),op);
}

void Equation::substituteParenthesis(const unsigned int& i,
                                     const Symbol::ParenthesisType& p)
{
    assert(i < substitutionsNumber());

    if(i < _left.substitutionsNumber())
        _left.substituteParenthesis(i, p);
    else
        _right.substituteParenthesis(i - _left.substitutionsNumber(),p);
}
const Symbol& Equation::getSubstitutedSymbol(const unsigned int& i) const
{
    assert(i < substitutionsNumber());

    if(i < _left.substitutionsNumber())
        return _left.getSymbol(i);
    else
        return _right.getSymbol(i - _left.substitutionsNumber());
}
bool Equation::isZeroSupportedForSubstitutedSymbol(const unsigned int i) const
{
    assert(i < substitutionsNumber());

    if(i < _left.substitutionsNumber())
        return _left.isZeroSupportedForSubstitutedSymbol(i);
    else
        return _right.isZeroSupportedForSubstitutedSymbol(i - _left.substitutionsNumber());
}
const std::string Equation::toString() const
{
    std::string result = _left.toString();
    result += '=';
    result += _right.toString();
    return result;
}
const std::string Equation::toStringRaw() const
{
    std::string result = _left.toStringRaw();
    result += '=';
    result += _right.toStringRaw();
    return result;
}
const std::string Equation::toStringLhs() const
{
    return _left.toStringRaw();
}
const std::string Equation::toStringRhs() const
{
    return _right.toStringRaw();
}
const std::string& Equation::getUntouchedLhsString() const
{
    return _left.getUntouchedString();
}
const std::string& Equation::getUntouchedRhsString() const
{
    return _right.getUntouchedString();
}
