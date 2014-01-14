#ifndef STAMP_H
#define STAMP_H
#include <string>
class Stamp
{
public:
    enum Color
    {
        Blue=98,
        Green=103,
        Red=114
    };
    const Color& getColor() const
    {
        return _color;
    }
    const unsigned int& getID() const
    {
        return _id;
    }
    const int& getRotation() const
    {
        return _rotation;
    }
    bool newStamp() const
    {
        return _is_new;
    }
    const std::string getFileName() const;
    Stamp(const Color& = Blue,
          const unsigned int& id=0,
          const int &rotation=0,
          bool is_new=false);
private:
    Color _color;
    unsigned int _id;
    int _rotation;
    bool _is_new;
};

#endif // STAMP_H
