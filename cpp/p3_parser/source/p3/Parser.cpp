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
#include "Parser.h"

#include <cctype>
#include <regex>

namespace p3::parser
{

    namespace tokenizer
    {

        pos name(pos input)
        {
            const char* ptr = input;
            while (std::isalpha(static_cast<unsigned char>(*ptr)))
                ++ptr;
            if (ptr == input)
                return input;
            while (*ptr == '_' || std::isalpha(static_cast<unsigned char>(*ptr)))
                ++ptr;
            return ptr;
        }

        namespace expression
        {
            std::regex floating_point(R"(^[-+]?([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][-+]?[0-9]+)?)");
        }
        pos floating_point(pos input)
        {
            std::cmatch match;
            if (std::regex_search(input, match, expression::floating_point))
                return match[0].second;
            return input;
        }

        namespace expression
        {
            std::regex comment(R"(\/\*([^*]|[\r\n]|(\*+([^*\/]|[\r\n])))*\*+\/)");
        }
        pos comment(pos input)
        {
            std::cmatch match;
            if (std::regex_search(input, match, expression::comment))
                return match[0].second;
            return input;
        }

        pos hex_color(pos pos)
        {
            if (*pos != '#')
                return pos;
            auto it = pos + 1;
            if (!std::isxdigit(*it))
                return pos;
            ++it;
            if (!std::isxdigit(*it))
                return pos;
            ++it;
            if (!std::isxdigit(*it))
                return pos;
            ++it;
            if (!std::isxdigit(*it))
                return it;
            ++it;
            if (!std::isxdigit(*it))
                return pos;
            ++it;
            if (!std::isxdigit(*it))
                return pos;
            ++it;
            if (!std::isxdigit(*it))
                return it;
            ++it;
            if (!std::isxdigit(*it))
                return pos;
            return ++it;
        }

        pos px(pos begin)
        {
            return std::strncmp(begin, "px", 2) == 0 ? &begin[2] : begin;
        }

        pos em(pos begin)
        {
            return std::strncmp(begin, "em", 2) == 0 ? &begin[2] : begin;
        }

        pos rem(pos begin)
        {
            return std::strncmp(begin, "rem", 3) == 0 ? &begin[3] : begin;
        }

        pos auto_(pos begin)
        {
            return std::strncmp(begin, "auto", 4) == 0 ? &begin[4] : begin;
        }

    } // tokenizer

    pos skip_whitespace(pos it)
    {
        while (true)
            if (std::isspace(static_cast<unsigned char>(*it)))
                ++it;
            else if (auto temp = tokenizer::comment(it); temp != it)
                it = temp;
            else
                break;
        return it;
    }

    pos Rule<Length, void>::parse(pos begin, Length& length)
    {
        auto it = skip_whitespace(begin);
        float value;
        if (auto temp = tokenizer::floating_point(it); it != temp)
        {
            value = std::stof(std::string(it, temp));
            it = temp;
        }
        else
            return begin;
        if (auto temp = tokenizer::px(it); it != temp)
        {
            length = Px{ value };
            return temp;
        }
        if (auto temp = tokenizer::em(it); it != temp)
        {
            length = Em{ value };
            return temp;
        }
        if (auto temp = tokenizer::rem(it); it != temp)
        {
            length = Rem{ value };
            return temp;
        }
        return begin;
    }

    pos Rule<Percentage, void>::parse(pos begin, Percentage& percentage)
    {
        auto it = skip_whitespace(begin);
        if (auto temp = tokenizer::floating_point(it); it != temp)
        {
            percentage.value = std::stof(std::string(it, temp));
            it = temp;
        }
        else
            return begin;
        return *it == '%' ? it + 1 : begin;
    }

    pos Rule<float, void>::parse(pos begin, float& value)
    {
        auto it = skip_whitespace(begin);
        if (auto temp = tokenizer::floating_point(it); temp != it)
        {
            value = std::stof(std::string(it, temp));
            return temp;
        }
        return begin;
    }

    pos Rule<LengthPercentage, void>::parse(pos begin, LengthPercentage& length_percentrage)
    {
        Length length;
        if (auto it = parser::parse(begin, length); it != begin)
        {
            length_percentrage = length;
            return it;
        }
        Percentage percentage;
        if (auto it = parser::parse(begin, percentage); it != begin)
        {
            length_percentrage = percentage;
            return it;
        }
        return begin;
    }

    pos Rule<Color, void>::parse(pos begin, Color& color)
    {
        auto it = skip_whitespace(begin);
        if (auto temp = tokenizer::hex_color(it); it != temp)
        {
            color.value = std::stoi(std::string(it + 1, temp), 0, 16);
            return temp;
        }
        return begin;
    }

    pos Rule<OptionalLengthPercentage, void>::parse(pos begin, OptionalLengthPercentage& olp)
    {
        auto it = skip_whitespace(begin);
        if (auto temp = tokenizer::auto_(it); it != temp)
        {
            olp.reset();
            return temp;
        }
        LengthPercentage length_percentage;
        if (auto temp = parser::parse<LengthPercentage>(begin, length_percentage); temp != it)
        {
            olp = std::move(length_percentage);
            return temp;
        }
        return begin;
    }

    pos Rule<LayoutLength, void>::parse(pos begin, LayoutLength& fd)
    {
        auto it = begin;
        if (auto temp = parser::parse<OptionalLengthPercentage>(it, std::get<0>(fd)); it == temp)
            return begin;
        else
            it = temp;
        if (auto temp = parser::parse<float>(it, std::get<1>(fd)); it == temp)
            return begin;
        else
            it = temp;
        if (auto temp = parser::parse<float>(it, std::get<2>(fd)); it == temp)
            return begin;
        else
            return it;
    }

}
