#ifndef ENCSTRINGS_H
#define ENCSTRINGS_H
#include <string>

inline std::string stringDecode(const unsigned int* a, const unsigned int* b, const unsigned int size)
{
    std::string res(size, ' ');
    for(unsigned int i=0; i<size; ++i)
        res[i] = char(a[i] ^ b[i]);
    return res;
}
#define CODE(...) {__VA_ARGS__}
#define CREATE_DECODED_STRING(variable, code, password, size) std::string variable; \
{ \
    unsigned int x[]=code; \
    unsigned int y[]=password; \
    variable = stringDecode(x, y, size); \
}

#endif // ENCSTRINGS_H
