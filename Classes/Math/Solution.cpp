#include "Solution.h"

Solution::Solution(const RawSubstitutionsArr& arr)
    : _substitutions_arr(arr)
{}

bool operator==(const Solution& a, const Solution& b)
{
    const RawSubstitutionsArr& aw = a.getSubstitutions();
    const RawSubstitutionsArr& bw = b.getSubstitutions();
    if(aw.size() != bw.size())
        return false;

    unsigned int size = aw.size();
    for(unsigned int i=0;i<size;++i)
    {
        if(aw[i] != bw[i])
            return false;
    }
    return true;
}

void Solution::write(OutputBinaryStream& os) const
{
    os << _substitutions_arr;
}
void Solution::read(InputBinaryStream &is)
{
    is >> _substitutions_arr;
}
