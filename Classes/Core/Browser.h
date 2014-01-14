#ifndef BROWSER_H
#define BROWSER_H
#include <string>
class Browser
{
public:
    enum URLId
    {
        FacebookGroup = 100,
        Site4Enjoy = 200,
        sendEmail = 300
    };

    static std::string createRedirectURL(const std::string& code);
    static void open(const std::string& url);
    static void openWebURL(const std::string& url);
    static void openMarketItem(const std::string& item_id);
    static void sendMail(const std::string& email, const std::string &subject="");

    static const std::string MAIL_PREFIX;
    static const std::string MARKET_PERFIX;
};

#endif // BROWSER_H
