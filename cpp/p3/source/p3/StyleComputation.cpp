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
#include "StyleComputation.h"

namespace p3
{

    Sizing const& StyleComputation::width_basis() const
    {
        return std::get<0>(width);
    }
    float StyleComputation::width_grow() const
    {
        return std::get<1>(width);
    }
    float StyleComputation::width_shrink() const
    {
        return std::get<2>(width);
    }
    Sizing const& StyleComputation::height_basis() const
    {
        return std::get<0>(height);
    }
    float StyleComputation::height_grow() const
    {
        return std::get<1>(height);
    }
    float StyleComputation::height_shrink() const
    {
        return std::get<2>(height);
    }

}
