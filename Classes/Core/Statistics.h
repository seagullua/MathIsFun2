#ifndef STATISTICS_H
#define STATISTICS_H
#include <vector>
#include <string>
#include <sstream>
class Statistics
{
public:

    class Params
    {
    public:
        typedef std::vector<std::string> SVec;
        Params()
        {}

        Params& add(const std::string& key, const std::string& value)
        {
            _key_string.push_back(key);
            _value_string.push_back(value);
            return *this;
        }

        Params& add(const std::string& key, const int value)
        {
            _key_string.push_back(key);
            std::stringstream ss;
            ss << value;
            _value_string.push_back(ss.str());
            return *this;
        }

        const SVec& getKeysString() const
        {
            return _key_string;
        }



        const SVec& getValuesString() const
        {
            return _value_string;
        }


    private:
        SVec _key_string;
        SVec _value_string;
    };

    static void logEvent(const std::string& name, const Params& p=Params());
};
#endif // STATISTICS_H
