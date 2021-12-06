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

#include <cstdint>
#include <unordered_map>
#include <string>
#include <sstream>
#include <optional>
#include <iomanip>

namespace p3
{

    class Color
    {
    public:
        Color() = default;
        Color(Color const&) = default;
        Color(Color&&) = default;

        // no explicit
        Color(std::string const&);

        // no explicit
        Color(std::uint32_t rgba);

        // no explicit
        Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha);

        Color& operator=(Color const&) = default;
        Color& operator=(Color&&) = default;

        bool operator==(Color const&) const;

        std::uint8_t red() const;
        std::uint8_t green() const;
        std::uint8_t blue() const;
        std::uint8_t alpha() const;

        struct named;

        operator std::uint32_t() const;

        static Color from_hex(std::string const&);
        static Color from_string(std::string const& input);

    private:
        std::uint8_t _red;
        std::uint8_t _green;
        std::uint8_t _blue;
        std::uint8_t _alpha;
        static std::unordered_map<std::string, Color const&> _lut;
    };

    namespace
    {
        inline Color _from_hex(std::string const& input)
        {
            if (input.size() == 6)
                return static_cast<std::uint32_t>(stoul(input, nullptr, 16)) << 8 | 0xFF;
            if (input.size() == 8)
                return static_cast<std::uint32_t>(stoul(input, nullptr, 16));
            else if (input.size() == 3)
            {
                auto x = static_cast<std::uint32_t>(stoul(input, nullptr, 16));
                return  ((x & 0x00F) << 4 | (x & 0x00F) << 0 | (x & 0x0F0) << 8 | (x & 0x0F0) << 4 | (x & 0xF00) << 12 | (x & 0xF00) << 8) << 8 | 0xFF;
            }
            else
                throw std::runtime_error("could not create Color from \"" + input + "\"");
        }
    }

    inline Color Color::from_hex(std::string const& input)
    {
        auto l = input.size();
        if (input.empty())
            return 0u;
        return _from_hex(input[0] == '#' ? input.substr(1) : input);
    }

    inline Color Color::from_string(std::string const& input)
    {
        if (input.empty())
            return Color();
        if (input[0] == '#')
            return from_hex(input);
        auto it = _lut.find(input);
        if (it != _lut.end())
            return it->second;
        throw std::runtime_error("failed to parse Color \"" + input + "\"");
    }

    inline Color::Color(std::uint32_t rgba)
        : _red(rgba >> 24 & 0x000000FF)
        , _green(rgba >> 16 & 0x000000FF)
        , _blue(rgba >> 8 & 0x000000FF)
        , _alpha(rgba >> 0 & 0x000000FF)
    {
    }

    inline Color::Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
        : _red(red)
        , _green(green)
        , _blue(blue)
        , _alpha(alpha)
    {
    }

    inline Color::Color(std::string const& str)
    {
        *this = from_string(str);
    }

    inline Color::operator std::uint32_t() const
    {
        return static_cast<std::uint32_t>(_red) << 24
            | static_cast<std::uint32_t>(_green) << 16
            | static_cast<std::uint32_t>(_blue) << 8
            | static_cast<std::uint32_t>(_alpha);
    }

    inline bool Color::operator==(Color const& color) const
    {
        return _red == color._red
            && _green == color._green
            && _blue == color._blue
            && _alpha == color._alpha;
    }

    inline std::uint8_t Color::red() const
    {
        return _red;
    }

    inline std::uint8_t Color::green() const
    {
        return _green;
    }

    inline std::uint8_t Color::blue() const
    {
        return _blue;
    }

    inline std::uint8_t Color::alpha() const
    {
        return _alpha;
    }

    struct Color::named
    {
        inline static const auto black = Color(0x000000ff);
        inline static const auto silver = Color(0xc0c0c0ff);
        inline static const auto gray = Color(0x808080ff);
        inline static const auto white = Color(0xffffffff);
        inline static const auto maroon = Color(0x800000ff);
        inline static const auto red = Color(0xff0000ff);
        inline static const auto purple = Color(0x800080ff);
        inline static const auto fuchsia = Color(0xff00ffff);
        inline static const auto green = Color(0x008000ff);
        inline static const auto lime = Color(0x00ff00ff);
        inline static const auto olive = Color(0x808000ff);
        inline static const auto yellow = Color(0xffff00ff);
        inline static const auto navy = Color(0x000080ff);
        inline static const auto blue = Color(0x0000ffff);
        inline static const auto teal = Color(0x008080ff);
        inline static const auto aqua = Color(0x00ffffff);
        inline static const auto orange = Color(0xffa500ff);
        inline static const auto aliceblue = Color(0xf0f8ffff);
        inline static const auto antiquewhite = Color(0xfaebd7ff);
        inline static const auto aquamarine = Color(0x7fffd4ff);
        inline static const auto azure = Color(0xf0ffffff);
        inline static const auto beige = Color(0xf5f5dcff);
        inline static const auto bisque = Color(0xffe4c4ff);
        inline static const auto blanchedalmond = Color(0xffe4c4ff);
        inline static const auto blueviolet = Color(0x8a2be2ff);
        inline static const auto brown = Color(0xa52a2aff);
        inline static const auto burlywood = Color(0xdeb887ff);
        inline static const auto cadetblue = Color(0x5f9ea0ff);
        inline static const auto chartreuse = Color(0x7fff00ff);
        inline static const auto chocolate = Color(0xd2691eff);
        inline static const auto coral = Color(0xff7f50ff);
        inline static const auto cornflowerblue = Color(0x6495edff);
        inline static const auto cornsilk = Color(0xfff8dcff);
        inline static const auto crimson = Color(0xdc143cff);
        inline static const auto darkblue = Color(0x00008bff);
        inline static const auto darkcyan = Color(0x008b8bff);
        inline static const auto darkgoldenrod = Color(0xb8860bff);
        inline static const auto darkgray = Color(0xa9a9a9ff);
        inline static const auto darkgreen = Color(0x006400ff);
        inline static const auto darkgrey = Color(0xa9a9a9ff);
        inline static const auto darkkhaki = Color(0xbdb76bff);
        inline static const auto darkmagenta = Color(0x8b008bff);
        inline static const auto darkolivegreen = Color(0x556b2fff);
        inline static const auto darkorange = Color(0xff8c00ff);
        inline static const auto darkorchid = Color(0x9932ccff);
        inline static const auto darkred = Color(0x8b0000ff);
        inline static const auto darksalmon = Color(0xe9967aff);
        inline static const auto darkseagreen = Color(0x8fbc8fff);
        inline static const auto darkslateblue = Color(0x483d8bff);
        inline static const auto darkslategray = Color(0x2f4f4fff);
        inline static const auto darkslategrey = Color(0x2f4f4fff);
        inline static const auto darkturquoise = Color(0x00ced1ff);
        inline static const auto darkviolet = Color(0x9400d3ff);
        inline static const auto deeppink = Color(0xff1493ff);
        inline static const auto deepskyblue = Color(0x00bfffff);
        inline static const auto dimgray = Color(0x696969ff);
        inline static const auto dimgrey = Color(0x696969ff);
        inline static const auto dodgerblue = Color(0x1e90ffff);
        inline static const auto firebrick = Color(0xb22222ff);
        inline static const auto floralwhite = Color(0xfffaf0ff);
        inline static const auto forestgreen = Color(0x228b22ff);
        inline static const auto gainsboro = Color(0xdcdcdcff);
        inline static const auto ghostwhite = Color(0xf8f8ffff);
        inline static const auto gold = Color(0xffd700ff);
        inline static const auto goldenrod = Color(0xdaa520ff);
        inline static const auto greenyellow = Color(0xadff2fff);
        inline static const auto grey = Color(0x808080ff);
        inline static const auto honeydew = Color(0xf0fff0ff);
        inline static const auto hotpink = Color(0xff69b4ff);
        inline static const auto indianred = Color(0xcd5c5cff);
        inline static const auto indigo = Color(0x4b0082ff);
        inline static const auto ivory = Color(0xfffff0ff);
        inline static const auto khaki = Color(0xf0e68cff);
        inline static const auto lavender = Color(0xe6e6faff);
        inline static const auto lavenderblush = Color(0xfff0f5ff);
        inline static const auto lawngreen = Color(0x7cfc00ff);
        inline static const auto lemonchiffon = Color(0xfffacdff);
        inline static const auto lightblue = Color(0xadd8e6ff);
        inline static const auto lightcoral = Color(0xf08080ff);
        inline static const auto lightcyan = Color(0xe0ffffff);
        inline static const auto lightgoldenrodyellow = Color(0xfafad2ff);
        inline static const auto lightgray = Color(0xd3d3d3ff);
        inline static const auto lightgreen = Color(0x90ee90ff);
        inline static const auto lightgrey = Color(0xd3d3d3ff);
        inline static const auto lightpink = Color(0xffb6c1ff);
        inline static const auto lightsalmon = Color(0xffa07aff);
        inline static const auto lightseagreen = Color(0x20b2aaff);
        inline static const auto lightskyblue = Color(0x87cefaff);
        inline static const auto lightslategray = Color(0x778899ff);
        inline static const auto lightslategrey = Color(0x778899ff);
        inline static const auto lightsteelblue = Color(0xb0c4deff);
        inline static const auto lightyellow = Color(0xffffe0ff);
        inline static const auto limegreen = Color(0x32cd32ff);
        inline static const auto linen = Color(0xfaf0e6ff);
        inline static const auto mediumaquamarine = Color(0x66cdaaff);
        inline static const auto mediumblue = Color(0x0000cdff);
        inline static const auto mediumorchid = Color(0xba55d3ff);
        inline static const auto mediumpurple = Color(0x9370dbff);
        inline static const auto mediumseagreen = Color(0x3cb371ff);
        inline static const auto mediumslateblue = Color(0x7b68eeff);
        inline static const auto mediumspringgreen = Color(0x00fa9aff);
        inline static const auto mediumturquoise = Color(0x48d1ccff);
        inline static const auto mediumvioletred = Color(0xc71585ff);
        inline static const auto midnightblue = Color(0x191970ff);
        inline static const auto mintcream = Color(0xf5fffaff);
        inline static const auto mistyrose = Color(0xffe4e1ff);
        inline static const auto moccasin = Color(0xffe4b5ff);
        inline static const auto navajowhite = Color(0xffdeadff);
        inline static const auto oldlace = Color(0xfdf5e6ff);
        inline static const auto olivedrab = Color(0x6b8e23ff);
        inline static const auto orangered = Color(0xff4500ff);
        inline static const auto orchid = Color(0xda70d6ff);
        inline static const auto palegoldenrod = Color(0xeee8aaff);
        inline static const auto palegreen = Color(0x98fb98ff);
        inline static const auto paleturquoise = Color(0xafeeeeff);
        inline static const auto palevioletred = Color(0xdb7093ff);
        inline static const auto papayawhip = Color(0xffefd5ff);
        inline static const auto peachpuff = Color(0xffdab9ff);
        inline static const auto peru = Color(0xcd853fff);
        inline static const auto pink = Color(0xffc0cbff);
        inline static const auto plum = Color(0xdda0ddff);
        inline static const auto powderblue = Color(0xb0e0e6ff);
        inline static const auto rosybrown = Color(0xbc8f8fff);
        inline static const auto royalblue = Color(0x4169e1ff);
        inline static const auto saddlebrown = Color(0x8b4513ff);
        inline static const auto salmon = Color(0xfa8072ff);
        inline static const auto sandybrown = Color(0xf4a460ff);
        inline static const auto seagreen = Color(0x2e8b57ff);
        inline static const auto seashell = Color(0xfff5eeff);
        inline static const auto sienna = Color(0xa0522dff);
        inline static const auto skyblue = Color(0x87ceebff);
        inline static const auto slateblue = Color(0x6a5acdff);
        inline static const auto slategray = Color(0x708090ff);
        inline static const auto slategrey = Color(0x708090ff);
        inline static const auto snow = Color(0xfffafaff);
        inline static const auto springgreen = Color(0x00ff7fff);
        inline static const auto steelblue = Color(0x4682b4ff);
        inline static const auto tan = Color(0xd2b48cff);
        inline static const auto thistle = Color(0xd8bfd8ff);
        inline static const auto tomato = Color(0xff6347ff);
        inline static const auto turquoise = Color(0x40e0d0ff);
        inline static const auto violet = Color(0xee82eeff);
        inline static const auto wheat = Color(0xf5deb3ff);
        inline static const auto whitesmoke = Color(0xf5f5f5ff);
        inline static const auto yellowgreen = Color(0x9acd32ff);
        inline static const auto rebeccapurple = Color(0x663399ff);
    };

    inline std::unordered_map<std::string, Color const&> Color::_lut =
    {
        {"black", Color::named::black},
        {"silver", Color::named::silver},
        {"gray", Color::named::gray},
        {"white", Color::named::white},
        {"maroon", Color::named::maroon},
        {"red", Color::named::red},
        {"purple", Color::named::purple},
        {"fuchsia", Color::named::fuchsia},
        {"green", Color::named::green},
        {"lime", Color::named::lime},
        {"olive", Color::named::olive},
        {"yellow", Color::named::yellow},
        {"navy", Color::named::navy},
        {"blue", Color::named::blue},
        {"teal", Color::named::teal},
        {"aqua", Color::named::aqua},
        {"orange", Color::named::orange},
        {"aliceblue", Color::named::aliceblue},
        {"antiquewhite", Color::named::antiquewhite},
        {"aquamarine", Color::named::aquamarine},
        {"azure", Color::named::azure},
        {"beige", Color::named::beige},
        {"bisque", Color::named::bisque},
        {"blanchedalmond", Color::named::blanchedalmond},
        {"blueviolet", Color::named::blueviolet},
        {"brown", Color::named::brown},
        {"burlywood", Color::named::burlywood},
        {"cadetblue", Color::named::cadetblue},
        {"chartreuse", Color::named::chartreuse},
        {"chocolate", Color::named::chocolate},
        {"coral", Color::named::coral},
        {"cornflowerblue", Color::named::cornflowerblue},
        {"cornsilk", Color::named::cornsilk},
        {"crimson", Color::named::crimson},
        {"darkblue", Color::named::darkblue},
        {"darkcyan", Color::named::darkcyan},
        {"darkgoldenrod", Color::named::darkgoldenrod},
        {"darkgray", Color::named::darkgray},
        {"darkgreen", Color::named::darkgreen},
        {"darkgrey", Color::named::darkgrey},
        {"darkkhaki", Color::named::darkkhaki},
        {"darkmagenta", Color::named::darkmagenta},
        {"darkolivegreen", Color::named::darkolivegreen},
        {"darkorange", Color::named::darkorange},
        {"darkorchid", Color::named::darkorchid},
        {"darkred", Color::named::darkred},
        {"darksalmon", Color::named::darksalmon},
        {"darkseagreen", Color::named::darkseagreen},
        {"darkslateblue", Color::named::darkslateblue},
        {"darkslategray", Color::named::darkslategray},
        {"darkslategrey", Color::named::darkslategrey},
        {"darkturquoise", Color::named::darkturquoise},
        {"darkviolet", Color::named::darkviolet},
        {"deeppink", Color::named::deeppink},
        {"deepskyblue", Color::named::deepskyblue},
        {"dimgray", Color::named::dimgray},
        {"dimgrey", Color::named::dimgrey},
        {"dodgerblue", Color::named::dodgerblue},
        {"firebrick", Color::named::firebrick},
        {"floralwhite", Color::named::floralwhite},
        {"forestgreen", Color::named::forestgreen},
        {"gainsboro", Color::named::gainsboro},
        {"ghostwhite", Color::named::ghostwhite},
        {"gold", Color::named::gold},
        {"goldenrod", Color::named::goldenrod},
        {"greenyellow", Color::named::greenyellow},
        {"grey", Color::named::grey},
        {"honeydew", Color::named::honeydew},
        {"hotpink", Color::named::hotpink},
        {"indianred", Color::named::indianred},
        {"indigo", Color::named::indigo},
        {"ivory", Color::named::ivory},
        {"khaki", Color::named::khaki},
        {"lavender", Color::named::lavender},
        {"lavenderblush", Color::named::lavenderblush},
        {"lawngreen", Color::named::lawngreen},
        {"lemonchiffon", Color::named::lemonchiffon},
        {"lightblue", Color::named::lightblue},
        {"lightcoral", Color::named::lightcoral},
        {"lightcyan", Color::named::lightcyan},
        {"lightgoldenrodyellow", Color::named::lightgoldenrodyellow},
        {"lightgray", Color::named::lightgray},
        {"lightgreen", Color::named::lightgreen},
        {"lightgrey", Color::named::lightgrey},
        {"lightpink", Color::named::lightpink},
        {"lightsalmon", Color::named::lightsalmon},
        {"lightseagreen", Color::named::lightseagreen},
        {"lightskyblue", Color::named::lightskyblue},
        {"lightslategray", Color::named::lightslategray},
        {"lightslategrey", Color::named::lightslategrey},
        {"lightsteelblue", Color::named::lightsteelblue},
        {"lightyellow", Color::named::lightyellow},
        {"limegreen", Color::named::limegreen},
        {"linen", Color::named::linen},
        {"mediumaquamarine", Color::named::mediumaquamarine},
        {"mediumblue", Color::named::mediumblue},
        {"mediumorchid", Color::named::mediumorchid},
        {"mediumpurple", Color::named::mediumpurple},
        {"mediumseagreen", Color::named::mediumseagreen},
        {"mediumslateblue", Color::named::mediumslateblue},
        {"mediumspringgreen", Color::named::mediumspringgreen},
        {"mediumturquoise", Color::named::mediumturquoise},
        {"mediumvioletred", Color::named::mediumvioletred},
        {"midnightblue", Color::named::midnightblue},
        {"mintcream", Color::named::mintcream},
        {"mistyrose", Color::named::mistyrose},
        {"moccasin", Color::named::moccasin},
        {"navajowhite", Color::named::navajowhite},
        {"oldlace", Color::named::oldlace},
        {"olivedrab", Color::named::olivedrab},
        {"orangered", Color::named::orangered},
        {"orchid", Color::named::orchid},
        {"palegoldenrod", Color::named::palegoldenrod},
        {"palegreen", Color::named::palegreen},
        {"paleturquoise", Color::named::paleturquoise},
        {"palevioletred", Color::named::palevioletred},
        {"papayawhip", Color::named::papayawhip},
        {"peachpuff", Color::named::peachpuff},
        {"peru", Color::named::peru},
        {"pink", Color::named::pink},
        {"plum", Color::named::plum},
        {"powderblue", Color::named::powderblue},
        {"rosybrown", Color::named::rosybrown},
        {"royalblue", Color::named::royalblue},
        {"saddlebrown", Color::named::saddlebrown},
        {"salmon", Color::named::salmon},
        {"sandybrown", Color::named::sandybrown},
        {"seagreen", Color::named::seagreen},
        {"seashell", Color::named::seashell},
        {"sienna", Color::named::sienna},
        {"skyblue", Color::named::skyblue},
        {"slateblue", Color::named::slateblue},
        {"slategray", Color::named::slategray},
        {"slategrey", Color::named::slategrey},
        {"snow", Color::named::snow},
        {"springgreen", Color::named::springgreen},
        {"steelblue", Color::named::steelblue},
        {"tan", Color::named::tan},
        {"thistle", Color::named::thistle},
        {"tomato", Color::named::tomato},
        {"turquoise", Color::named::turquoise},
        {"violet", Color::named::violet},
        {"wheat", Color::named::wheat},
        {"whitesmoke", Color::named::whitesmoke},
        {"yellowgreen", Color::named::yellowgreen},
        {"rebeccapurple", Color::named::rebeccapurple}
    };

    inline std::string to_string(Color const& color)
    {
        std::stringstream ss;
        ss << "0x" 
            << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>(color.red())
            << static_cast<int>(color.green()) 
            << static_cast<int>(color.blue())
            << static_cast<int>(color.alpha());
        return ss.str();
    }

}
