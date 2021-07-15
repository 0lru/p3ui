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
#include "Color.h"

#include <imgui.h>

namespace p3
{

        inline void convert(ImVec4 &im, Color const& oo)
        {
            im.x = std::round(oo.red() / 255.f);
            im.y = std::round(oo.green() / 255.f);
            im.z = std::round(oo.blue() / 255.f);
            im.w = std::round(oo.alpha() / 255.f);
        }

        inline ImVec4 convert(Color const& oo)
        {
            ImVec4 im;
            convert(im, oo);
            return im;
        }

        inline bool operator==(ImVec4 const& v1, ImVec4 const& v2)
        {
            return v1.x == v2.x && v1.y == v2.y && v1.w == v2.w && v1.z == v2.z;
        }

        inline bool operator!=(ImVec4 const& v1, ImVec4 const& v2)
        {
            return !(v1 == v2);
        }

        inline bool operator==(ImVec2 const& v1, ImVec2 const& v2)
        {
            return v1.x == v2.x && v1.y == v2.y;
        }

        inline bool operator!=(ImVec2 const& v1, ImVec2 const& v2)
        {
            return !(v1 == v2);
        }


}