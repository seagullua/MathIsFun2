#include "Browser.h"

const std::string Browser::MAIL_PREFIX = "mailto:";
const std::string Browser::MARKET_PERFIX = "market:";

bool hasPrefix(const std::string& s, const std::string& prefix)
{
    std::string real_prefix = s.substr(0, prefix.size());
    return real_prefix == prefix;
}

std::string removePrefix(const std::string& s, const std::string& prefix)
{
    return s.substr(prefix.size(), s.size() - prefix.size());
}

void Browser::open(const std::string& url)
{
    if(hasPrefix(url, MAIL_PREFIX))
    {
        sendMail(removePrefix(url, MAIL_PREFIX));
    }
    else if(hasPrefix(url, MARKET_PERFIX))
    {
        openMarketItem(removePrefix(url, MARKET_PERFIX));
    }
    else
    {
        openWebURL(url);
    }
}

std::string Browser::createRedirectURL(const std::string& code)
{
    return "http://4enjoy.com/r/auto/"+code+"/";
}
