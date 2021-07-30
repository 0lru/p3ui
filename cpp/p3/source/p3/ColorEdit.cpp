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

#include "ColorEdit.h"
#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    namespace
    {
        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            LayoutLength const& initial_width() override
            {
                static auto initial = LayoutLength{30|em, 0.f, 0.f};
                return initial;
            }
            LayoutLength const& initial_height() override
            {
                static auto initial = LayoutLength{std::nullopt, 0.f, 0.f};
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& ColorEdit::style_strategy() const
    {
        return _style_strategy;
    }

    ColorEdit::ColorEdit()
        : Node("ColorEdit")
        , _value{ 0.f, 0.f, 0.f, 1.f }
        , _color(0, 0, 0, 255)
    {
    }

    void ColorEdit::render_impl(Context&, float width, float height)
    {
        
        ImGui::SetNextItemWidth(width);
        if (ImGui::ColorEdit4(imgui_label().c_str(), _value))
        {
            _color = p3::Color(
                std::uint8_t(_value[0] * 255.f),
                std::uint8_t(_value[1] * 255.f),
                std::uint8_t(_value[2] * 255.f),
                std::uint8_t(_value[3] * 255.f)
            );
            if (_on_change)
            {
                postpone([color = _color, f = _on_change]() {
                    f(std::move(color));
                });
            }
        }
        update_status();
    }

    void ColorEdit::update_content()
    {
        
        auto const& context = *ImGui::GetCurrentContext();
        auto const font_size = context.FontSize;
        _automatic_height = font_size + 2 * context.Style.FramePadding.y;
        _automatic_width = font_size * 30.f;
    }

    void ColorEdit::set_on_change(OnChange on_change)
    {
        _on_change = on_change;
    }

    ColorEdit::OnChange ColorEdit::on_change() const
    {
        return _on_change;
    }

    void ColorEdit::set_value(Color color)
    {
        std::swap(_color, color);
        _value[0] = _color.red() / 255.f;
        _value[1] = _color.green() / 255.f;
        _value[2] = _color.blue() / 255.f;
        _value[3] = _color.alpha() / 255.f;
    }

    Color const& ColorEdit::value() const
    {
        return _color;
    }

}
