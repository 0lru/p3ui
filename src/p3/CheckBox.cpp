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

#include "CheckBox.h"

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
            FlexibleLength const& initial_height() override
            {
                static auto initial = FlexibleLength{std::nullopt, 0.f, 1.f};
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& CheckBox::style_strategy() const
    {
        return _style_strategy;
    }

    CheckBox::CheckBox(std::optional<std::string> label)
        : Node("CheckBox")
    {
        set_label(std::move(label));
    }

    void CheckBox::render_impl(float width, float height)
    {
        
        ImVec2 size(width, height);
        if (ImGui::Checkbox(imgui_label().c_str(), &_value) && _on_change)
            postpone([f = _on_change, value = _value]() {
                f(value);
            });
        update_status();
    }

    void CheckBox::set_on_change(OnChange on_change)
    {
        _on_change = on_change;
    }

    CheckBox::OnChange CheckBox::on_change() const
    {
        return _on_change;
    }

    bool CheckBox::value() const
    {
        return _value;
    }

    void CheckBox::set_value(bool value)
    {
        _value = value;
    }

    void CheckBox::update_content()
    {
        
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_width = _automatic_height = font_size + 2 * frame_padding.y;
        if (label())
        {
            const ImVec2 label_size = ImGui::CalcTextSize(label().value().c_str(), NULL, true);
            _automatic_width += label_size.x + context_ptr->Style.ItemInnerSpacing.x;
        }
    }

}