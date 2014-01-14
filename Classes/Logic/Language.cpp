#include "Language.h"
#include "Core/Statistics.h"
#include <map>
void Language::setLanguage(const std::string& lang)
{

    typedef std::map<std::string, Language::Languages> LMap;
    LMap supported_languages;

    supported_languages["ru"] = Language::Russian;
    supported_languages["nl"] = Language::Dutch;
    supported_languages["fr"] = Language::French;
    supported_languages["de"] = Language::German;
    supported_languages["hu"] = Language::Hungarian;
    supported_languages["it"] = Language::Italian;
    supported_languages["ja"] = Language::Japanese;
    supported_languages["ko"] = Language::Korean;
    supported_languages["pl"] = Language::Polish;
    supported_languages["pt"] = Language::Portuges;
    supported_languages["es"] = Language::Spanish;
    supported_languages["tr"] = Language::Turkish;
    supported_languages["uk"] = Language::Ukrainian;

    LMap::iterator it = supported_languages.find(lang);
    if(it == supported_languages.end())
    {
        _lang = Language::English;
        _lang_code = "en";
        Statistics::logEvent("Translation not found", Statistics::Params()
                             .add("Language", lang));
    }
    else
    {
        _lang = it->second;
        _lang_code = it->first;

        Statistics::logEvent("Translation found", Statistics::Params()
                             .add("Language", lang));
    }
}

Language::Languages Language::getLanguage()
{
    return _lang;
}
const std::string& Language::getLangCode()
{
    return _lang_code;
}

std::string Language::localizeFileName(const std::string& file_name)
{

    unsigned int last_dot=0;
    for(unsigned int i=0; i<file_name.size(); ++i)
    {
        if(file_name[i] == '.')
            last_dot = i;
    }
    std::string res(file_name.begin(), file_name.begin()+last_dot+1);
    res.reserve(file_name.size() + 3);
    res += _lang_code;
    res += ".";
    res += &file_name[last_dot+1];
    return res;

}

Language::Languages Language::_lang = Language::English;
std::string Language::_lang_code = "en";
