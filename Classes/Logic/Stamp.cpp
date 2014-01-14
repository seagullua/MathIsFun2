#include "Stamp.h"

const std::string Stamp::getFileName() const
{
    std::string res("stamp_@#.png");
    res[6] = char(_color);
    res[7] = '0' + _id;
    return res;
}

Stamp::Stamp(const Color& color,
      const unsigned int& id,
      const int& rotation, bool is_new)
    : _color(color), _id(id), _rotation(rotation), _is_new(is_new)
{

}
