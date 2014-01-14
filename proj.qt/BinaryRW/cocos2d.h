#ifndef COCOS2DX_H
#define COCOS2DX_H
namespace cocos2d
{
    typedef unsigned char GLubyte;
    typedef struct _ccColor3B
    {
        GLubyte    r;
        GLubyte    g;
        GLubyte b;
    } ccColor3B;

    //! helper macro that creates an ccColor3B type
    static inline ccColor3B
    ccc3(const GLubyte r, const GLubyte g, const GLubyte b)
    {
        ccColor3B c = {r, g, b};
        return c;
    }
}
using namespace cocos2d;
#endif // COCOS2DX_H
