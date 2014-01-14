#ifndef EQUATION_H
#define EQUATION_H
#include <string>
#include "Node.h"
#include "ExpressionParser.h"

class NodeHolder
{
public:
    NodeHolder(const std::string& expression, bool calculate_all);
    NodeHolder(const NodeHolder& helper);
    NodeHolder& operator=(const NodeHolder&);
    ~NodeHolder();
    unsigned int substitutionsNumber() const;
    void substituteDigit(const unsigned int& i, const unsigned int& new_digit);
    void substituteOperation(const unsigned int& i,
                             const Symbol::OperationType& op);
    void substituteParenthesis(const unsigned int& i,
                             const Symbol::ParenthesisType& p);
    //Symbol::PossibleType getSubstitutionPossibleType(const unsigned int& i) const;
    const Symbol& getSymbol(const unsigned int& i) const;
    bool isZeroSupportedForSubstitutedSymbol(const unsigned int i) const;
    const ExpressionResults& allResults() const
    {
        return _all_results;
    }
    const ExpressionResult& result() const
    {
        return _result;
    }
    const std::string toString() const;


    const std::string toStringRaw() const
    {
        return _raw_expression;
    }

    const std::string& getUntouchedString() const
    {
        return _untouched_expression;
    }
private:
    void rebuild();
    bool hasStructuralProblems() const;
    void checkStructure();
    void updateSubstitutions();
    void switchMode();
    enum Mode{
        PreSubstitution, //Make substitution when model can't be built
        //for example 2#4=6
        PostSubstitution //When the model can be build and intermidiate calculus
        //can be done. allResults can return something only in this mode
    };
    typedef Node::SubstitutionsArr SubstitutionsArr;
    std::string _raw_expression;
    std::string _untouched_expression;
    ExpressionResults _all_results;
    ExpressionResult _result;
    bool _calculate_all;
    Mode _mode;
    ExpressionParser::Expression _parsed_expression;
    SubstitutionsArr _pre_substitution;
};

class Equation
{
public:
    Equation(const std::string& lhs, const std::string& rhs,
             bool calculate_all=true);

    const ExpressionResults& leftAllResults() const
    {
        return _left.allResults();
    }
    const ExpressionResults& rightAllResults() const
    {
        return _right.allResults();
    }
    const ExpressionResult& leftResult() const
    {
        return _left.result();
    }
    const ExpressionResult& rightResult() const
    {
        return _right.result();
    }

    unsigned int substitutionsNumber() const;
    void substituteDigit(const unsigned int& i, const unsigned int& new_digit);
    void substituteOperation(const unsigned int& i,
                             const Symbol::OperationType& op);
    void substituteParenthesis(const unsigned int& i,
                             const Symbol::ParenthesisType& p);
    //Symbol::PossibleType getSubstitutionPossibleType(const unsigned int& i) const;
    const Symbol& getSubstitutedSymbol(const unsigned int& i) const;
    bool isZeroSupportedForSubstitutedSymbol(const unsigned int i) const;
    const std::string toString() const;
    const std::string toStringRaw() const;
    const std::string toStringLhs() const;
    const std::string toStringRhs() const;
    const std::string& getUntouchedLhsString() const;
    const std::string& getUntouchedRhsString() const;
private:
    NodeHolder _left;
    NodeHolder _right;
};

#endif // EQUATION_H
