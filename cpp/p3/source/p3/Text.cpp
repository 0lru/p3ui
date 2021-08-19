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

#include "constant.h"
#include "Text.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3
{

    namespace
    {
        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            LayoutLength const& initial_height() override
            {
                static auto initial = LayoutLength{std::nullopt, 0.f, 0.f};
                return initial;
            }
            LayoutLength const& initial_width() override
            {
                static auto initial = LayoutLength{std::nullopt, 0.f, 0.f};
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& Text::style_strategy() const
    {
        return _style_strategy;
    }

    Text::Text(std::string value, std::optional<std::string> label)
        : Node("Text")
        , _value(std::move(value))
    {
        set_label(std::move(label));
    }

    void Text::render_impl(Context&, float width, float height)
    {
        
        ImGui::SetNextItemWidth(label() ? width * GoldenRatio : width);
        if (label())
        {
            ImGui::LabelText(label().value().c_str(), _value.c_str());
        }
        else
        {
            // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetCurrentConvalue()->Style.FramePadding.x);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetCurrentContext()->Style.FramePadding.y);
            ImGui::Text(_value.c_str());
            // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetCurrentContext()->Style.FramePadding.y);
            // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetCurrentContext()->Style.FramePadding.x);
        }
    }

    void Text::set_value(std::string value)
    {
        _value = std::move(value);
        set_needs_update();
        std::optional<float> x;
    }

    std::string const& Text::value() const
    {
        return _value;
    }

    void Text::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = font_size + 2.f * frame_padding.y;
        const ImVec2 label_size = ImGui::CalcTextSize(_value.c_str(), NULL, true);
        _automatic_width = label_size.x;
        if (label())
        {
            _automatic_width += context_ptr->Style.ItemInnerSpacing.x;
            _automatic_width += ImGui::CalcTextSize(label().value().c_str(), NULL, true).x;
        }
    }

}
