#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H
#include "Node.h"
#include <memory>
class ExpressionResult
{
public:
    ExpressionResult(const std::string& exp, const MInt& res)
        : _expression(exp), _result(res), _evaluated(true)
    {}
    ExpressionResult()
        :_expression(""), _result(0), _evaluated(false)
    {}

    const MInt& result() const
    {
        return _result;
    }
    const std::string& expression() const
    {
        return _expression;
    }
    bool isEvaluated() const
    {
        return _evaluated;
    }
    bool isSimpleRes() const;
    const std::string toString() const;
private:
    std::string _expression;
    MInt _result;
    bool _evaluated;
};
typedef std::vector<ExpressionResult> ExpressionResults;
typedef std::auto_ptr<Node> NodePtr;
class ExpressionParser
{
public:
    typedef Term::SymbolArr Expression;
    static Node* parseExpression(const Expression& expression);
    static const Expression parse(const std::string& expression);
    static const ExpressionResult evaluate(const Node& node);
    static const ExpressionResults getAllEvaluations(const Node& node);
private:
    ExpressionParser();
};

#endif // EXPRESSIONPARSER_H
