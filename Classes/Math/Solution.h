#ifndef SOLUTION_H
#define SOLUTION_H
#include <vector>
#include "Binary/Binary.h"
typedef std::vector<char> RawSubstitutionsArr;
class Solution
{
public:
    Solution(const RawSubstitutionsArr& arr=RawSubstitutionsArr(0));
    const RawSubstitutionsArr& getSubstitutions() const
    {
        return _substitutions_arr;
    }
    //Binary.h IO
    void write(OutputBinaryStream& os) const;
    void read(InputBinaryStream &is);
private:
    RawSubstitutionsArr _substitutions_arr;
};

inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const Solution& s)
{
    s.write(os);
    return os;
}

inline InputBinaryStream& operator>>(InputBinaryStream& is, Solution& s)
{
    s.read(is);
    return is;
}

typedef std::vector<Solution> Solutions;

bool operator==(const Solution& a, const Solution& b);
#endif // SOLUTION_H
