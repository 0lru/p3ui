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
#include <implot.h>

namespace p3
{

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

        inline void convert(ImVec4 &im, Color const& color)
        {
            im.x = color.red() / 255.f;
            im.y = color.green() / 255.f;
            im.z = color.blue() / 255.f;
            im.w = color.alpha() / 255.f;
        }

        inline void convert(Color& color, ImVec4 const& src)
        {
            color = Color(
                std::uint8_t(src.x * 255.f),
                std::uint8_t(src.y * 255.f),
                std::uint8_t(src.z * 255.f),
                std::uint8_t(src.w * 255.f)
            );
        }

        inline ImVec4 convert(Color const& color)
        {
            return ImVec4(
                color.red() / 255.f,
                color.green() / 255.f,
                color.blue() / 255.f,
                color.alpha() / 255.f
            );
        }

        inline void convert(std::optional<Color>& color, ImVec4 const& src)
        {
            if (src == IMPLOT_AUTO_COL)
                color = std::nullopt;
            else
                color = Color(
                    std::uint8_t(src.x * 255.f),
                    std::uint8_t(src.y * 255.f),
                    std::uint8_t(src.z * 255.f),
                    std::uint8_t(src.w * 255.f)
                );
        }

        inline ImVec4 convert(std::optional<Color> const& color)
        {
            if (color)
                return ImVec4(
                    color.value().red() / 255.f,
                    color.value().green() / 255.f,
                    color.value().blue() / 255.f,
                    color.value().alpha() / 255.f
                );
            else
                return IMPLOT_AUTO_COL;
        }

}