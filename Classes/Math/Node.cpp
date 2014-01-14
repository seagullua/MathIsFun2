#include "Node.h"
#include <memory>
Node::Node(const Term& t)
    : _term(t), _a(0), _b(0), _has_substitutions(_term.hasSubstitutions())
{
}

Node::Node(const Term& t, const Node& a, const Node& b)
    :_term(t), _a(new Node(a)), _b(new Node(b)),
      _has_substitutions(_term.hasSubstitutions()
        || a.hasSubstitutions()
        || b.hasSubstitutions())
{
}

Node::Node(const Node& n)
    :_term(n._term),
      _a(n._a ? new Node(*n._a) : 0),
      _b(n._b ? new Node(*n._b) : 0),
      _has_substitutions(n._has_substitutions)
{
}

Node& Node::operator=(const Node& n)
{
    this->~Node();
    new (this) Node(n);
    return *this;
}

Node::~Node()
{
    if(_a != 0)
        delete _a;
    if(_b != 0)
        delete _b;
}


const std::string Node::toString(bool debug_output) const
{
    if(isLeaf())
        return _term.toString();
    else
    {
        std::string res("");
        if(debug_output)
            res+="(";
        if(_a)
            res+=_a->toString(debug_output);
        res+=_term.toString();
        if(_b)
            res+=_b->toString(debug_output);
        if(debug_output)
            res+=")";
        return res;
    }
}

