#include "ExpressionParser.h"
#include <stack>
#include <cassert>
#include <exception>
#include <sstream>

const std::string ExpressionResult::toString() const
{
    std::stringstream ss;
    ss << expression() << "=" << result() << std::endl;
    return ss.str();
}

bool ExpressionResult::isSimpleRes() const
{
    if(!isEvaluated())
        return false;

    unsigned int res_size = 1;
    MInt res = result();
    res /= 10;
    while(res)
    {
        res_size++;
        res /= 10;
    }
    if(res_size == expression().size())
        return true;
    return false;
}

class Parser
{
public:
    Parser(const ExpressionParser::Expression& expression)
        : _expression(expression), _node_buffer(0)
    {

    }
    bool parse()
    {
        for(unsigned int idx=0; idx<_expression.size(); ++idx)
        {
            Symbol cur = _expression[idx];
            if(cur.getType() == Symbol::Digit)
            {
                //This is digit
                _digits_buffer.push_back(cur);
            }
            else if(cur.getType() == Symbol::Operation)
            {
                //Operation
                if(!numberReadEnd())
                    return false;
                //Symbol::OperationType op = getOperationByChar(cur);
                if(!evaluate(getPriority(cur)))
                    return false;
                Term t(Term::Operation,cur);
                _operations.push(t);
            }
            else if(cur.getType() == Symbol::Parenthesis)
            {
                if(cur.getParenthesis() == Symbol::Right) //To many )
                    return false;

                if(_digits_buffer.size()!=0) //expression like 5(...)
                    return false;

                if(_node_buffer!=0) //expressiong like (...)(...)
                    return false;

                int opened = 1;
                unsigned int balance_idx = _expression.size();
                for(unsigned int i=idx+1; i<_expression.size(); ++i)
                {
                    Symbol pcur(_expression[i]);
                    if(pcur.getType() == Symbol::Parenthesis)
                    {
                        if(pcur.getParenthesis() == Symbol::Left)
                            opened++;
                        else if(pcur.getParenthesis() == Symbol::Right)
                        {
                            if(opened==0) //To much )
                                return false;
                            opened--;
                            if(opened==0) //Balance is here
                            {
                                balance_idx=i;
                                break;
                            }
                        }
                    }
                }
                if(balance_idx < _expression.size())
                {
                    //Take part and evaluate it
                    ExpressionParser::Expression inside(_expression.begin()+idx+1,
                                                        _expression.begin()+balance_idx);
                    Parser p(inside);
                    if(!p.parse())
                        return false;
                    _node_buffer = new Node(p.getParsedExpression());
                    idx=balance_idx;
                }
                else
                    return false;
            }
            else
            {
                //throw "Unsupported symbol";
                return false;
            }
        }
        if(!numberReadEnd())
            return false;
        if(!evaluate())
            return false;
        //assert(_numbers.size() == 1);
        if(_numbers.size()!=1)
            return false;
        return true;
    }
    const Node& getParsedExpression()
    {
        assert(_numbers.size() == 1);
        return _numbers.top();
    }

private:
    std::stack<Node> _numbers;
    std::stack<Term> _operations;
    ExpressionParser::Expression _expression;
    Term::SymbolArr _digits_buffer;
    Node* _node_buffer;
    /**
     * @brief Get priority of the operation
     * @param op
     * @return
     */
    int getPriority(const Symbol& s)
    {
        Symbol::OperationType op = s.getOperation();
        if(op == Symbol::Multiply)
            return 3;
        else if(op == Symbol::Substract)
            return 2;
        return 1;
    }



    /**
     * @brief When parser meets not digit this method is called
     */
    bool numberReadEnd()
    {
        if(_node_buffer!=0)
        {
            //Add some (...) to numbers
            if(!_digits_buffer.size()==0)
            {
                //assert(_digits_buffer.size()==0);
                return false;
            }
            _numbers.push(*_node_buffer);
            delete _node_buffer;
            _node_buffer=0;
        }
        else
        {
            //The situation when to operators are near 5++6
            if(_digits_buffer.size() == 0)
            {
                //throw "Wrong syntax";
                return false;
            }

            Term number(_digits_buffer);
            _digits_buffer.clear();

            _numbers.push(number);
        }
        return true;
    }

    bool evaluate(const int max_priority=0)
    {

        if(!_operations.empty())
        {
            bool evaluation_needed = false;
            if(_numbers.size() < 2)
            {
                //throw "Wrong syntax";
                return false;
            }
            //int cur_priority = getPriority(_operations.top().getSymbol());
            if(evaluation_needed ||
                    max_priority <= getPriority(_operations.top().getSymbol()))
            {

                Node b = _numbers.top();
                _numbers.pop();
                Node a = _numbers.top();
                _numbers.pop();

                /*if(cur_priority == max_priority && a.hasSubstitutions())
                {
                    _numbers.push(a);
                    _numbers.push(b);
                }
                else
                {*/
                    Term t = _operations.top();
                    _operations.pop();

                    Node new_node(t, a, b);
                    _numbers.push(new_node);

                    if(!evaluate(max_priority))
                        return false;
                //}

            }
        }
        return true;
    }
};

/**
 * @brief Format
 * _ - any digit
 * # - any operation
 * $ - any parenthesis
 * @ - digit or operation
 * & - digit or parenthesis
 * % - operation or parenthesis
 * A - digit or operation or parenthesis
 */
class EqReader
{
public:
    /**
     * @brief Convert char to operation
     * @param r
     * @return
     */
    static Symbol::OperationType getOperationByChar(const char r)
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
    static const ExpressionParser::Expression parse(const std::string& str)
    {
        ExpressionParser::Expression res(0);
        res.reserve(str.size());
        for(unsigned int i=0; i<str.size(); ++i)
        {
            char cur = str[i];
            if(cur == '_')
                res.push_back(Symbol::createSubstitutable(Symbol::PDigit));
            else if(cur == '#')
                res.push_back(Symbol::createSubstitutable(Symbol::POperation));
            else if(cur == '$')
                res.push_back(Symbol::createSubstitutable(
                                  Symbol::PParenthesis));
            else if(cur == '@')
                res.push_back(Symbol::createSubstitutable(
                                  Symbol::PDigitOrOperation));
            else if(cur == '&')
                res.push_back(Symbol::createSubstitutable(
                                  Symbol::PParenthesisOrDigit));
            else if(cur == '%')
                res.push_back(Symbol::createSubstitutable(
                                  Symbol::PParenthesisOrOperation));
            else if(cur == 'A')
                res.push_back(Symbol::createSubstitutable(
                                  Symbol::PAny));
            else if(cur == '+' || cur == '-' || cur == '*')
                res.push_back(Symbol::createSymbol(Symbol::Operation, getOperationByChar(cur)));
            else if((cur >= '0' && cur <= '9'))
                res.push_back(Symbol::createSymbol(Symbol::Digit, cur-'0'));
            else if(cur == '(')
                res.push_back(Symbol::createSymbol(Symbol::Parenthesis, Symbol::Left));
            else if(cur == ')')
                res.push_back(Symbol::createSymbol(Symbol::Parenthesis, Symbol::Right));
            else
                assert(false);
        }
        return res;
    }
};
class Eval
{
public:
    Eval(const Node& node, bool save_evaluations=false)
        :_node(&node), _save_all_evaluations(save_evaluations), _all_evaluations(0)
    {

    }

    ExpressionResult evaluate()
    {
        return do_evaluate(_node);
    }

    const ExpressionResults& getAllEvaluations()
    {
        return _all_evaluations;
    }

private:
    ExpressionResult do_evaluate(const Node* node)
    {
        if(node->isLeaf())
        {
            //Leaf evaluation
            assert(node->getTerm().getType() == Term::Number);
            if(node->getTerm().numberCanBeEvaluated())
            {
                ExpressionResult res(node->getTerm().toString(),
                                     node->getTerm().toNumber());
                return res;
            }
            //We can't evaluate it
            return ExpressionResult();
        }
        else
        {
            const Node* left = node->a();
            const Node* right = node->b();

            ExpressionResult left_eval = do_evaluate(left);
            ExpressionResult right_eval = do_evaluate(right);

            if(left_eval.isEvaluated() && right_eval.isEvaluated())
            {
                //We can evaluate this node
                MInt a = left_eval.result();
                MInt b = right_eval.result();
                MInt res = 0;
                Symbol::OperationType op = node->getTerm().getSymbol().getOperation();
                if(op == Symbol::Add)
                    res = a + b;
                else if(op == Symbol::Substract)
                    res = a - b;
                else if(op == Symbol::Multiply)
                    res = a * b;
                std::string expr_str = left_eval.expression();
                expr_str += node->getTerm().toString();
                expr_str += right_eval.expression();
                ExpressionResult expr_res(expr_str, res);
                if(_save_all_evaluations)
                    _all_evaluations.push_back(expr_res);
                return expr_res;

            }
            return ExpressionResult(); //We can't evaluate this node
        }
    }
    const Node* _node;
    bool _save_all_evaluations;
    ExpressionResults _all_evaluations;

};

Node* ExpressionParser::parseExpression(const Expression& expression)
{
    Parser parser(expression);
    if(parser.parse())
        return new Node(parser.getParsedExpression());
    else
        return 0;
}

const ExpressionResult ExpressionParser::evaluate(const Node& node)
{
    Eval eval(node, false);
    return eval.evaluate();
}
const ExpressionParser::Expression ExpressionParser::parse(const std::string& expression)
{
    return EqReader::parse(expression);
}

const ExpressionResults ExpressionParser::getAllEvaluations(const Node& node)
{
    Eval eval(node, true);
    eval.evaluate();
    return eval.getAllEvaluations();
}
