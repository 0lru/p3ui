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

#include "string_utils.h"

#include <array>
#include <optional>
#include <variant>

namespace p3
{

    struct Pixels
    {
        struct Suffix {};
        float value;
    };

    struct Percentage
    {
        struct Suffix {};
        float value;
    };

    // font height
    struct Em
    {
        struct Suffix {};
        float value;
    };

    // root font height
    struct Rem
    {
        struct Suffix {};
        float value;
    };

    using Length = std::variant<Pixels, Em, Rem>;
    using Length2 = std::array<Length, 2>;
    using LengthPercentage = std::variant<Length, Percentage>;

    inline constexpr std::nullopt_t auto_{std::nullopt_t::_Tag{}};
    inline static Pixels::Suffix const px = Pixels::Suffix{};
    inline static Em::Suffix const em = Em::Suffix{};
    inline static Rem::Suffix const rem = Rem::Suffix{};
    inline static Percentage::Suffix const percent = Percentage::Suffix{};

    template<typename T>
    Length operator |(T value, Pixels::Suffix const&)
    {
        return Pixels{ static_cast<float>(value) };
    }

    template<typename T>
    LengthPercentage operator |(T value, Percentage::Suffix const&)
    {
        return Percentage{ static_cast<float>(value) };
    }

    template<typename T>
    Length operator |(T value, Em::Suffix const&)
    {
        return Em{ static_cast<float>(value) };
    }

    template<typename T>
    Length operator |(T value, Rem::Suffix const&)
    {
        return Rem{ static_cast<float>(value) };
    }

}
