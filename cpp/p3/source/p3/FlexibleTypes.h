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

#include "StyleTypes.h"

#include <cstdint>
#include <optional>

namespace p3
{

    using OptionalLengthPercentage = p3::parser::OptionalLengthPercentage;
    using FlexibleLength = p3::parser::FlexibleLength;

    enum class Direction
    {
        Horizontal=0,
        Vertical=1
    };

    enum class Alignment
    {
        Start=0,
        Center=1,
        End=2,
        Stretch=3,
        Baseline=4
    };

    enum class Justification
    {
        SpaceBetween = 0,
        SpaceAround = 1,
        Start = 2,
        Center = 3,
        End = 4
    };

}
