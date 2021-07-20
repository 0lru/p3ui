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
#include "InputScalar.h"
#include "imgui_datatype.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    template<typename DataType>
    inline void InputScalar<DataType>::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = font_size + 2 * frame_padding.y;
        _automatic_width = DefaultItemWidthEm * font_size;
        if (label())
        {
            const ImVec2 label_size = ImGui::CalcTextSize(label().value().c_str(), NULL, true);
            _automatic_width += label_size.x + context_ptr->Style.ItemInnerSpacing.x;
        }
    }

    template<typename DataType>
    inline void InputScalar<DataType>::render_impl(Context&, float width, float height)
    {
        if (label())
            ImGui::SetNextItemWidth(width * 0.65f);
        else
            ImGui::SetNextItemWidth(width);
        DataType value = _value;
        auto vptr = disabled() ? &value : &_value;
        bool changed = ImGui::InputScalar(
            imgui_label().c_str(),
            imgui_datatype<DataType>,
            vptr,
            _step ? &_step.value() : NULL,
            _format ? _format.value().c_str() : nullptr,
            0);
        if (changed && _on_change && !disabled())
            postpone([f = _on_change, value = _value]() {
                f(value);
            });
        update_status();
    }

    template InputScalar<std::int8_t>;
    template InputScalar<std::uint8_t>;
    template InputScalar<std::int16_t>;
    template InputScalar<std::uint16_t>;
    template InputScalar<std::int32_t>;
    template InputScalar<std::uint32_t>;
    template InputScalar<std::int64_t>;
    template InputScalar<std::uint64_t>;
    template InputScalar<float>;
    template InputScalar<double>;

}