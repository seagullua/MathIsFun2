#ifndef SOLUTION_H
#define SOLUTION_H
#include <vector>
#include <ADLib/Storage/ADStream.h>

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
    void write(ADStreamOut& os) const;
    void read(ADStreamIn &is);
private:
    RawSubstitutionsArr _substitutions_arr;
};

inline ADStreamOut& operator<<(ADStreamOut& os, const Solution& s)
{
    s.write(os);
    return os;
}

inline ADStreamIn& operator>>(ADStreamIn& is, Solution& s)
{
    s.read(is);
    return is;
}

typedef std::vector<Solution> Solutions;

bool operator==(const Solution& a, const Solution& b);
#endif // SOLUTION_H
