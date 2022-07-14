
#pragma once

#include <regex>
#include <string>

namespace p3 {

inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(),
        s.end());
}

inline void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

inline std::vector<std::string> tokenize(std::string const& input, std::string const& regex, int sup = -1)
{
    std::regex re(regex);
    std::sregex_token_iterator first { input.begin(), input.end(), re, sup }, last;
    return { first, last };
}

inline bool ends_with(std::string const& str, std::string const& ending)
{
    if (str.length() >= ending.length()) {
        return (0 == str.compare(str.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

}
