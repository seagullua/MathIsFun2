#ifndef BROWSER_H
#define BROWSER_H
#include <string>

#include <ADLib/Device/ADBrowser.h>

inline std::string createRedirectURL(const std::string& code)
{
    return "http://4enjoy.com/r/auto/"+code+"/";
}

#endif // BROWSER_H
