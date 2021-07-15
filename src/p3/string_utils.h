/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#pragma once

#include <string>
#include <regex>

namespace p3
{

    inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    inline void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
    }

    inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }

    inline std::vector<std::string> tokenize(std::string const& input, std::string const& regex, int sup = -1) 
    {
        std::regex re(regex);
        std::sregex_token_iterator first{ input.begin(), input.end(), re, sup }, last;
        return { first, last };
    }

    inline bool ends_with(std::string const &str, std::string const &ending) 
    {
        if (str.length() >= ending.length()) {
            return (0 == str.compare (str.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }

}
