#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <string>
class Language
{
public:
    enum Languages
    {
        English=0,
        Russian=1,
        Spanish=2,
        Japanese=3,
        Portuges=4,
        German=5,
        French=6,
        Korean=7,
        Turkish=8,
        Italian=9,
        Ukrainian=10,
        Dutch=11,
        Polish=12,
        Hungarian=13

    };

    static void setLanguage(const std::string& lang_code);
    static Languages getLanguage();
    static const std::string& getLangCode();
    static std::string localizeFileName(const std::string& file_name);



    static std::string getDeviceLanguage();
private:
    static Languages _lang;
    static std::string _lang_code;

    Language();
};

#endif // LANGUAGE_H
