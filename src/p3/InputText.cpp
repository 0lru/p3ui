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
#include "InputText.h"

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

    StyleStrategy& InputText::style_strategy() const
    {
        return _style_strategy;
    }

    InputText::InputText(std::size_t size, std::optional<std::string> label)
        : Node("InputText")
        , _text()
        , _size(size)
    {
        _text.reserve(size);
        set_label(std::move(label));
    }

    void InputText::render_impl(float width, float height)
    {
        ImGui::SetNextItemWidth(label() ? width* GoldenRatio : width);
        if(_hint)
            ImGui::InputTextWithHint(imgui_label().c_str(), _hint.value().c_str(), _text.data(), _size);
        else
            ImGui::InputText(imgui_label().c_str(), _text.data(), _size);
        update_status();
    }

    void InputText::set_hint(std::optional<std::string> hint)
    {
        _hint = std::move(hint);
    }

    std::optional<std::string> const& InputText::hint(std::string) const
    {
        return _hint;
    }

    void InputText::set_callback(Callback callback)
    {
        _callback = callback;
    }

    InputText::Callback InputText::callback() const
    {
        return _callback;
    }

    void InputText::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = font_size + 2 * frame_padding.y;
        _automatic_width = DefaultItemWidthEm * font_size + 2 * frame_padding.x;
        if (label())
        {
            const ImVec2 label_size = ImGui::CalcTextSize(label().value().c_str(), NULL, true);
            _automatic_width += label_size.x + context_ptr->Style.ItemInnerSpacing.x;
        }
    }

}