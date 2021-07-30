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

#include <p3/Types.h>

namespace p3
{

    using Px = p3::parser::Px;
    using Percentage = p3::parser::Percentage;
    using Em = p3::parser::Em;
    using Rem = p3::parser::Rem;
    using Length = p3::parser::Length;

    using Length2 = std::array<Length, 2>;
    using LengthPercentage = p3::parser::LengthPercentage;
    using OptionalLengthPercentage = p3::parser::OptionalLengthPercentage;

    using LayoutLength = p3::parser::LayoutLength;
    using Direction = p3::parser::Direction;
    using Alignment = p3::parser::Alignment;
    using Justification = p3::parser::Justification;

    template<typename T> struct UnitType {};
    inline static UnitType<Px> const px;
    inline static UnitType<Em> const em;
    inline static UnitType<Rem> const rem;
    inline static UnitType<Percentage> const percent;
    inline static UnitType<std::nullopt_t> const auto_;

    template<typename T>
    Length operator |(T value, UnitType<Px> const&)
    {
        return Px{ static_cast<float>(value) };
    }

    template<typename T>
    LengthPercentage operator |(T value, UnitType<Percentage> const&)
    {
        return Percentage{ static_cast<float>(value) };
    }

    template<typename T>
    Length operator |(T value, UnitType<Em> const&)
    {
        return Em{ static_cast<float>(value) };
    }

    template<typename T>
    Length operator |(T value, UnitType<Rem> const&)
    {
        return Rem{ static_cast<float>(value) };
    }

}
