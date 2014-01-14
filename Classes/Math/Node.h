#ifndef NODE_H
#define NODE_H
#include "Term.h"



class Node
{
public:
    typedef Term::SubstitutionsArr SubstitutionsArr;
    Node(const Term&);
    Node(const Term&, const Node&, const Node&);
    Node(const Node&);
    Node& operator=(const Node&);
    ~Node();
    bool hasSubstitutions() const
    {
        return _has_substitutions;
    }

    bool isLeaf() const
    {
        return _a == 0 || _b == 0;
    }
    const Node* a() const
    {
        return _a;
    }
    const Node* b() const
    {
        return _b;
    }
    const Term& getTerm() const
    {
        return _term;
    }
    const std::string toString(bool debug_output=false) const;
private:
    Term _term;
    Node* _a;
    Node* _b;
    bool _has_substitutions;
};

#endif // NODE_H
